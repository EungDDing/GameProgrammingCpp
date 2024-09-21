#pragma once
#include "CameraComponent.h"

class OrbitCamera : public CameraComponent {
public:
	OrbitCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetYawSpeed() const { return mYawSpeed; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetYawSpeed(float speed) { mYawSpeed = speed; }
private:
	// offset from target
	Vector3 mOffset;
	// up vector of camera
	Vector3 mUp;
	// rotation per sec speed of pitch
	float mPitchSpeed;
	// rotation per sec speed of yaw
	float mYawSpeed;
};