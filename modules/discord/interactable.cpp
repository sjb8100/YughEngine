// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "core/math/math_funcs.h"
#include "interactable.h"

// Sets default values for this component's properties
Interactable::Interactable() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	this->InteractDistance = 150;

	IsHolding = false;
	IsTapping = false;

	FinishedAnimation = true;
	FinishedAudio = true;

	CanInteract = true;

	Nulled = false;

	SetRevSpeed(1);
	RevDeg = 1;
}

void Interactable::_notification(int p_what) {

	switch (p_what) {

		case NOTIFICATION_PROCESS:

			float DeltaTime = get_process_delta_time();

			switch (InteractType) {

				case InteractMethod::INTERACT_METHOD_HOLD:

					if (IsHolding) {
						HoldTime = HoldTime + DeltaTime * RevSpeed;

						if (HoldTime >= TimeToInteract) {
							ExecInteract(false);

							if (TimeToInteract > 0)
								IsHolding = false;

							HoldTime = 0;
						}
					} else {
						HoldTime = HoldTime - DeltaTime * RevDeg;

						// TODO: Anything wrong with this clamp?
						HoldTime = CLAMP(HoldTime, 0.f, TimeToInteract);
					}

					// Update the SKMesh animation position
					if (AnimPlayer)
						AnimPlayer->seek(HoldTime / TimeToInteract, true);

					break;

				case InteractMethod::INTERACT_METHOD_TAP:

					TapAmount = TapAmount - TapDeg * DeltaTime;

					if (TapAmount <= 0) {
						IsTapping = false;
					}

					break;
			}

			break;

	}
}

//void Interactable::FireInteraction_Implementation()
//{
//	if (bCanPlay)
//	{
//		OnStartInteract.Broadcast();
//
//		switch (InteractType)
//		{
//		case EInteractType::Normal:
//			ExecuteInteract();
//
//			break;
//		case EInteractType::Hold:
//			bIsHolding = true;
//
//			break;
//		case EInteractType::Tap:
//			if (bIsTapping)
//			{
//				TapAmount += TapAdd;
//
//				if (TapAmount > TapCap)
//				{
//					ExecuteInteract();
//
//					bIsTapping = false;
//				}
//			}
//			else
//			{
//				bIsTapping = true;
//				TapAmount = TapStart;
//			}
//
//			break;
//		}
//
//
//	}
//	else
//		OnInteractedCantPlay.Broadcast();
//}

//void Interactable::ExecuteInteract_Implementation()
//{
//	TripFlags();
//
//	OnInteract.Broadcast();
//}

//void Interactable::ShortCircuitInteract_Implementation()
//{
//	TripFlags();
//}

bool Interactable::Playable() {
	switch (WaitType) {
		case InteractWait::INTERACT_WAIT_NEITHER:
			return true;

		case InteractWait::INTERACT_WAIT_ANIMATION_ONLY:
			if (this->FinishedAnimation)
				return true;

		case InteractWait::INTERACT_WAIT_AUDIO_ONLY:
			if (this->FinishedAudio)
				return true;

		case InteractWait::INTERACT_WAIT_BOTH:
			if (this->FinishedAudio && this->FinishedAnimation)
				return true;
	}

	return false;
}

void Interactable::ResetFlags() {
	FinishedAudio = true;
	FinishedAnimation = true;
	CanInteract = true;
}

void Interactable::TripFlags() {
	FinishedAnimation = false;
	FinishedAudio = false;

	if (WaitType != InteractWait::INTERACT_WAIT_NEITHER)
		CanInteract = false;
}

void Interactable::FinishAudio() {
	FinishedAudio = true;

	if (!CanInteract) {
		CanInteract = Playable();

		if (CanInteract)
			emit_signal("on_finished_wait");
	}
}

void Interactable::FinishAnimation() {
	FinishedAnimation = true;

	if (!CanInteract) {
		CanInteract = Playable();

		if (CanInteract)
			emit_signal("on_finished_wait");
	}
}

void Interactable::PreInteract(Node *Interactor) {
	CurrentInteractor = Interactor;

	Nulled = false;

	emit_signal("stop_interaction");

	if (Nulled)
		emit_signal("nulled_interact");
	else
		emit_signal("succeeded_interact");
}

void Interactable::ExecInteract(bool SilentExec) {
	Silent = SilentExec;

	if (!Silent)
		TripFlags();

	emit_signal("interact");
}

void Interactable::ShortCircuitInteract() {
}

void Interactable::FinishInteract() {
	if (Nulled) {
		if (AnimPlayer)
			AnimPlayer->stop();

		//if (NulledSound)
		//UGameplayStatics::SpawnSoundAtLocation(this, NulledSound, this->GetComponentLocation());
	} else
		ExecInteract(false);
}

bool Interactable::GetCanInteract() {
	emit_signal("check_can_play");

	return (CanPlay && CanInteract);
}

bool Interactable::LoadCanPlay() {
	emit_signal("check_can_play");

	return CanPlay;
}

void Interactable::LoadIsNulled() {
	emit_signal("nulled_interact");
}

void Interactable::ApplyAnimAndAudio(Ref<Animation> NewAnim, Ref<AudioStream> NewSound) {
	// Check for audio and play

	if (AnimPlayer && NewAnim.is_valid()) {
		AnimPlayer->add_animation("interacted", NewAnim);
		AnimPlayer->play("interacted");
		// Bind FinishAnimation to AnimPlayer end signal
		AnimPlayer->connect("animation_finished", this, "finish_animation");
	}

	if (NewSound.is_valid()) {
		// TODO: Creates a sound and calls FinishAudio when it's complete
		//UGameplayStatics::SpawnSoundAtLocation(this, NewSound, this->GetComponentLocation())->OnAudioFinished.AddDynamic(this, &Interactable::FinishAudio);
	}
}
