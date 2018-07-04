// Copyright 2018 odplot productions LLC. All rights reserved.

#include "interactable_oneshot.h"

void InteractableOneshot::ExecInteract(bool SilentExec) {
	if (!Nulled)
		ApplyAnimAndAudio(GetPlayAnim(), GetPlaySound());
}

void InteractableOneshot::ShortCircuitInteract() {
	ApplyAnimAndAudio(GetPlayAnim(), GetPlaySound());
}
