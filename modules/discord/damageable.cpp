// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "damageable.h"

//// Sets default values for this component's properties
//UTimeKeeperComponent::UTimeKeeperComponent() {
//	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
//	// off to improve performance if you don't need them.
//	PrimaryComponentTick.bCanEverTick = true;
//
//	TimeRate = 1.0f;
//	AdvancingTime = true;
//}
//
//// Called when the game starts
//void UTimeKeeperComponent::BeginPlay() {
//	Super::BeginPlay();
//
//	// ...
//}
//
//// Called every frame
//void UTimeKeeperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) {
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	if (AdvancingTime) {
//		CurrentTime.AdvanceTime(DeltaTime * TimeRate);
//	}
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
