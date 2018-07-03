// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"

// TODO: Not integrating boost libraries right now to keep it simple, but should later
//#include "boost/date_time/gregorian/gregorian_types.hpp"

// Attach to an actor to let it keep track of time.
class Damageable : public Node {
	GDCLASS(Damageable, Node);

public:
	Damageable() {
		tracked_seconds = 0;
		seconds_in_minute = 60;
		minutes_in_hour = 60;
		hours_in_day = 24;
	};

	~Damageable(){};

	int seconds_in_minute;

	int minutes_in_hour;

	int hours_in_day;

	int current_year;

	int current_month;

	int current_day;

			//virtual _process(float delta) override;

			/*void RectifyTime() {
		if (TrackedTime.GetSeconds() > TimeScales.SecondsInMinute) {
			TrackedTime += FTimespan().FromMinutes(1);

			TrackedTime -= FTimespan().FromSeconds(TimeScales.SecondsInMinute);

			if (TrackedTime.GetMinutes() >= TimeScales.MinutesInHour) {
				TrackedTime += FTimespan().FromHours(1);

				TrackedTime -= FTimespan().FromMinutes(TimeScales.MinutesInHour);

				if (TrackedTime.GetHours() >= TimeScales.HoursInDay) {
					TrackedTime += FTimespan().FromDays(1);

					TrackedTime -= FTimespan().FromHours(TimeScales.HoursInDay);
				}
			}
		}
	}

	void GetSeconds() {

	}

	float GetMinuteHandRotation() {
		return ((float)TrackedTime.GetMinutes() / TimeScales.MinutesInHour) * 360.f;
	}

	float GetHourHandRotation(bool Is12Hour) {
		if (Is12Hour)
			return (TrackedTime.GetHours() / (TimeScales.HoursInDay / 2)) * 360.f;
		else
			return (TrackedTime.GetHours() / TimeScales.HoursInDay) * 360.f;
	}

	bool IsAfterTime(FTimespan CheckTime) {
		return TrackedTime > CheckTime;
	}

	int GetHour(bool Is12Hour) {
		if (TrackedTime.GetHours() == 0 || TrackedTime.GetHours() == 12)
			return 12;

		if (!Is12Hour)
			return TrackedTime.GetHours();
		else
			return TrackedTime.GetHours() % 12;
	}*/

			//FString GetFormattedTime() {
			//	return FString::FromInt(GetHour(true)) + TEXT(":") + FString::FromInt(TrackedTime.GetMinutes());
			//}
			//// Sets default values for this component's properties
			//UTimeKeeperComponent();

			//// Called when the game starts
			//virtual void BeginPlay() override;

			//// Called every frame
			//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

			//// Defined variables
			//bool AdvancingTime; // True if we should count time up

			//float TimeRate; // At what rate do we multiply real time passage by

			//FGameTime CurrentTime;

			//void AddTime(FTimespan AddTime);

			//void SetAdvancingTime(bool advancing);

			//void SetTimeRate(float newRate);

			//float GetMinuteHandRotation();

			//float GetHourHandRotation(bool Is12Hour);

			//bool IsAtleastTime(FTimespan CheckTime);

			//FString GetDigitalFormattedTime(bool Is12Hour);

			//bool IsMorning();

			//float GetCurrentHour(bool Truncated);

			void
			set_tracked_seconds(int p_seconds);
	int get_tracked_seconds() const;

protected:
	static void _bind_methods();

private:
	int tracked_seconds;
};

void Damageable::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_tracked_seconds", "seconds"), &Damageable::set_tracked_seconds);
	ClassDB::bind_method(D_METHOD("get_tracked_seconds"), &Damageable::get_tracked_seconds);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "tracked_seconds"), "set_tracked_seconds", "get_tracked_seconds");
}

void Damageable::set_tracked_seconds(int p_seconds) {
	tracked_seconds = p_seconds;
}

int Damageable::get_tracked_seconds() const {
	return tracked_seconds;
}
