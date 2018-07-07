// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "dictionary.h"
#include "object.h"

class Node;

//////////////////////////////////////////////////////////////////////////
/// This class is used to hold save files and manage putting them onto
/// the disc, or loading them in wait. Globally available so that anyone
/// can call to save or load if needed.
//////////////////////////////////////////////////////////////////////////

// TODO: Add error handling to this class
class USaveWorker : public Object {
	GDCLASS(USaveWorker, Object)

public:
	static USaveWorker *get_singleton() { return singleton; }

	USaveWorker() {
		singleton = this;

		/// Get all subsystems and attempt to start them
		/// If they start, add them to our list
		CurrentUserFile = MakeUserDict();
		CurrentUserFile["name"] = "no_name";
		EncryptKey = "fdzierjeirofdanjodahugweig382947HFIUAfhy7935auih";
	}

	~USaveWorker() {
	}

	//////////////////////////////////////////////////////////////////////////
	/// Meta save tools.
	/// Meta save files store info related to the system, like graphics and
	/// languages options that are likely similar for most users on the device
	//////////////////////////////////////////////////////////////////////////

	/// The save file for the machine
	Dictionary MetaSaveFile;

	/// Gets the MetaSaveFile. If there isn't one, it makes one first
	Dictionary GetMetaSaveFile();

	/// The path that the Meta file is saved to.
	const String MetaSaveFileName = "MetaSave";

	/// Freezes MetaSaveFile onto the HDD.
	void SaveMeta();

	/// Applies the a Settings to the game
	/// @param Settings - The FSettingsBundle to apply.
	void SetSettings(Dictionary Settings);

	Dictionary MakeSettingsDict() {
		Dictionary meta;

		meta["aa_quality"] = 2;
		meta["post_setting"] = 2;
		meta["shadow_setting"] = 2;
		meta["texture_setting"] = 2;
		meta["resolution"] = "1920x1080";
		meta["fullscreen"] = false;
		meta["brightness"] = 1.0f;
		meta["gamma"] = 2.18f;
		meta["language_code"] = "en";
		meta["has_seen_production"] = false;
		meta["has_seen_credits"] = false;
		meta["has_selected_language"] = false;

		return meta;
	}

	//////////////////////////////////////////////////////////////////////////
	/// User and save game tools
	/// One user is loaded into the game at any time
	/// A user has information about control preferences, display prefs like
	/// sound volume and text speed, etc
	/// A user can have many saved games
	///
	/// If using cloud saves, this is the thing that would be uploaded.
	///
	//////////////////////////////////////////////////////////////////////////

	/// Current usersave file for the game. Possible that it's not serialized to the disc yet
	Dictionary CurrentUserFile;
	Dictionary GetCurrentUserFile() { return CurrentUserFile; }
	void SetCurrentUserFile(String NewUser);

	/// Index of the current save game being used from the CurrentUserFile
	String SaveSlot;
	String GetSaveSlot() { return SaveSlot; }
	void SetSaveSlot(String NewSaveSlot) { SaveSlot = NewSaveSlot; }

	/// Pushes the current save game onto the HDD
	void PushSaveGame();

	/// Saves data from the game into the CurrentSaveFile.
	void SaveGameData(bool freeze);

	// Loads all of the data from the CurrentSaveFile into the game
	void LoadGameData();

	/// Adds or updates an entry for one object into the CurrentSaveFile
	/// @param Obj - The object to save
	void SaveObjectData(Node* Obj);

	/// Applies data from CurrentSaveFile to an object
	/// @param Obj - The object to apply it to
	void LoadObjectData(Node* Obj);

	/// Deletes the save game present at FileName
	/// @param FileName - The name of the file to check for and delete
	void DeleteSaveGame(const String FileName);

	// Utility - these are used to save and load the user data. Not called in gdscript directly.

	/// Used for encrypting the save data so users can't mess with it
	String EncryptKey;

	void SaveGameToSlot(Dictionary File, String FileName);

	void DeleteGameFromSlot(String FileName);

	Dictionary LoadGameFromSlot(String FileName);

	Dictionary GetUserSaveGames() { return Dictionary(CurrentUserFile["save_games"]); }
	Dictionary GetUserCurrentSave() { return Dictionary(GetUserSaveGames()[SaveSlot]); }
	Dictionary GetUserControls() { return Dictionary(CurrentUserFile["controls"]); }

	String GetSavePath(const String FileName) { return "user://" + FileName + ".save"; }

protected:
	static USaveWorker *singleton;

