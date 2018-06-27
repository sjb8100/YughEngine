#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "discord.h"

static Discord *DiscordPtr = nullptr;

void register_discord_types() {
	// Inculde new CPP files here to "register" them with ClassDB::register_class<CLASS>();
	// Check here http://docs.godotengine.org/en/stable/development/cpp/custom_modules_in_cpp.html

	ClassDB::register_class<Discord>();
	DiscordPtr = memnew(Discord);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Discord", Discord::get_singleton()));
}

void unregister_discord_types(){

}
