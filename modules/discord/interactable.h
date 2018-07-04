// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"
#include "scene/resources/texture.h"
#include "servers/audio/audio_stream.h"
#include "scene/resources/animation.h"
#include "scene/animation/animation_player.h"

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
		NEITHER,
		ANIMATION_ONLY,
		AUDIO_ONLY,
		BOTH
	};

	// Method for interacting
	enum InteractMethod {
		INSTANT, // When interacted with, it fires
		HOLD, // Must hold the interact button to fire it
		TAP // Must tap the button repeatedly to fire it
	};

	// Sets default values for this component's properties
	Interactable();
	~Interactable() {}

	void TickComponent(float DeltaTime);

	String Interaction;

	float InteractDistance = 1.5f;

	bool CanInteract = true;

	bool CanPlay;

	Ref<Texture> InteractIcon;

	float TimeToInteract = 3.f;

	bool FinishedAnimation;

	bool FinishedAudio;

	InteractWait WaitType;

	AnimationPlayer* AnimPlayer;

	InteractMethod InteractMethod;

	Node* CurrentInteractor;

	bool Silent;

	void ApplyAnimAndAudio(Ref<Animation> NewAnim, Ref<AudioStream> NewSound);

	//////////////////////////////////////////////////////////////////////////
	// Nulled variables
	//////////////////////////////////////////////////////////////////////////

	Ref<Animation> NulledAnim;

	Ref<AudioStream> NulledSound;

	//////////////////////////////////////////////////////////////////////////
	// Tapping variables
	//////////////////////////////////////////////////////////////////////////

	float TapAmount;

	float TapCap;

	float TapDeg;

	float TapStart;

	float TapAdd;

	//////////////////////////////////////////////////////////////////////////
	// Hold variables
	//////////////////////////////////////////////////////////////////////////

	bool IsHolding;

	float HoldTime;

	float RevSpeed;

	Ref<Animation> HoldMontage;

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
	void PreInteract(Node* Interactor);

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

		ADD_SIGNAL(MethodInfo("finished_wait"));
		ADD_SIGNAL(MethodInfo("stop_interaction"));
		ADD_SIGNAL(MethodInfo("nulled_interact"));
		ADD_SIGNAL(MethodInfo("on_something_began"));
		ADD_SIGNAL(MethodInfo("interact"));
		ADD_SIGNAL(MethodInfo("succeeded_interact"));
		ADD_SIGNAL(MethodInfo("check_can_play"));
		ADD_SIGNAL(MethodInfo("load_interact"));
	}

private:
};
