#include "register_types.h"
#include "class_db.h"
#include "engine.h"
#include "analytics.h"

static Analytics *AnalyticsPtr = nullptr;

void register_analytics_types() {
	// Inculde new CPP files here to "register" them with ClassDB::register_class<CLASS>();
	// Check here http://docs.godotengine.org/en/stable/development/cpp/custom_modules_in_cpp.html

	ClassDB::register_class<Analytics>();
	AnalyticsPtr = memnew(Analytics);
	Engine::get_singleton()->add_singleton(Engine::Singleton("Analytics", Analytics::get_singleton()));
}

void unregister_analytics_types(){

}
