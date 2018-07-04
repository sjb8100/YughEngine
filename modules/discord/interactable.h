// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/animation/animation_player.h"
#include "scene/main/node.h"
#include "scene/resources/animation.h"
#include "scene/resources/texture.h"
#include "servers/audio/audio_stream.h"

// A set of things that dictate how an actor behaves when interacting with
// an object of a certain Tag.
//USTRUCT(BlueprintType)
//struct FInteractionResponse {
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Basic)
//	UAnimMontage *Animation;
//
//	// Audio
//};

//////////////////////////////////////////////////////////////////////////
// This component should be attached to any object that wants the ability
// to be interacted with. The object should implement the functions that it
// wants - OnInteract, OnStopInteract, and so on, to define behavior.
// Additionally, it supplies standard interaction stuff, like animations and
// audio cues.
//
// One InteractableComponent per action.
//////////////////////////////////////////////////////////////////////////

class Interactable : public Node {
	GDCLASS(Interactable, Node)

public:
	// The type of waiting for the interact to do
	enum InteractWait {
		INTERACT_WAIT_NEITHER,
		INTERACT_WAIT_ANIMATION_ONLY,
		INTERACT_WAIT_AUDIO_ONLY,
		INTERACT_WAIT_BOTH
	};

	// Method for interacting
	enum InteractMethod {
		INTERACT_METHOD_INSTANT, // When interacted with, it fires
		INTERACT_METHOD_HOLD, // Must hold the interact button to fire it
		INTERACT_METHOD_TAP // Must tap the button repeatedly to fire it
	};

	// Sets default values for this component's properties
	Interactable();
	~Interactable() {}

	void _notification(int p_what);

	String Interaction;
	String GetInteraction() const { return Interaction; }
	void SetInteraction(String val) { Interaction = val; }

	float InteractDistance = 1.5f;

	bool CanInteract = true;

	bool CanPlay;

	Ref<Texture> InteractIcon;
	Ref<Texture> GetInteractIcon() const { return InteractIcon; }
	void SetInteractIcon(Ref<Texture> val) { InteractIcon = val; }

	float TimeToInteract = 3.f;

	bool FinishedAnimation;

	bool FinishedAudio;

	InteractWait WaitType;
	InteractWait GetWaitType() const { return WaitType; }
	void SetWaitType(InteractWait val) { WaitType = val; }

	AnimationPlayer *AnimPlayer;

	InteractMethod InteractType;
	InteractMethod GetInteractMethod() const { return InteractType; }
	void SetInteractMethod(InteractMethod val) { InteractType = val; }

	Node *CurrentInteractor;

	bool Silent;

	void ApplyAnimAndAudio(Ref<Animation> NewAnim, Ref<AudioStream> NewSound);

	//////////////////////////////////////////////////////////////////////////
	// Nulled variables
	//////////////////////////////////////////////////////////////////////////

	Ref<Animation> NulledAnim;
	Ref<Animation> GetNulledAnim() const { return NulledAnim; }
	void SetNulledAnim(Ref<Animation> val) { NulledAnim = val; }

	Ref<AudioStream> NulledSound;
	Ref<AudioStream> GetNulledSound() const { return NulledSound; }
	void SetNulledSound(Ref<AudioStream> val) { NulledSound = val; }

	//////////////////////////////////////////////////////////////////////////
	// Tapping variables
	//////////////////////////////////////////////////////////////////////////

	// The current number of taps that have happened
	float TapAmount;

	// The number of taps that must be done to activate the interaction event
	float TapCap;
	float GetTapCap() const { return TapCap; }
	void SetTapCap(float val) { TapCap = val; }

	// The speed at which the tapped amount degrades per second
	float TapDeg;
	float GetTapDeg() const { return TapDeg; }
	void SetTapDeg(float val) { TapDeg = val; }

	float TapAdd;

	//////////////////////////////////////////////////////////////////////////
	// Hold variables
	//////////////////////////////////////////////////////////////////////////

	bool IsHolding;

	float HoldTime;
	float GetHoldTime() const { return HoldTime; }
	void SetHoldTime(float val) { HoldTime = val; }

