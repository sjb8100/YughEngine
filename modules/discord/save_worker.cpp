// Fill out your copyright notice in the Description page of Project Settings.

#include "save_worker.h"
#include "engine.h"
#include "core/os/os.h"
#include "scene/main/scene_tree.h"
#include "core/os/file_access.h"
#include "core/io/json.h"
#include "core/os/dir_access.h"
#include "scene/main/viewport.h"
#include "scene/resources/environment.h"

USaveWorker *USaveWorker::singleton = nullptr;

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

void USaveWorker::PushSaveGame() {
	// TODO: Save based on the save file's name
	SaveGameToSlot(CurrentUserFile, "p");
}

void USaveWorker::PullSaveGame(const String FileName) {
	CurrentUserFile = LoadGameFromSlot(FileName);
}

void USaveWorker::SaveGameData(bool freeze) {
	Dictionary new_save_data = MakeSaveGame();
	new_save_data["records"] = IterateSaveGameData();
	GetUserSaveGames()[SaveSlot] = new_save_data;

	if (freeze)
		PushSaveGame();
}

void USaveWorker::LoadGameData() {
	IterateLoadGameData(GetUserCurrentSave()["records"]);
}

void USaveWorker::SaveObjectData(Node* Obj) {
	IterateSaveObjectData(Obj, GetUserCurrentSave()["records"]);
}

void USaveWorker::LoadObjectData(Node* Obj) {
	IterateLoadActorObject(Obj, GetUserCurrentSave()["records"]);
}

void USaveWorker::DeleteSaveGame(const String FileName) {
	// If we're deleting the currently docked save, reset it
	if (CurrentUserFile["save_slot_name"] == FileName)
		CurrentUserFile = MakeSaveGame();


	DeleteGameFromSlot(FileName);
}

void USaveWorker::ArchiveSaveGame(String ArchiveName) {
	//CurrentSaveFile->ActiveGame.SceneTitle = ArchiveName;
	//CurrentSaveFile->ArchivedGames.Add(CurrentSaveFile->ActiveGame);
}

void USaveWorker::SaveGameToSlot(Dictionary File, String FileName) {
	//FileAccess* saver = new FileAccess();

	//DirAccess* p = DirAccess::create_for_path("user://" + FileName + ".save");

	//saver.open("user://" + FileName + ".save", FileAccess::WRITE);
	//// TODO: Is this OK?
	//saver.store_line(JSON::print(File));
	//saver.close();
}

Dictionary USaveWorker::LoadGameFromSlot(String FileName) {
	//FileAccess* loader = new FileAccess();

	//if (loader.file_exists(GetSavePath(FileName))) {
	//	loader.open(GetSavePath(FileName), FileAccess::READ);
	//	String err;
	//	int err_line;
	//	Dictionary load;
	//	JSON::parse(loader.get_line(), load, err, err_line);
	//	// TODO: Check if there was an error or not
	//	return load;
	//}

	// If there wasn't a file, return an empty dictionary
	Dictionary empty;
	return empty;
}

void USaveWorker::DeleteGameFromSlot(String FileName) {
	String f = FileName + ".save";
	DirAccess* p = DirAccess::create_for_path("user://");
	if (p->file_exists(f)) {
		p->remove(f);
	}
}

Dictionary USaveWorker::IterateSaveGameData() {
	Dictionary newrecords;

	List<Node *> nodes = GetSaveGroup();
	for (int i = 0; i < nodes.size(); i++)
		IterateSaveObjectData(nodes[i], newrecords);

	return newrecords;
}

void USaveWorker::IterateSaveObjectData(Node *obj, Dictionary ActorRecords) {
	obj->call("save_game"); // The node should prepare itself for saving
	ActorRecords[obj->get_path()] = SerializeObject(obj);
}

Dictionary USaveWorker::SerializeObject(Node *Obj) {
	Dictionary record;

	// Nodes should make an array named "saves" with a list of values to save
	Array save_params = Obj->get("saves");

	// For each item in the save_params, hash that value into a dict
	for (int i = 0; i < save_params.size(); i++)
		record[save_params[i]] = Obj->get(save_params[i]);

	return record;
}

void USaveWorker::IterateLoadGameData(Dictionary ActorRecords) {
	// TODO: Would probably be better to just get all nodes, not only those in the group
	List<Node *> nodes = GetSaveGroup();

	for (int i = 0; i < nodes.size(); i++)
		IterateLoadActorObject(nodes[i], ActorRecords);
}

void USaveWorker::IterateLoadActorObject(Node *Obj, Dictionary ActorRecords) {
	if (ActorRecords.has(Obj->get_path())) {
		ReadSerialObject(Obj, ActorRecords[Obj->get_path()]);
		Obj->call("load_game");
	}
}

void USaveWorker::ReadSerialObject(Node *Obj, Dictionary Record) {
	Array keys = Record.keys();

	for (int i = 0; i < keys.size(); i++)
		Obj->set(keys[i], Record[keys[i]]);
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
