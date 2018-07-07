// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"

// TODO: Incorporate the C++20 time library into this
//#include <chrono>
//#include "date/date.h"

// Attach to an actor to let it keep track of time.
class TimeKeeper : public Node {
	GDCLASS(TimeKeeper, Node);

public:
	const int sec_in_min = 60;
	const int min_in_hour = 60;
	const int hour_in_day = 24;

	TimeKeeper() {
		sec = 0;
		min = 0;
		hour = 8;
		day = 18;
		month = 8;
		year = 1973;

		TimeRate = 1.0f;
		AdvancingTime = true;
	};

	~TimeKeeper(){};

	void _notification(int p_what);

	float sec;
	int min;
	int hour;
	int day;
	int month;
	int year;

	/// At what rate do we multiply real time passage by
	float TimeRate;
	float GetTimeRate() const { return TimeRate; }
	void SetTimeRate(float val) { TimeRate = val; }

	/// True if we should count time up
	bool AdvancingTime;
	bool GetAdvancingTime() const { return AdvancingTime; }
	void SetAdvancingTime(bool val) { AdvancingTime = val; }

	void normalize();

	void AddTime(Dictionary AddTime);

	void SetTime(Dictionary Time);

	Dictionary GetTime() {
		Dictionary time;

		time["sec"] = sec;
		time["min"] = min;
		time["hour"] = hour;
		time["day"] = day;
		time["month"] = month;
		time["year"] = year;

		return time;
	}

	bool IsMorning() {
		return hour < 12;
	}

	float GetMinuteHandRotation() {
		return ((float)min / min_in_hour) * 360.f;
	}

	float GetHourHandRotation() {
		return ((float)hour / hour_in_day) * 360.f;
	}

	bool IsAfterTime(int _hour) {
		return _hour > hour;
	}

	int GetHour(bool Is12Hour) {
		if (hour == 0 || hour == 12)
			return 12;

		if (!Is12Hour)
			return hour;
		else
			return hour % 12;
	}

	String GetFormattedTime(bool Is12Hour) {
		return String(Variant(GetHour(Is12Hour))) + ":" + String(Variant(min));
	}

protected:
	static void _bind_methods() {

		ClassDB::bind_method("get_time_rate", &TimeKeeper::GetTimeRate);
		ClassDB::bind_method(D_METHOD("set_time_rate", "time_rate"), &TimeKeeper::SetTimeRate);
		ADD_PROPERTY(PropertyInfo(Variant::REAL, "time_rate"), "set_time_rate", "get_time_rate");

		ClassDB::bind_method("get_advancing_time", &TimeKeeper::GetAdvancingTime);
		ClassDB::bind_method(D_METHOD("set_advancing_time", "advancing_time"), &TimeKeeper::SetAdvancingTime);
		ADD_PROPERTY(PropertyInfo(Variant::BOOL, "advancing_time"), "set_advancing_time", "get_advancing_time");

		ClassDB::bind_method("get_time", &TimeKeeper::GetTime);
		ClassDB::bind_method("get_minute_hand_rotation", &TimeKeeper::GetMinuteHandRotation);
		ClassDB::bind_method("get_hour_hand_rotation", &TimeKeeper::GetHourHandRotation);
		ClassDB::bind_method(D_METHOD("get_hour", "12_hour"), &TimeKeeper::GetHour);
		ClassDB::bind_method(D_METHOD("get_digital_formatted", "12_hour"), &TimeKeeper::GetHour);
		ClassDB::bind_method("is_morning", &TimeKeeper::IsMorning);

		// Signals useful to keep the UI updated
		ADD_SIGNAL(MethodInfo("minute_changed"));
		ADD_SIGNAL(MethodInfo("hour_changed"));
	}
};
