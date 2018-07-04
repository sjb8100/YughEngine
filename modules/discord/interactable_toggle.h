// Copyright 2018 odplot productions LLC. All rights reserved.

#pragma once

#include "interactable.h"

// A Toggle animation component should be attached to objects that need to save
// a two way switch interaction, like a door or lightswitch. Animations and sounds
// play for toggling both ways.
class InteractableToggle : public Interactable {
	GDCLASS(InteractableToggle, Interactable)

public:
	// Play when the toggle switches to true
	Ref<Animation> AnimToggle;

	// Play when the toggle switches to false
	Ref<Animation> AnimUntoggle;

	Ref<AudioStream> ToggleSound;

	Ref<AudioStream> UntoggleSound;

	// TODO: Add sounds

	// If true, only the AnimToggle is used, and it is played in reverse for toggle false
	bool ForwardBackAnim;

	bool Toggled;

	void SetupAnimationState();

	// Call to pause the anim
	void PauseAnimation();

	// Call to resume the anim
	void ResumeAnimation();

	void ShortCircuitToggle(bool Toggle);

	virtual void ExecInteract(bool SilentExec) override;

private:
	// Track the current montage
	Ref<Animation> CurrrentPlayingMontage;

	static void _bind_methods() {

	}
};
