#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/resources/texture.h"

// All of the net systems (Steam, Discord, etc), must inheret from this.
// They implement their behavior
class NetSystem {

public:
	NetSystem() {}

	// The destroy function should close the system down
	~NetSystem() {}

	// True if this system uses sessions
	bool Sessions;

	// True if this system uses cloud saves
	bool CloudSaves;

	virtual void RunCallbacks() = 0;

	virtual void WriteAchievementProgress(String Name, float Progress) = 0;

	// Starts the subsystem
	// @return True if the subsystem is available on this platform
	// Not necessarily if it successfully started.
	virtual bool Initialize() = 0;
};

//////////////////////////////////////////////////////////////////////////
// Online networking integration for multiple backends in godot 3.
// Available backends will start and run. One backend must be labeled
// as primary for certain functions to run on. Other functions will
// run on all subsystems that are available.
//////////////////////////////////////////////////////////////////////////

class Network : public Node {
	GDCLASS(Network, Node);

public:
	static Network *get_singleton() { return singleton; }

	Network() {
		singleton = this;

		// Get all subsystems and attempt to start them
		// If they start, add them to our list
	}

	~Network() {
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods to establish servers and join them
	// Sessions can either be lobbies or games in progress. Any instance
	// of the game that is visible on the net.
	//
	// These only work for one subsystem. It doesn't make sense for a game
	// to do this across multiple backends.
	//
	// As an example, a user may have purchased your game on Steam, yet
	// you have your own backend which does the matchmaking, but which
	// replicates achievements and rich presence. By setting your backend
	// as the "primary", calling these functions will update both your and
	// steam's rich presence and achievements, but the sessions calls will
	// be routed to your subsystem.
	//
	// There are four networking systems to declare:
	// Sessions - Used for matchmaking and game joining. One system only
	// Cloud saves - Used to save game data to the cloud. One system only
	// Achievements - Used to track achievements. All systems
	// Rich presence - Shows detailed info about the current game. All systems
	//////////////////////////////////////////////////////////////////////////

	// A Vector of all of the registered networks
	List<NetSystem *> Networks;

	// Used by many online systems to give feedback to the program
	void RunCallbacks() {
		// TODO: Figure out how to use iterators or anything here besides this
		for (int i = 0; i < Networks.size(); i++)
			Networks[i]->RunCallbacks();
	}

	// Creates a session from the default online subsystem.
	// Can include parameters based on game mode.
	void CreateSession(String Name = "EMPTY") {}

	// Destroys the current session. Only the host should do this.
	void DestroySession() {}

	// Returns a list of sessions matching some criteria.
	void FindSessions() {}

	// Attempts to join a session based on some criteria.
	// If the session is not joinable, returns why.
	void JoinSession(String Name = "EMPTY") {}

	// Updates the session with new info. Examples are a new name, or password,
	// or number of current players, and so on.
	void UpdateSession() {

	}

	//////////////////////////////////////////////////////////////////////////
	// For getting friends, avatars, etc
	//////////////////////////////////////////////////////////////////////////

	// Call to get the user handle of the player
	String GetHandle() { return ""; }

	// Call to get the avatar of the player
	void GetAvatar() {}

	//////////////////////////////////////////////////////////////////////////
	// Methods to push to the cloud, save games, etc
	// Only works for the primary subsystem.
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Methods for achievements
	//////////////////////////////////////////////////////////////////////////

	// Writes the achievement progress to all available subsystems.
	void WriteAchievementProgress(String Name, float Progress) {
		/*for (NetSystem *Network : Networks)
			Network->WriteAchievementProgress(Name, Progress);*/
	}

	//////////////////////////////////////////////////////////////////////////
	// Methods for rich presence. Updates rich presence across all subsystems
	// that are active.
	//////////////////////////////////////////////////////////////////////////

protected:
	static Network *singleton;

	// Binding all methods and signals
	static void _bind_methods() {
		//ClassDB::bind_method("InitDiscord", &Discord::InitDiscord);

		//ADD_SIGNAL(MethodInfo("discord_ready"));
		//ADD_SIGNAL(MethodInfo("discord_error", PropertyInfo(Variant::INT, "errcode"), PropertyInfo(Variant::STRING, "message")));

		//ADD_SIGNAL(MethodInfo("found_sessions", PropertInfo(Variant::ARRAY, "sessions")));

		//// Bindings for the constants defined by Discord
		//BIND_CONSTANT(DISCORD_REPLY_NO);
	}

	void _notification(int p_what);
};
