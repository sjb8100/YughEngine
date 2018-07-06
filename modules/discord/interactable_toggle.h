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
	Ref<Animation> GetAnimToggle() { return AnimToggle; }
	void SetAnimToggle(const Ref<Animation> p_anim) { AnimToggle = p_anim; }

	// Play when the toggle switches to false
	Ref<Animation> AnimUntoggle;
	Ref<Animation> GetAnimUntoggle() { return AnimUntoggle; }
	void SetAnimUntoggle(const Ref<Animation> p_anim) { AnimUntoggle = p_anim; }
	
	Ref<AudioStream> ToggleSound;
	Ref<AudioStream> GetToggleSound() { return ToggleSound; }
	void SetToggleSound(const Ref<AudioStream> p_sound) { ToggleSound = p_sound; }
	
	Ref<AudioStream> UntoggleSound;
	void SetUntoggleSound(const Ref<AudioStream> sound) { UntoggleSound = sound; }
	Ref<AudioStream> GetUntoggleSound() { return UntoggleSound; }

	// TODO: Add sounds

	// If true, only the AnimToggle is used, and it is played in reverse for toggle false
	bool ForwardBackAnim;
	bool GetForwardBackAnim() const { return ForwardBackAnim; }
	void SetForwardBackAnim(bool val) { ForwardBackAnim = val; }

	bool Toggled;
	bool GetToggled() const { return Toggled; }
	void SetToggled(bool val) { Toggled = val; }

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

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_toggle_animation", "animation"), &InteractableToggle::SetAnimToggle);
		ClassDB::bind_method("get_toggle_animation", &InteractableToggle::GetAnimToggle);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "toggle_animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_toggle_animation", "get_toggle_animation");

		ClassDB::bind_method(D_METHOD("set_toggle_sound", "sound"), &InteractableToggle::SetToggleSound);
		ClassDB::bind_method("get_toggle_sound", &InteractableToggle::GetToggleSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "toggle_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_toggle_sound", "get_toggle_sound");

		ClassDB::bind_method(D_METHOD("set_untoggle_animation", "animation"), &InteractableToggle::SetAnimUntoggle);
		ClassDB::bind_method("get_untoggle_animation", &InteractableToggle::GetAnimUntoggle);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "untoggle_animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_untoggle_animation", "get_untoggle_animation");

		ClassDB::bind_method(D_METHOD("set_untoggle_sound", "sound"), &InteractableToggle::SetUntoggleSound);
		ClassDB::bind_method("get_untoggle_sound", &InteractableToggle::GetUntoggleSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "untoggle_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_untoggle_sound", "get_untoggle_sound");
	}
};
