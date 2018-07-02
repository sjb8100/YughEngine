#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "network.h"
#include "steam.h"
#include "discord.h"

static Network *NetworkPtr = nullptr;

// Provided to access these in particular if needed
static Steam *SteamPtr = nullptr;
static Discord *DiscordPtr = nullptr;


void register_network_types() {
	// Inculde new CPP files here to "register" them with ClassDB::register_class<CLASS>();
	// Check here http://docs.godotengine.org/en/stable/development/cpp/custom_modules_in_cpp.html

	ClassDB::register_class<Network>();
	NetworkPtr = memnew(Network);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Network", Network::get_singleton()));

	ClassDB::register_class<Steam>();
	SteamPtr = memnew(Steam);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Steam", Steam::get_singleton()));

	ClassDB::register_class<Discord>();
	DiscordPtr = memnew(Discord);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Discord", Discord::get_singleton()));
}

void unregister_network_types(){

}
