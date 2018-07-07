// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "timekeeper.h"

void TimeKeeper::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_PROCESS:
			
			if (AdvancingTime) {
				sec += get_process_delta_time() * TimeRate;
				normalize();
			}

			break;
	}
}

inline void TimeKeeper::normalize() {
	if (sec > sec_in_min) {
		min++;
		sec -= sec_in_min;
		emit_signal("minute_changed");

		if (min >= min_in_hour) {
			hour++;
			min -= min_in_hour;
			emit_signal("hour_changed");

			if (hour >= hour_in_day) {
				day++;
				hour -= hour_in_day;
			}
		}
	}
}

inline void TimeKeeper::AddTime(Dictionary AddTime) {
	sec += (float)AddTime["sec"];
	min += (int)AddTime["min"];
	hour += (int)AddTime["hour"];
	day += (int)AddTime["day"];
	month += (int)AddTime["month"];
	year += (int)AddTime["year"];

	normalize();
}

inline void TimeKeeper::SetTime(Dictionary Time) {
	sec = Time["sec"];
	min = Time["min"];
	hour = Time["hour"];
	day = Time["day"];
	month = Time["month"];
	year = Time["year"];

	normalize();
}
