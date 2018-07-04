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

	Ref<AudioStream> PlaySound;

private:
};
