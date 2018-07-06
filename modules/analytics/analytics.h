#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"

//////////////////////////////////////////////////////////////////////////
// Analytics systems should inheret from this class and implement the events.
// Should implement them in such a way so as to keep data private.
// Ideal use is simply to see how your game is being played, to introduce
// bug fixes, enhance the game through patches, etc. Not for predatory
// targeting stuff.
// It's just not worth it. Make your game fun!
//////////////////////////////////////////////////////////////////////////

class AnalyticsSystem {

public:
	AnalyticsSystem() {}

	// The destroy function should close the system down
	~AnalyticsSystem() {}

	// Call to start a session
	virtual void StartSession() = 0;

	// Call to explicitely end a session. Some analytics may require this,
	// while others time out.
	virtual void EndSession() = 0;

	// Records a screen of your game. Use whenever a new level/mode/UI screen
	// is loaded.
	// @param Name - The name of the loaded screen
	virtual void RecordScreen(String Name) = 0;

	// Records an event in your game. Use to mark whenever a player
	// does something of interest - starts a level, loses a level, etc.
	// @param Name - The name of the event to record.
	virtual void RecordEvent(String Name) = 0;

	// Records an event with some extra parameters.
	// @param EventName - The name of the event to record.
	// @param AttributeName - The name of the event attribute.
	// @param AttributeValue - The value of the attribute.
	virtual void RecordEventWithAttribute(String EventName, String AttributeName, String AttributeValue) = 0;

	// Some analytics providers have a local cache that uploads the data
	// at a later time. This should clear the data in that cache.
	virtual void FlushCache() = 0;
};

//////////////////////////////////////////////////////////////////////////
// This class is called in GDScript and routes requests to the supplies
// AnalyticsSystem.
//////////////////////////////////////////////////////////////////////////

class Analytics : public Object {
	GDCLASS(Analytics, Object);

public:
	static Analytics *get_singleton() { return singleton; }

	Analytics() {
		singleton = this;
	}

	~Analytics() {
	}

	// A Vector of all of the registered analytics providers
	Vector<AnalyticsSystem *> Analytics;

	// Methods to run analytics
	// These simply call the equivalent methods in all registered AnalyticsSystems

	void StartSession() {
		/*for (AnalyticsSystem *Analytic : Analytics)
			Analytic->StartSession();*/
	}

	void EndSession() {
		/*for (AnalyticsSystem *Analytic : Analytics)
			Analytic->EndSession();*/
	}

	void RecordScreen(String Name) {
		/*for (AnalyticsSystem *Analytic : Analytics)
			Analytic->RecordScreen(Name);*/
	}

	void RecordEvent(String Name) {
		//for (AnalyticsSystem *Analytic : Analytics)
			//Analytic->RecordEvent(Name);
	}

	void RecordEventWithAttribute(String EventName, String AttributeName, String AttributeValue) {

	}

	void FlushCache() {

	}

protected:
	static Network *singleton;

	// Binding all methods and signals
	static void _bind_methods() {
		ClassDB::bind_method("StartSession", &Analytics::StartSession);
		ClassDB::bind_method("EndSession", &Analytics::EndSession);
		ClassDB::bind_method(D_METHOD("RecordEvent", "EventName"), &Analytics::RecordEvent);
	}
};
