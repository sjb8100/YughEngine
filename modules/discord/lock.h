// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"
#include "servers/audio/audio_stream.h"

// Simulates a lock. Can be locked or unlocked if the correct key is applied, and
// plays sounds when these events happen.
class Lock : public Node {
	GDCLASS(Lock, Node)

public:
	// The required key that must be used to unlock the door
	//TSubclassOf<AInventoryItem> RequiredKey;

	// True if this is considered "locked"
	bool Locked;
	void SetLocked(bool p_Locked) { Locked = p_Locked; }
	bool GetLocked() { return Locked; }

	// Attempt to unlock the door with a given key
	// @param TryKey - The key to try and check against RequiredKey
	// @return - True if the door was unlocked, false if it was an unlock fail
	//bool TryUnlock(TSubclassOf<AInventoryItem> TryKey);

	// Call to unlock the door.
	// @param Silent - True if the sound shouldn't play
	void Unlock(bool Silent = false);

	// Call to lock the door.
	// @param Silent - True if the sound shouldn't play
	void DoLock(bool Silent = false);

	// Call to fail unlock the door
	void FailUnlock();

	// Played when it is locked. A key turning or something.
	Ref<AudioStream> LockSound;
	void SetLockSound(const Ref<AudioStream> p_LockSound) { LockSound = p_LockSound; }
	Ref<AudioStream> GetLockSound() { return LockSound; }

	// Played when it is unlocked. Keys jingling or something.
	Ref<AudioStream> UnlockSound;
	void SetUnlockSound(const Ref<AudioStream> p_UnlockSound) { UnlockSound = p_UnlockSound; }
	Ref<AudioStream> GetUnlockSound() { return UnlockSound; }

	// Played when it is failed to be unlocked. A shoulder shoving against the door or something.
	Ref<AudioStream> FailUnlockSound;
	void SetFailUnlockSound(const Ref<AudioStream> p_FailUnlockSound) { FailUnlockSound = p_FailUnlockSound; }
	Ref<AudioStream> GetFailUnlockSound() { return FailUnlockSound; }

protected:
	static void _bind_methods() {
		ClassDB::bind_method(D_METHOD("unlock", "silent"), &Lock::Unlock);
		ClassDB::bind_method(D_METHOD("lock", "silent"), &Lock::DoLock);

		ClassDB::bind_method(D_METHOD("set_lock_sound", "sound"), &Lock::SetLockSound);
		ClassDB::bind_method("get_lock_sound", &Lock::GetLockSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "lock_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_lock_sound", "get_lock_sound");

		ClassDB::bind_method(D_METHOD("set_unlock_sound", "sound"), &Lock::SetUnlockSound);
		ClassDB::bind_method("get_unlock_sound", &Lock::GetUnlockSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "unlock_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_unlock_sound", "get_unlock_sound");

		ClassDB::bind_method(D_METHOD("set_fail_unlock_sound", "sound"), &Lock::SetFailUnlockSound);
		ClassDB::bind_method("get_fail_unlock_sound", &Lock::GetFailUnlockSound);
		ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fail_unlock_sound", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_fail_unlock_sound", "get_fail_unlock_sound");

		ClassDB::bind_method(D_METHOD("set_locked", "locked"), &Lock::SetLocked);
		ClassDB::bind_method("get_locked", &Lock::GetLocked);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "locked"), "set_locked", "get_locked");

		ADD_SIGNAL(MethodInfo("on_fail_unlock")); // Broadcast when the door fails to unlock
		ADD_SIGNAL(MethodInfo("on_unlock")); // Broadcast when the door successfuly unlocks
		ADD_SIGNAL(MethodInfo("on_lock")); // Broadcast when the door locks
	}
};
