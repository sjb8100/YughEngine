// Fill out your copyright notice in the Description page of Project Settings.

#include "save_worker.h"
#include "engine.h"
#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "core/os/file_access.h"
#include "core/io/json.h"
#include "core/os/dir_access.h"
#include "core/io/file_access_encrypted.h"
#include "scene/main/viewport.h"
#include "scene/resources/environment.h"
#include "core/io/file_access_memory.h"
#include "core/math/math_funcs.h"
#include "core/variant_parser.h"

USaveWorker *USaveWorker::singleton = nullptr;

//////////////////////////////////////////////////////////////////////////
// Meta stuff
//////////////////////////////////////////////////////////////////////////

Dictionary USaveWorker::GetMetaSaveFile() {

	if (!FileAccess::exists("user://" + MetaSaveFileName + ".save")) {
		MetaSaveFile = MakeSettingsDict();
		SaveMeta();
	} else
		MetaSaveFile = LoadGameFromSlot(MetaSaveFileName);

	return MetaSaveFile;
}

void USaveWorker::SaveMeta() {
	SaveGameToSlot(MetaSaveFile, MetaSaveFileName);
}

void USaveWorker::SetSettings(Dictionary Settings) {
		// OS variables are VSync and resolution/window stuff
		OS::get_singleton()->set_use_vsync(false);
		//OS.get_singleton()->set_window_size();
		OS::get_singleton()->set_window_fullscreen(true);

		// Viewport decides shadow quality and AA
		SceneTree::get_singleton()->get_root()->set_msaa(Viewport::MSAA_4X);
		SceneTree::get_singleton()->get_root()->set_shadow_atlas_size(1024);

		// Environment variables are mostly about post process
		// Blur, glow, fog, etc
		SceneTree::get_singleton()->get_root()->get_world()->get_environment()->set_adjustment_brightness(1.0f);
		SceneTree::get_singleton()->get_root()->get_world()->get_environment()->set_dof_blur_far_quality(Environment::DOF_BLUR_QUALITY_MEDIUM);
		SceneTree::get_singleton()->get_root()->get_world()->get_environment()->set_dof_blur_near_quality(Environment::DOF_BLUR_QUALITY_HIGH);

		// Missing:
		// Set texture quality
}

//////////////////////////////////////////////////////////////////////////
// User save stuff
//////////////////////////////////////////////////////////////////////////

void USaveWorker::SetCurrentUserFile(String NewUser) {
	if (FileAccess::exists(GetSavePath(NewUser)))
		CurrentUserFile = LoadGameFromSlot(NewUser).duplicate();
	else {
		CurrentUserFile = MakeUserDict();
		CurrentUserFile["name"] = NewUser;
		PushSaveGame();
	}
}

void USaveWorker::SaveGameData(bool freeze) {
	Dictionary new_save_data = MakeSaveGame();
	new_save_data["records"] = MakeSaveGameRecords();
	GetUserSaveGames()[SaveSlot] = new_save_data.duplicate();

	if (freeze)
		PushSaveGame();
		
}

void USaveWorker::LoadGameData() {
	IterateLoadGameData(GetUserCurrentSave()["records"]);
}

void USaveWorker::PushSaveGame() {
	SaveGameToSlot(CurrentUserFile, CurrentUserFile["name"]);
}

void USaveWorker::DeleteSaveGame(const String FileName) {
	// If we're deleting the currently docked save, reset it
	if (CurrentUserFile["save_slot_name"] == FileName)
		CurrentUserFile = MakeSaveGame();

	DeleteGameFromSlot(FileName);
}

void USaveWorker::SaveObjectData(Node *Obj) {
	MakeNodeRecord(Obj, GetUserCurrentSave()["records"]);
}

void USaveWorker::LoadObjectData(Node *Obj) {
	IterateLoadActorObject(Obj, GetUserCurrentSave()["records"]);
}

//////////////////////////////////////////////////////////////////////////
// Utilities
//////////////////////////////////////////////////////////////////////////

