// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#include "thirdpersoncamera.h"
#include "core/math/math_funcs.h"

thirdpersoncamera::thirdpersoncamera() {

	set_process(true);

	// Rotation
	RotationSpeed = 10.0f;
	LockPitch = LockYaw = LockRoll = true;

	// Translation
	FloatWidths = AnchorWidths = CenterVector = Vector3(0, 0, 0);
	PositionSpeeds = Vector3(2, 2, 2);
	TranslationScales = Vector3(1, 1, 1);

	// Frame settings
	Offset = Vector3(0, 0, 0);
	Distance = 300;

	AnchorSpeed = 80;
}

// TODO: Figure out what this is supposed to actually be to tick
void thirdpersoncamera::Tick(float DeltaTime) {
	TargetPosition = CalculatePosition();

	// I don't see the use of centervector here. Only used here ... as well as translationscales
	// TODO: Commenting out for now.
	//Vector3 ToPosition = CenterVector + ((TargetPosition - CenterVector) * TranslationScales);

	// For translation
	this->set_translation(FrameBasedVectorLerp(this->get_translation(), TargetPosition, PositionSpeeds, DeltaTime));

	// For rotation
	// TODO: Add rotation offset in properly
	//this->SetActorRotation(FMath::Lerp(this->GetActorRotation(), TargetRotation, RotationSpeed * DeltaTime));
	//this->set_rotation(Quat(this->get_rotation().slerp(RotationOffset, RotationSpeed * DeltaTime)).get_euler());
	this->set_rotation(this->get_rotation().slerp(RotationOffset.get_euler(), RotationSpeed * DeltaTime));
}

void thirdpersoncamera::CalculateTargetOffset() {
	Spatial *Frame = GetFrameOfReference();

	// Gets the offset of the camera
	// TODO: Blueprint code uses InverseTransformDirection. I think InverseTransformVectorNoScale does the same ...
	//Vector3 Start = ExternalFrame->GetActorTransform().InverseTransformVectorNoScale(Target->GetActorLocation());
	Vector3 Start = ExternalFrame->get_global_transform().xform_inv(Target->get_translation());

	Vector3 Offset = this->get_translation() + this->get_global_transform().xform(Vector3(1, 0, 0)) * Distance + ExternalFrame->get_global_transform().xform(Offset);

	//TargetOffset = Start - ExternalFrame->GetActorTransform().InverseTransformVectorNoScale(Offset);
	TargetOffset = Start - ExternalFrame->get_global_transform().xform_inv(Offset);
}

Spatial *thirdpersoncamera::GetFrameOfReference() {
	if (ExternalFrame)
		return ExternalFrame;

	return this;
}

void thirdpersoncamera::CalculateTargets() {
	// For translation
	TargetPosition = CalculatePosition();

	// For rotation
	// TODO: Check of this implementation is the same as UKismetMath FindLookAtRotation
	TargetRotation = RemoveLockedRotation(Quat(this->get_global_transform().get_origin() - Target->get_global_transform().get_origin()));
}

Vector3 thirdpersoncamera::CalculatePosition() {
	Vector3 p1 = CalculateCenter();
	Spatial *FOR = GetFrameOfReference();

	Vector3 p2 = XDirPosts ? GetPostsOffset(FOR->get_global_transform().xform(Vector3(1, 0, 0)), FloatWidths.x) : GetExtentsOffset(FOR->get_global_transform().xform(Vector3(1, 0, 0)), FloatWidths.x, TargetOffset.x, AnchorWidths.x);
	Vector3 p3 = YDirPosts ? GetPostsOffset(FOR->get_global_transform().xform(Vector3(0, 0, 1)), FloatWidths.y) : GetExtentsOffset(FOR->get_global_transform().xform(Vector3(0, 0, 1)), FloatWidths.y, TargetOffset.y, AnchorWidths.y);
	Vector3 p4 = ZDirPosts ? GetPostsOffset(FOR->get_global_transform().xform(Vector3(0, 1, 0)), FloatWidths.z) : GetExtentsOffset(FOR->get_global_transform().xform(Vector3(1, 1, 0)), FloatWidths.z, TargetOffset.z, AnchorWidths.z);

	return p1 + p2 + p3 + p4;
}

Vector3 thirdpersoncamera::CalculateCenter() {
	return (Target->get_translation() - GetFrameOfReference()->get_global_transform().xform(Offset)) - this->get_global_transform().xform(Vector3(1, 0, 0)) * Distance;
}

