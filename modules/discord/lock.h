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

	// Attempt to unlock the door with a given key
	// @param TryKey - The key to try and check against RequiredKey
	// @return - True if the door was unlocked, false if it was an unlock fail
	//bool TryUnlock(TSubclassOf<AInventoryItem> TryKey);

	// Call to unlock the door.
	// @param Silent - True if the sound shouldn't play
	void Unlock(bool Silent);

	// Call to lock the door.
	// @param Silent - True if the sound shouldn't play
	void DoLock(bool Silent);

	// Call to fail unlock the door
	void FailUnlock();

	// Played when it is locked. A key turning or something.
	Ref<AudioStream> LockSound;

	// Played when it is unlocked. Keys jingling or something.
	Ref<AudioStream> UnlockSound;

	// Played when it is failed to be unlocked. A shoulder shoving against the door or something.
	Ref<AudioStream> FailUnlockSound;

protected:
	static void _bind_methods() {
		ADD_SIGNAL(MethodInfo("on_fail_unlock")); // Broadcast when the door fails to unlock
		ADD_SIGNAL(MethodInfo("on_unlock")); // Broadcast when the door successfuly unlocks
		ADD_SIGNAL(MethodInfo("on_lock")); 	// Broadcast when the door locks
	}
};
