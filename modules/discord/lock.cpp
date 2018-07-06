// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "lock.h"

//void UInteractableLockComponent::BeginPlay() {
//	// When the game starts, let everyone know its initial state
//	if (Locked)
//		OnLock.Broadcast();
//	else
//		OnUnlock.Broadcast();
//}

//bool UInteractableLockComponent::TryUnlock(TSubclassOf<AInventoryItem> TryKey) {
//	if (Locked) {
//		if (TryKey == RequiredKey)
//			Unlock(false);
//		else
//			FailUnlock();
//	}
//
//	return TryKey == RequiredKey;
//}

void Lock::Unlock(bool Silent) {
	//if (!Silent && UnlockSound)
		//UGameplayStatics::PlaySoundAtLocation(this, UnlockSound, this->GetComponentLocation());

	Locked = false;
	emit_signal("on_unlock");
}

void Lock::DoLock(bool Silent) {
	//if (!Silent && LockSound)
		//UGameplayStatics::PlaySoundAtLocation(this, LockSound, this->GetComponentLocation());

	Locked = true;
	emit_signal("on_lock");
}

void Lock::FailUnlock() {
	// It doesn't make sense to call this silently, so the audio always plays
	//if (FailUnlockSound)
		//UGameplayStatics::PlaySoundAtLocation(this, FailUnlockSound, this->GetComponentLocation());

	emit_signal("on_fail_unlock");
}
