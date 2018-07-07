// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include <algorithm>

#include "yughalogue.h"

//std::vector<Yughalogue*> Yughalogue::Yughalogues;

void Yughalogue::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_ENTER_TREE:

			if (!ActivatingYughalogue || StartPrepared)
				EnableSelf();

			if (PlayOnStart) {
				// TODO: Wait for some delay time ...

				// Play the sequence, ignoring if it can or not
				ExecutePlayingSequence();
			}

			// Add to a "YughalogueManager"
			//Yughalogues.push_back(this);

			if (ActivatingYughalogue) {
				if (ActivateOnComplete)
					ActivatingYughalogue->connect("completed_yughalogue", this, "enable_self");
				else
					ActivatingYughalogue->connect("started_yughalogue", this, "enable_self");
			}

			if (DisablingYughalogue)
				DisablingYughalogue->connect("started_yughalogue", this, "disable_self");

			if (Trigger)
				Trigger->connect("area_entered", this, "check_trigger");

			if (TriggerDeactivator)
				TriggerDeactivator->connect("area_entered", this, "check_trigger_deactivate");

			break;

		case NOTIFICATION_EXIT_TREE:
			// Deleting by value as explained here https://stackoverflow.com/questions/3385229/c-erase-vector-element-by-value-rather-than-by-position
			//Yughalogues.erase(std::remove(Yughalogues.begin(), Yughalogues.end(), this), Yughalogues.end());
			break;
	}
}

void Yughalogue::Repeat() {
	FinishedYughalogue(false);
	//StartYughalogue();
}

void Yughalogue::FinishedYughalogue(bool Complete) {
	TimesPlayed++;
	Playing = false;
	emit_signal("finished_yughalogue");

	// Do Finish stuff

	if (Complete) {
		Completed = true;
		emit_signal("completed_yughalogue");

		// TODO: Consider making an OnCompleted function?
		if (DisableOnComplete)
			DisableSelf();
	}
}

void Yughalogue::ResetStats() {
	TimesPlayed = ReturnValue = 0;
}

bool Yughalogue::GetCanPlay() {
	return (Playable && !Playing && CheckTimesPlayed());
}

bool Yughalogue::CheckTimesPlayed() {
	return (TimesPlayed < MaxPlays || MaxPlays <= 0);
}

void Yughalogue::EnableSelf() {
	Enabled = Playable = true;

	if (PlayOnEnable)
		PlayFromBeginningYughalogue();

	emit_signal("enabled_yughalogue");
}

void Yughalogue::PlayFromBeginningYughalogue() {
	if (GetCanPlay())
		ExecutePlayingSequence();
}

void Yughalogue::ExecutePlayingSequence() {
	if (FirstTime)
		FirstPlaySetup();

	Returning = false;
	Playing = Played = HasPlayed = true;

	YughStart(TimesPlayed);
	emit_signal("started_yughalogue");

	// Freeze players, ban categories, etc
}

void Yughalogue::Suspend() {
	Playing = false;
	YughStart(TimesPlayed);
}

void Yughalogue::YughStart(int Time) {
}

void Yughalogue::DisableSelf() {
	Playable = false;
	Playing = false;
	YughStart(TimesPlayed);
}

void Yughalogue::FirstPlaySetup() {
	FirstTime = false;

	// Unlock and lock doors, hide or reveal actors, etc. General setup
}

void Yughalogue::CheckAllYughalogues() {
	// Bubblesort based on a "CheckStatusPosition" int

	//for (auto Yughalogue : Yughalogues)
		//Yughalogue->Initialize();
}

void Yughalogue::Initialize() {
	// Hide or show actors based on its current state.
	// For instance, if it can't play, make its actors disabled
}

void Yughalogue::CheckTriggerDeactivate(Node *OverlappedActor, Node *OtherActor) {
	// Check for if the overlapping actor is the yughalogue controller

	// TODO: Can this be moved into the trigger itself?
	Suspend();
}

void Yughalogue::CheckTrigger(Node *OverlappedActor, Node *OtherActor) {
	PlayFromBeginningYughalogue();
}

void Yughalogue::YughWarmup() {
	YughStart(TimesPlayed);
}

void Yughalogue::Resume() {
	Returning = Playing = true;

	YughStart(TimesPlayed);
}

void Yughalogue::DisablingYughalogueStarted() {
	DisableSelf();
	Suspend();
}