void USaveWorker::SaveGameToSlot(Dictionary File, String FileName) {
	/*if (EncryptKey == "") {
		FileAccess *saver = FileAccess::open(GetSavePath(FileName), FileAccess::WRITE);
		saver->store_line(JSON::print(File));
		saver->close();
	} else {
		FileAccess *saver = FileAccess::open(GetSavePath(FileName), FileAccess::WRITE);
		FileAccessEncrypted* encrypter = new FileAccessEncrypted();
		encrypter->open_and_parse_password(saver, EncryptKey, FileAccessEncrypted::MODE_WRITE_AES256);
		encrypter->close();
		saver->close();
	}*/

	FileAccess *saver = FileAccess::open(GetSavePath(FileName), FileAccess::WRITE);
	saver->store_line(JSON::print(File));
	saver->close();
}

// Saving/loading functions
Dictionary USaveWorker::LoadGameFromSlot(String FileName) {
	Dictionary pulled;

	// File access params
	Variant loaded;
	String err;
	int err_int;

	if (FileAccess::exists(GetSavePath(FileName))) {
		/*if (EncryptKey == "") {
			FileAccess *loader = FileAccess::open(GetSavePath(FileName), FileAccess::READ);
			JSON::parse(loader->get_line(), loaded, err, err_int);
			pulled = Dictionary(loaded);
			loader->close();
		} else {
			FileAccess *loader = FileAccess::open(GetSavePath(FileName), FileAccess::READ);
			FileAccessEncrypted *decrypter = new FileAccessEncrypted();
			decrypter->open_and_parse_password(loader, EncryptKey, FileAccessEncrypted::MODE_READ);
			JSON::parse(loader->get_line(), loaded, err, err_int);
			pulled = Dictionary(loaded);
			decrypter->close();
			loader->close();
		}*/

		FileAccess *loader = FileAccess::open(GetSavePath(FileName), FileAccess::READ);
		JSON::parse(loader->get_line(), loaded, err, err_int);
		pulled = Dictionary(loaded);
		loader->close();
	}

	return pulled;
}

void USaveWorker::DeleteGameFromSlot(String FileName) {

}

// Iterate saves and loads

Dictionary USaveWorker::MakeSaveGameRecords() {
	Dictionary newrecords;

	List<Node *> nodes = GetSaveGroup();
	for (int i = 0; i < nodes.size(); i++)
		MakeNodeRecord(nodes[i], newrecords);

	return newrecords;
}

void USaveWorker::MakeNodeRecord(Node *obj, Dictionary ActorRecords) {
	obj->call("save_game"); // The node should prepare itself for saving
	ActorRecords[String(obj->get_path())] = SerializeNode(obj);
}

Dictionary USaveWorker::SerializeNode(Node *Obj) {
	Dictionary record;

	// Nodes should make an array named "saves" with a list of values to save
	Array save_params = Obj->get("saves");

	// For each item in the save_params, hash that value into a dict
	for (int i = 0; i < save_params.size(); i++)
		record[save_params[i]] = Obj->get(save_params[i]).get_construct_string();

	return record;
}

void USaveWorker::IterateLoadGameData(Dictionary ActorRecords) {
	// TODO: Would probably be better to just get all nodes, not only those in the group
	List<Node *> nodes = GetSaveGroup();

	for (int i = 0; i < nodes.size(); i++)
		IterateLoadActorObject(nodes[i], ActorRecords);
}

void USaveWorker::IterateLoadActorObject(Node *Obj, Dictionary ActorRecords) {
	if (ActorRecords.has(String(Obj->get_path()))) {
		ReadSerialObject(Obj, ActorRecords[String(Obj->get_path())]);
		Obj->call("load_game");
	}
}

void USaveWorker::ReadSerialObject(Node *Obj, Dictionary Record) {
	Array keys = Record.keys();

	for (int i = 0; i < keys.size(); i++) {
		Variant val;
		String err;
		int err_line;
		VariantParser::StreamString* ss = new VariantParser::StreamString();
		ss->s = Record[keys[i]];
		VariantParser::parse(ss, val, err, err_line);
		Obj->set(keys[i], val);
	}
		
}

void USaveWorker::ResetLoadedStatus() {
	List<Node *> nodes = GetSaveGroup();

	for (int i = 0; i < nodes.size(); i++)
		nodes[i]->set("loaded", false);
}

void USaveWorker::ResetObjectLoadedStatus(Node *obj) {
	obj->set("loaded", false);
}

List<Node *> USaveWorker::GetSaveGroup() {
	List<Node *> nodes;
	SceneTree::get_singleton()->get_nodes_in_group("save", &nodes);
	return nodes;
}