Vector3 thirdpersoncamera::GetPostsOffset(const Vector3 &DirectionVector, float AnchorWidth) {
	float dot = Target->get_global_transform().xform(Vector3(1, 0, 0)).dot(DirectionVector);

	// Return the direction ...
	return DirectionVector * (dot ? AnchorWidth : AnchorWidth * -1);
}

Vector3 thirdpersoncamera::GetExtentsOffset(const Vector3 &DirectionVector, float AnchorWidth, float TOffset, float Width) {
	Vector3 r1;
	Vector3 r2;

	bool offsetPositive = TOffset >= 0;

	Vector3 p1 = DirectionVector * AnchorWidth * (offsetPositive ? -1 : 1);
	Vector3 p2 = DirectionVector * TOffset * -1;

	bool b2 = Math::abs(TOffset) >= AnchorWidth;

	r1 = b2 ? p1 : p2;

	float TotalWidth = AnchorWidth + Width;

	bool b3 = Math::abs(TOffset) > TotalWidth && Math::is_equal_approx(Math::abs(TOffset), TotalWidth);

	Vector3 p3 = DirectionVector * TotalWidth * (offsetPositive ? -1 : 1);

	r2 = b3 ? p3 : r1;

	return r2;
}

Quat thirdpersoncamera::RemoveLockedRotation(const Quat &CurrentRotation) {
	Vector3 NewRotator;
	Vector3 CurrentRotator = CurrentRotation.get_euler();
	//
	NewRotator.x = LockRoll ? this->get_rotation().x : CurrentRotator.x;
	NewRotator.y = LockPitch ? this->get_rotation().y : CurrentRotator.y;
	NewRotator.z = LockYaw ? this->get_rotation().z : CurrentRotator.z;

	return Quat(NewRotator);
}

Vector3 thirdpersoncamera::FrameBasedVectorLerp(const Vector3 &From, const Vector3 &To, const Vector3 &Speeds, float Tick) {
	Vector3 LerpedVector;

	Transform FORTransform = GetFrameOfReference()->get_global_transform();

	// Previously "FORTransform.TransformVector(Speeds)
	Vector3 TSpeed = FORTransform.xform(Speeds).abs();
	Vector3 TOffset = FORTransform.xform(TargetOffset).abs();
	Vector3 TAnchorWidths = FORTransform.xform(AnchorWidths).abs();
	Vector3 TFloatWidths = FORTransform.xform(FloatWidths).abs();

	bool bUseX = GetLerpParam(TOffset.x, TAnchorWidths.x, TFloatWidths.x);
	bool bUseY = GetLerpParam(TOffset.y, TAnchorWidths.y, TFloatWidths.y);
	bool bUseZ = GetLerpParam(TOffset.z, TAnchorWidths.z, TFloatWidths.z);

	float xAlpha = bUseX ? TSpeed.x * AnchorSpeed : AnchorSpeed * Tick;
	float yAlpha = bUseY ? TSpeed.y * AnchorSpeed : AnchorSpeed * Tick;
	float zAlpha = bUseY ? TSpeed.z * AnchorSpeed : AnchorSpeed * Tick;

	return LerpVector(From, To, Vector3(xAlpha, yAlpha, zAlpha));
}

Vector3 thirdpersoncamera::LerpVector(const Vector3 &From, const Vector3 &To, const Vector3 &Alpha) {
	return Vector3(Math::lerp(From.x, To.x, Alpha.x), Math::lerp(From.y, To.y, Alpha.y), Math::lerp(From.z, To.z, Alpha.z));
}

bool thirdpersoncamera::GetLerpParam(const float Offst, const float AnchorWidth, const float FloatWidth) {
	return (Offst > (AnchorWidth + FloatWidth)) && !Math::is_equal_approx(AnchorWidth + FloatWidth, Offst);
}

void thirdpersoncamera::_bind_methods() {
	BIND_ENUM_CONSTANT(STATIONARY);
	BIND_ENUM_CONSTANT(TRANSLATING);
	BIND_ENUM_CONSTANT(ROTATING);
	BIND_ENUM_CONSTANT(SPLINE);

	BIND_ENUM_CONSTANT(NONE);
	BIND_ENUM_CONSTANT(CROSSDISSOLVE);
	BIND_ENUM_CONSTANT(WIPE);
	BIND_ENUM_CONSTANT(DIP);

	BIND_ENUM_CONSTANT(LOCAL);
	BIND_ENUM_CONSTANT(WORLD);
	BIND_ENUM_CONSTANT(EXTERNAL);
}
