#pragma once
#include "CameraComponent.h"

class FollowCamera : public CameraComponent {
public:
	FollowCamera(class Actor* owner);

	void Update(float deltaTime) override;

	void SnapToIdeal();

	void SetHorzDist(float dist) { mHorzDist = dist; }
	void SetVertDist(float dist) { mVertDist = dist; }
	void SetTargetDist(float dist) { mTargetDist = dist; }
	void SetSpringConstant(float spring) { mSpringConstant = spring; }
private:
	Vector3 ComputeCameraPos() const;
	
	// actual position of camera
	Vector3 mActualPos;
	// velocity of actual camera
	Vector3 mVelocity;
	// horizontal follow distance
	float mHorzDist;
	// vertical follow distance
	float mVertDist;
	// target distance
	float mTargetDist;
	// spring constant (higher is more stiff)
	float mSpringConstant;
};