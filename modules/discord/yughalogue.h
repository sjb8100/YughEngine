// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include <vector>

#include "class_db.h"
#include "scene/3d/area.h"
#include "scene/main/node.h"

//////////////////////////////////////////////////////////////////////////
/// The Yughalogue Framework is a generic interface to abstracts
/// out scripted events, including repeatable NPC patterns, dialogue trees,
/// and the like.
///
/// It handles making the actors show up when it's enabled, and running it
/// when they are selected.
//////////////////////////////////////////////////////////////////////////

class Yughalogue : public Node {
	GDCLASS(Yughalogue, Node)

public:
	// Actor command for how fast to move
	enum ActorMoveSpeed {
		ACTOR_MOVE_SPEED_NORMAL,
		ACTOR_MOVE_SPEED_FAST,
		ACTOR_MOVE_SPEED_SLOW
	};

	enum ActorDisableType {
		ON_YUGHALOGUE_FINISH,
		ON_YUGHALOGUE_DISABLE,
		ON_YUGHALOGUE_ENABLE
	};

	Yughalogue(){};
	~Yughalogue(){};

	void _notification(int p_what);

	/// Yughalogue will be activated when this one is played.
	/// If this is empty, it is activated at the game start.
	Yughalogue *ActivatingYughalogue;
	NodePath _activating_yughalogue;
	NodePath GetActivatingYughalogue() const { return _activating_yughalogue; }
	void SetActivatingYughalogue(NodePath val) {
		if (get_node(val)->is_class("yughalogue")) {
			_activating_yughalogue = val;
			ActivatingYughalogue = (Yughalogue*)get_node(val);
		}
	}

	/// Yughalogue will be deactivated when this one is played
	Yughalogue *DisablingYughalogue;
	NodePath _disabling_yughalogue;
	NodePath GetDisablingYughalogue() const { return _disabling_yughalogue; }
	void SetDisablingYughalogue(NodePath val) {
		if (get_node(val)->is_class("yughalogue")) {
			_disabling_yughalogue = val;
			DisablingYughalogue = (Yughalogue*)get_node(val);
		}
	}

	/// Play automatically when enabled?
	bool PlayOnEnable;
	bool GetPlayOnEnable() const { return PlayOnEnable; }
	void SetPlayOnEnable(bool val) { PlayOnEnable = val; }

	/// If true, self disables this yughalogue when it is set to complete.
	bool DisableOnComplete;
	bool GetDisableOnComplete() const { return DisableOnComplete; }
	void SetDisableOnComplete(bool val) { DisableOnComplete = val; }

	/// If true, activates this YG when the ActivatingYughalogue is set to complete.
	bool ActivateOnComplete;
	bool GetActivateOnComplete() const { return ActivateOnComplete; }
	void SetActivateOnComplete(bool val) { ActivateOnComplete = val; }

	/// Will play when the player steps into this area
	Area *Trigger;
	NodePath _trigger;
	NodePath GetTrigger() const { return _trigger; }
	void SetTrigger(NodePath val) {
		if (get_node(val)->is_class("area")) {
			_trigger = val;
			Trigger = (Area*)get_node(val);
		}	
	}

	/// Deactivates this yughalogue when passed through.
	Area *TriggerDeactivator;
	NodePath _trigger_deactivator;
	NodePath GetTriggerDeactivator() const { return _trigger_deactivator; }
	void SetTriggerDeactivator(NodePath val) {
		if (get_node(val)->is_class("area")) {
			_trigger_deactivator = val;
			TriggerDeactivator = (Area*)get_node(val);
		}
	}

	/// Yughalogue will play when the game starts
	bool PlayOnStart;
	bool GetPlayOnStart() const { return PlayOnStart; }
	void SetPlayOnStart(bool val) { PlayOnStart = val; }

	/// Yughalogue will be prepared to play on game start
	bool StartPrepared;
	bool GetStartPrepared() const { return StartPrepared; }
	void SetStartPrepared(bool val) { StartPrepared = val; }

	/// The max number of times the Yughalogue can play.
	/// If <= 0, considered to be infinite.
	int MaxPlays;
	int GetMaxPlays() const { return MaxPlays; }
	void SeteMaxPlays(int val) { MaxPlays = val; }

	/// Will wait for this actor to step into the trigger
	Node *Activator;

	bool FirstTime;

	bool Played;

	/// Delay before playing for PlayOnStart
	float PlayOnStartDelay;

	/// True if the Yughalogue is currently playing
	bool Playing;

	/// True if this has been played at least once
	bool HasPlayed;

	/// The number of times the Yughalogue has played through
	int TimesPlayed;

	//////////////////////////////////////////////////////////////////////////
	// Functions
	//////////////////////////////////////////////////////////////////////////

	/// Checks if it can be played, and if so, preforms setup to actually play it
	void PlayFromBeginningYughalogue();

	void Resume();

	/// Call to suspend the currently playing yughalogue
	void Suspend();

	/// Call to play the Yughalogue again
	void Repeat();

	/// This should be called when it's done.
	/// @param Complete - True if it being finish also implies "complete"
	void FinishedYughalogue(bool Complete);

	/// Reset its number of times played and so on.
	void ResetStats();

	/// Determines if the Yughalogue can be played based on its current state
	bool GetCanPlay();

	/// Return true if it can be played based on its number of times played
	bool CheckTimesPlayed();

	void EnableSelf();

