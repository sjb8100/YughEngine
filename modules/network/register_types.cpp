#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "network.h"

static Network *NetworkPtr = nullptr;

void register_network_types() {
	// Inculde new CPP files here to "register" them with ClassDB::register_class<CLASS>();
	// Check here http://docs.godotengine.org/en/stable/development/cpp/custom_modules_in_cpp.html

	ClassDB::register_class<Network>();
	NetworkPtr = memnew(Network);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Network", Network::get_singleton()));
}

void unregister_network_types(){

}
