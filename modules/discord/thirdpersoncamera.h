// odplot productions is a trademarked name. Project Yugh is a copyrighted property. This code, however, is free to be copy and extended as you see fit.

#pragma once

#include "dictionary.h"
#include "object.h"
#include "reference.h"
#include "scene/main/node.h"
#include "scene/3d/spatial.h"

// TODO: Not integrating boost libraries right now to keep it simple, but should later
//#include "boost/date_time/gregorian/gregorian_types.hpp"

// Attach to an actor to let it keep track of time.
class thirdpersoncamera : public Spatial {
	GDCLASS(thirdpersoncamera, Spatial);

public:

	enum CameraType {
		STATIONARY,
		TRANSLATING,
		ROTATING,
		SPLINE
	};

	enum CameraTransition {
		NONE,
		CROSSDISSOLVE,
		WIPE,
		DIP
	};

	enum FrameOfReference {
		LOCAL,
		WORLD,
		EXTERNAL
	};

	thirdpersoncamera();
	~thirdpersoncamera() { }

	void Tick(float DeltaSeconds);

	// An actor that can be given for the camera to base its movement around
	// instead of itself. Makes most sense for this to be stationary
	Spatial *ExternalFrame;

	// The target the camera "looks" at, used for calculations
	Spatial *Target;

	// Offset from the target
	Vector3 Offset;

	// How far away should the camera act from the target
	float Distance;

	///////////////////////////////////////////////////////////////////////////
	// Translation variables. In this mode, the camera doesn't rotate to look
	// at the target, it only moves around the world.
	///////////////////////////////////////////////////////////////////////////

	// "Posts" for each direction in 3D space. These are items that the target
	// is allowed to move within without the camera following along.
	bool XDirPosts;
	bool YDirPosts;
	bool ZDirPosts;

	// The range in ecah direction the camera floats. While within this range,
	// the camera will smoothly glide to the desired position.
	Vector3 FloatWidths;

	// The clamp range for each direction. If the camera reaches this range,
	// it will stick and not move any further.
	Vector3 AnchorWidths;

	// A scaling factor for translating the camera
	Vector3 TranslationScales;

	// Offset for where to make calculations from the center of the camera.
	Vector3 CenterVector;

	// When floating to the target, the speed to float.
	Vector3 PositionSpeeds;

	//////////////////////////////////////////////////////////////////////////
	// Rotation variables. Used for the camera's rotation mode, where it
	// follows the Target without translating.
	//////////////////////////////////////////////////////////////////////////

	// Variables to lock its rotation in any of the three directions
	bool LockRoll;
	bool LockPitch;
	bool LockYaw;

	Quat RotationOffset;

	// The speed of rotation
	float RotationSpeed;

private:
	// Fill in ..
	void CalculateTargetOffset();

	// Returns the correct frame of reference for the camera's settings
	// @return The AActor to use
	Spatial *GetFrameOfReference();

	// The calculated offset based on frames of reference
	Vector3 TargetOffset;
	
	Vector3 TargetPosition;

	Quat TargetRotation;

	
	void CalculateTargets();

	// Calculates
	
	Vector3 CalculatePosition();

	
	Vector3 CalculateCenter();

	// Given a direction and width, find the offets.
	Vector3 GetPostsOffset(const Vector3 &DirectionVector, float AnchorWidth);

	// Given anchors, what's the anchor width?
	Vector3 GetExtentsOffset(const Vector3 &DirectionVector, float AnchorWidth, float TOffset, float Width);

	Quat RemoveLockedRotation(const Quat &CurrentRotation);

	Vector3 FrameBasedVectorLerp(const Vector3 &From, const Vector3 &To, const Vector3 &Speeds, float Tick);

	Vector3 LerpVector(const Vector3 &From, const Vector3 &To, const Vector3 &Alpha);

	bool GetLerpParam(const float Offst, const float AnchorWidth, const float FloatWidth);

	// Set to a value that gives good clamping, smoothly. Activates when
	// the target is out of range.
	float AnchorSpeed;

protected:
	static void _bind_methods();
};