	float RevSpeed;
	float GetRevSpeed() const { return RevSpeed; }
	void SetRevSpeed(float val) { RevSpeed = val; }

	Ref<Animation> HoldMontage;
	Ref<Animation> GetHoldMontage() const { return HoldMontage; }
	void SetHoldMontage(const Ref<Animation> val) { HoldMontage = val; }

	bool HoldAniming;

	bool HeldCanInteract;

	float RevDeg;

	bool IsTapping;

	// Functions

	bool Playable();

	void ResetFlags();

	void TripFlags();

	void FinishAudio();

	void FinishAnimation();

	// Called before the interaction goes
	void PreInteract(Node *Interactor);

	// Broadcasts the interact and sets triggers
	virtual void ExecInteract(bool SilentExec);

	// Call this to ignore all checks and just do the interaction
	virtual void ShortCircuitInteract();

	// Called when the whole interaction is done
	void FinishInteract();

	bool GetCanInteract();

	bool LoadCanPlay();

	void LoadIsNulled();

	bool Nulled;

	//void RegisterWaitForAudio(UAudioComponent* WaitSound);

protected:
	static void _bind_methods() {
		ClassDB::bind_method("finish_animation", &Interactable::FinishAnimation);
		ClassDB::bind_method("finish_audio", &Interactable::FinishAudio);
		ClassDB::bind_method("playable", &Interactable::Playable);
		ClassDB::bind_method("trip_play_flags", &Interactable::TripFlags);

		ADD_GROUP("Null", "");
		ClassDB::bind_method("get_nulled_animation", &Interactable::GetNulledAnim);
		ClassDB::bind_method(D_METHOD("set_nulled_animation", "animation"), &Interactable::SetNulledAnim);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "nulled_animation", PROPERTY_HINT_RESOURCE_TYPE, "Animation"), "set_nulled_animation", "get_nulled_animation");

		ClassDB::bind_method("get_nulled_sound", &Interactable::GetNulledSound);
		ClassDB::bind_method(D_METHOD("set_nulled_sound", "sound"), &Interactable::SetNulledSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "nulled_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_nulled_sound", "get_nulled_sound");

		ADD_GROUP("Tapping", "");
		ClassDB::bind_method("get_tap_cap", &Interactable::GetTapCap);
		ClassDB::bind_method(D_METHOD("set_tap_cap", "capacity"), &Interactable::SetTapCap);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "tap_cap"), "set_tap_cap", "get_tap_cap");

		ClassDB::bind_method("get_tap_deg", &Interactable::GetTapDeg);
		ClassDB::bind_method(D_METHOD("set_tap_deg", "degradation"), &Interactable::SetTapDeg);
		ADD_PROPERTY(PropertyInfo(Variant::INT, "tap_deg"), "set_tap_deg", "get_tap_deg");

		ADD_SIGNAL(MethodInfo("finished_wait"));
		ADD_SIGNAL(MethodInfo("stop_interaction"));
		ADD_SIGNAL(MethodInfo("nulled_interact"));
		ADD_SIGNAL(MethodInfo("on_something_began"));
		ADD_SIGNAL(MethodInfo("interact"));
		ADD_SIGNAL(MethodInfo("succeeded_interact"));
		ADD_SIGNAL(MethodInfo("check_can_play"));
		ADD_SIGNAL(MethodInfo("load_interact"));

		BIND_ENUM_CONSTANT(INTERACT_WAIT_NEITHER);
		BIND_ENUM_CONSTANT(INTERACT_WAIT_ANIMATION_ONLY);
		BIND_ENUM_CONSTANT(INTERACT_WAIT_AUDIO_ONLY);
		BIND_ENUM_CONSTANT(INTERACT_WAIT_BOTH);

		BIND_ENUM_CONSTANT(INTERACT_METHOD_INSTANT);
		BIND_ENUM_CONSTANT(INTERACT_METHOD_HOLD);
		BIND_ENUM_CONSTANT(INTERACT_METHOD_TAP);
	}

private:
};

VARIANT_ENUM_CAST(Interactable::InteractWait);
VARIANT_ENUM_CAST(Interactable::InteractMethod);
