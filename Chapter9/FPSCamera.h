#pragma once
#include "CameraComponent.h"

class FPSCamera : public CameraComponent {
public:
	FPSCamera(class Actor* owner);

	void Update(float deltaTime) override;

	float GetPitch() const { return mPitch; }
	float GetPitchSpeed() const { return mPitchSpeed; }
	float GetMaxPitch() const { return mMaxPitch; }

	void SetPitchSpeed(float speed) { mPitchSpeed = speed; }
	void SetMaxPitch(float pitch) { mMaxPitch = pitch; }
private:
	// rotation per sec speed of pitch
	float mPitchSpeed;
	// maximum pitch deviation from forward
	float mMaxPitch;
	// current pitch
	float mPitch;
};