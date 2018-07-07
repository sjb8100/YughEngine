// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "timekeeper.h"

//// Sets default values for this component's properties
//UTimeKeeperComponent::UTimeKeeperComponent() {
//	TimeRate = 1.0f;
//	AdvancingTime = true;
//}
//
//void UTimeKeeperComponent::SetAdvancingTime(bool advancing) {
//	AdvancingTime = advancing;
//}
//
//void UTimeKeeperComponent::SetTimeRate(float newRate) {
//	TimeRate = newRate;
//}
//
//float UTimeKeeperComponent::GetMinuteHandRotation() {
//	return CurrentTime.GetMinuteHandRotation();
//}
//
//float UTimeKeeperComponent::GetHourHandRotation(bool Is12Hour) {
//	return CurrentTime.GetHourHandRotation(Is12Hour);
//}
//
//bool UTimeKeeperComponent::IsAtleastTime(FTimespan CheckTime) {
//	return CurrentTime.IsAfterTime(CheckTime);
//}
//
//void UTimeKeeperComponent::AddTime(FTimespan AddTime) {
//	CurrentTime.AddTime(AddTime);
//}
//
//FString UTimeKeeperComponent::GetDigitalFormattedTime(bool Is12Hour) {
//	FString MinuteString;
//
//	if (CurrentTime.TrackedTime.GetMinutes() < 10)
//		MinuteString = FString(TEXT("0")) + FString::FromInt(CurrentTime.TrackedTime.GetMinutes());
//	else
//		MinuteString = FString::FromInt(CurrentTime.TrackedTime.GetMinutes());
//
//	FString HourString;
//
//	if (CurrentTime.GetHour(Is12Hour) < 10)
//		HourString = FString(TEXT(" ")) + FString::FromInt(CurrentTime.GetHour(Is12Hour));
//	else
//		HourString = FString::FromInt(CurrentTime.GetHour(Is12Hour));
//
//	return HourString + FString(TEXT(":")) + MinuteString;
//}
//
//bool UTimeKeeperComponent::IsMorning() {
//	if (CurrentTime.TrackedTime.GetHours() < 12)
//		return true;
//	else
//		return false;
//}
//
//float UTimeKeeperComponent::GetCurrentHour(bool Truncated) {
//	if (Truncated)
//		return CurrentTime.TrackedTime.GetHours();
//	else
//		return CurrentTime.TrackedTime.GetHours() + (float)CurrentTime.TrackedTime.GetMinutes() / CurrentTime.TimeScales.MinutesInHour;
//}

void TimeKeeper::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_PROCESS:

			if (AdvancingTime)
				d += seconds(5);
				//CurrentTime.AdvanceTime(DeltaTime * TimeRate);

			break;
	}
}

void TimeKeeper::set_tracked_seconds(int p_seconds) {
	tracked_seconds = p_seconds;
}

int TimeKeeper::get_tracked_seconds() const {
	return tracked_seconds;
}
