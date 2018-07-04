// Copyright 2018 odplot productions LLC. All rights reserved.

#include "interactable_oneshot.h"

void InteractableOneshot::ExecInteract(bool SilentExec) {
	if (!Nulled)
		ApplyAnimAndAudio(PlayAnim, PlaySound);
}

void InteractableOneshot::ShortCircuitInteract() {
	ApplyAnimAndAudio(PlayAnim, PlaySound);
}
