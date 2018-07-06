#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "timekeeper.h"
#include "interactable.h"
#include "interactable_toggle.h"
#include "interactable_oneshot.h"
#include "lock.h"
#include "save_worker.h"

static USaveWorker *SaveWorkerPtr = nullptr;

void register_discord_types() {
	// Inculde new CPP files here to "register" them with ClassDB::register_class<CLASS>();
	// Check here http://docs.godotengine.org/en/stable/development/cpp/custom_modules_in_cpp.html

	ClassDB::register_class<TimeKeeper>();
	ClassDB::register_class<Interactable>();
	ClassDB::register_class<InteractableOneshot>();
	ClassDB::register_class<InteractableToggle>();
	ClassDB::register_class<Lock>();

	SaveWorkerPtr = memnew(USaveWorker);
	Engine::get_singleton()->add_singleton(Engine::Singleton("SaveWorker", USaveWorker::get_singleton()));
}

void unregister_discord_types(){

}
