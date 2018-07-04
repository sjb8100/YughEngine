// Copyright 2018 odplot productions LLC. All rights reserved.

#pragma once

#include "interactable.h"

// This component should be put on objects to give then a one shot interact method.
// One shot means that an animation plays, and a sound plays, and then that can
// be repeated.
class InteractableOneshot : public Interactable {
	GDCLASS(InteractableOneshot, Interactable)

public:
	virtual void ExecInteract(bool SilentExec) override;
	virtual void ShortCircuitInteract() override;

	Ref<Animation> PlayAnim;
	Ref<Animation> GetPlayAnim() const { return PlayAnim; }
	void SetPlayAnim(const Ref<Animation> val) { PlayAnim = val; }

	Ref<AudioStream> PlaySound;
	Ref<AudioStream> GetPlaySound() const { return PlaySound; }
	void SetPlaySound(const Ref<AudioStream> val) { PlaySound = val; }

private:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("set_animation", "animation"), &InteractableOneshot::SetPlayAnim);
		ClassDB::bind_method("get_animation", &InteractableOneshot::GetPlayAnim);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_animation", "get_animation");

		ClassDB::bind_method(D_METHOD("set_play_sound", "sound"), &InteractableOneshot::SetPlaySound);
		ClassDB::bind_method("get_play_sound", &InteractableOneshot::GetPlaySound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_play_sound", "get_play_sound");
	}
};