	static void _bind_methods() {
		ClassDB::bind_method("get_user", &USaveWorker::GetCurrentUserFile);
		ClassDB::bind_method(D_METHOD("set_user", "user"), &USaveWorker::SetCurrentUserFile);

		ClassDB::bind_method(D_METHOD("save_game", "freeze"), &USaveWorker::SaveGameData);
		ClassDB::bind_method("load_game", &USaveWorker::LoadGameData);

		ClassDB::bind_method(D_METHOD("save_object", "node"), &USaveWorker::SaveObjectData);
		ClassDB::bind_method(D_METHOD("load_object", "node"), &USaveWorker::LoadObjectData);

		ClassDB::bind_method("get_save_slot", &USaveWorker::GetSaveSlot);
		ClassDB::bind_method(D_METHOD("set_save_slot", "save_slot"), &USaveWorker::SetSaveSlot);

		ClassDB::bind_method("get_controls", &USaveWorker::GetUserControls);

		ClassDB::bind_method("get_settings_data", &USaveWorker::GetMetaSaveFile);
		ClassDB::bind_method(D_METHOD("set_settings", "settings"), &USaveWorker::SetSettings);
	}

private:

	//////////////////////////////////////////////////////////////////////////
	/// Dictionary definitions
	//////////////////////////////////////////////////////////////////////////

	Dictionary MakeUserDict() {
		Dictionary user;

		user["name"] = "";
		user["controls"] = MakeControlsDict();
		user["subtitles"] = false;
		user["closed_captions"] = false;
		user["finished_game"] = false; /// True if the user has beat the game at least noce
		user["save_games"] = Dictionary(); /// Array of savegame dictionaries for this user
		user["last_game"] = 0; /// The array index of the most recent played game

		return user;
	}

	Dictionary MakeControlsDict() {
		Dictionary controls;

		controls["look_sensitivity"] = 1.0f;
		controls["invert_y"] = false;
		controls["invert_x"] = false;

		return controls;
	}

	Dictionary MakeSaveGame() {
		Dictionary savegame;

		/// This data is sort of a "header". Can display info about the game before really loading it.
		savegame["scene_title"] = "";
		savegame["save_time"] = 0;
		savegame["time_played"] = 0;
		savegame["records"] = Dictionary(); // Dictionary of records

		return savegame;
	}

	//////////////////////////////////////////////////////////////////////////
	/// Saving
	//////////////////////////////////////////////////////////////////////////

	/// Clears the data from ActorRecords, and then puts all savable game data into it
	/// @return A dictionary with all of the save data incorporated into it.
	/// Dictionary is of the format { node_path : (Dictionary) param_data }
	Dictionary MakeSaveGameRecords();

	/// Creates or overwrites a single referenced actor
	/// @param ActorRecords - The ActorRecords you want the actor saved to
	void MakeNodeRecord(Node *obj, Dictionary ActorRecords);

	//////////////////////////////////////////////////////////////////////////
	// Loading
	//////////////////////////////////////////////////////////////////////////

	/// Loads each actor present in the game if saved data is present for them.
	/// This function loops over all actors in the game, calling LoadActorObject
	/// for each. As each actor is loaded, a flag is checked so that they're not
	/// loaded again, to support multi stage loading.
	/// @param ActorRecords - The ActorRecords to pull from
	void IterateLoadGameData(Dictionary ActorRecords);

	/// Initializes the Obj with the Record
	/// @param Obj - The UObject in the world to load
	/// @param Record - The data to apply to the Obj
	void IterateLoadActorObject(Node *Obj, Dictionary ActorRecords);

	//////////////////////////////////////////////////////////////////////////
	/// Utilities
	//////////////////////////////////////////////////////////////////////////

	/// Resets the Loaded flag on each present actor.
	void ResetLoadedStatus();

	/// Resets the loaded flag for one actor
	/// @param obj - The actor to reset the flag on.
	void ResetObjectLoadedStatus(Node *obj);

	List<Node *> GetSaveGroup();

	//////////////////////////////////////////////////////////////////////////
	// Serialization
	//////////////////////////////////////////////////////////////////////////

	/// Does the actual work of pulling out an FObjectRecord from a given Obj
	/// @param Obj - The UObject to serialize.
	/// @return A dictionary containing values of the serialized params
	/// from the Obj, in the form { param_name : param_value }
	Dictionary SerializeNode(Node *Obj);

	/// Reads out an object's serialization from Record and puts it onto Obj
	/// @param Obj - The UObject to load data to
	/// @param Record - The record to pull data from.
	void ReadSerialObject(Node *Obj, Dictionary Record);
};
