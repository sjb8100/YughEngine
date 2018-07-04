// Copyright 2018 odplot productions LLC. All rights reserved.

#include "interactable_toggle.h"

//void UInteractableToggleComponent::BeginPlay() {
//	Super::BeginPlay();
//
//	SetupAnimationState();
//}

void InteractableToggle::SetupAnimationState() {
	if (AnimPlayer) {
		AnimPlayer->add_animation("toggle_animation", AnimToggle);
		AnimPlayer->play("toggle_animation");
		AnimPlayer->stop();
	}
}

void InteractableToggle::PauseAnimation() {
	AnimPlayer->stop(false);
}

void InteractableToggle::ResumeAnimation() {
	AnimPlayer->play();
}

void InteractableToggle::ShortCircuitToggle(bool Toggle) {
	Toggled = !Toggle;

	ExecInteract(false);
}

void InteractableToggle::ExecInteract(bool SilentExec) {
	// Set the opposite toggle
	Toggled = !Toggled;

	// Broadcast a toggle interact


	if (Silent)
		SetupAnimationState();
	else {
		// Handle audio

		if (AnimPlayer) {
			Ref<Animation> UseMontage = Toggled || ForwardBackAnim ? AnimUntoggle : AnimToggle;

			AnimPlayer->add_animation("toggle_interact", AnimToggle);
			float PlayRate = !Toggled && ForwardBackAnim ? -1 : 1;
			AnimPlayer->play("toggle_interact", -1, PlayRate, PlayRate == -1);
			AnimPlayer->connect("animation_finished", this, "finish_animation");
			//SKMesh->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UInteractableComponent::FinishAnimation);
		}
	}
}