	/// Starts the Yughalogue with the given Times played
	void YughStart(int Time);

	int CheckStatusPosition;

	void FirstPlaySetup();

	/// Call this to make all active Yughalogues check for initialization
	static void CheckAllYughalogues();

	// TODO: Not sure what this does
	void YughWarmup();

protected:
	/// Called only internally and should be reimplementedby children who need different
	/// uses
	//void StartYughalogue();

	static void _bind_methods() {

		ClassDB::bind_method("get_activating_yughalogue", &Yughalogue::GetActivatingYughalogue);
		ClassDB::bind_method(D_METHOD("set_activating_yughalogue", "yughalogue"), &Yughalogue::SetActivatingYughalogue);
		ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "activating_yughalogue"), "set_activating_yughalogue", "get_activating_yughalogue");

		ClassDB::bind_method("get_disabling_yughalogue", &Yughalogue::GetDisablingYughalogue);
		ClassDB::bind_method(D_METHOD("set_disabling_yughalogue", "yughalogue"), &Yughalogue::SetDisablingYughalogue);
		ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "disabling_yughalogue"), "set_disabling_yughalogue", "get_disabling_yughalogue");

		ClassDB::bind_method("get_play_on_enable", &Yughalogue::GetPlayOnEnable);
		ClassDB::bind_method(D_METHOD("set_play_on_enable", "yughalogue"), &Yughalogue::SetPlayOnEnable);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "play_on_enable"), "set_play_on_enable", "get_play_on_enable");

		ClassDB::bind_method("get_disable_on_complete", &Yughalogue::GetDisableOnComplete);
		ClassDB::bind_method(D_METHOD("set_disable_on_complete", "yughalogue"), &Yughalogue::SetDisableOnComplete);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "disable_on_complete"), "set_disable_on_complete", "get_disable_on_complete");

		ClassDB::bind_method("get_activate_on_complete", &Yughalogue::GetActivateOnComplete);
		ClassDB::bind_method(D_METHOD("set_activate_on_complete", "yughalogue"), &Yughalogue::SetActivateOnComplete);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "activate_on_complete"), "set_activate_on_complete", "get_activate_on_complete");

		ClassDB::bind_method("get_trigger", &Yughalogue::GetTrigger);
		ClassDB::bind_method(D_METHOD("set_trigger", "yughalogue"), &Yughalogue::SetTrigger);
		ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "trigger"), "set_trigger", "get_trigger");

		ClassDB::bind_method("get_trigger_deactivator", &Yughalogue::GetTriggerDeactivator);
		ClassDB::bind_method(D_METHOD("set_trigger_deactivator", "yughalogue"), &Yughalogue::SetTriggerDeactivator);
		ADD_PROPERTY(PropertyInfo(Variant::NODE_PATH, "trigger_deactivator"), "set_trigger_deactivator", "get_trigger_deactivator");

		ClassDB::bind_method("play", &Yughalogue::PlayFromBeginningYughalogue);
		ClassDB::bind_method("resume", &Yughalogue::Resume);
		ClassDB::bind_method("suspend", &Yughalogue::Suspend);
		ClassDB::bind_method("repeat", &Yughalogue::Repeat);
		ClassDB::bind_method(D_METHOD("finish", "complete"), &Yughalogue::FinishedYughalogue);

		ClassDB::bind_method("enable_self", &Yughalogue::EnableSelf);
		ClassDB::bind_method("disable_self", &Yughalogue::DisableSelf);
		ClassDB::bind_method("check_trigger", &Yughalogue::CheckTrigger);
		ClassDB::bind_method("check_trigger_deactivate", &Yughalogue::CheckTriggerDeactivate);

		ADD_SIGNAL(MethodInfo("enabled_yughalogue"));
		ADD_SIGNAL(MethodInfo("started_yughalogue"));
		ADD_SIGNAL(MethodInfo("resumed_yughalogue"));
		ADD_SIGNAL(MethodInfo("finished_yughalogue"));
		ADD_SIGNAL(MethodInfo("completed_yughalogue"));

		BIND_ENUM_CONSTANT(ACTOR_MOVE_SPEED_NORMAL)
		BIND_ENUM_CONSTANT(ACTOR_MOVE_SPEED_FAST)
		BIND_ENUM_CONSTANT(ACTOR_MOVE_SPEED_SLOW)

		BIND_ENUM_CONSTANT(ON_YUGHALOGUE_FINISH)
		BIND_ENUM_CONSTANT(ON_YUGHALOGUE_DISABLE)
		BIND_ENUM_CONSTANT(ON_YUGHALOGUE_ENABLE)
	}

private:
	/// True if the Yughalogue is doing a return trip
	bool Returning;

	/// The return value
	int ReturnValue;

	/// True if the Yughalogue will play when called
	bool Playable;

	/// This is set to true when the Yughalogue is "complete" - that is, when its primary task is done.
	bool Completed;

	bool Enabled;

	void Initialize();

	void DisableSelf();

	/// Run steps to play the yughalogue
	void ExecutePlayingSequence();

	void CheckTriggerDeactivate(Node *OverlappedActor, Node *OtherActor);

	void CheckTrigger(Node *OverlappedActor, Node *OtherActor);

	void DisablingYughalogueStarted();

	//static std::vector<Yughalogue *> Yughalogues;
};

VARIANT_ENUM_CAST(Yughalogue::ActorMoveSpeed);
VARIANT_ENUM_CAST(Yughalogue::ActorDisableType);
