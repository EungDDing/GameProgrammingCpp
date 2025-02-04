#include "FPSCamera.h"
#include "Actor.h"

FPSCamera::FPSCamera(Actor* owner)
	:CameraComponent(owner)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{

}

void FPSCamera::Update(float deltaTime) {
	// call parent update (doesn't do anything right now)
	CameraComponent::Update(deltaTime);
	// camera position is owner position
	Vector3 cameraPos = mOwner->GetPosition();

	// update pitch based on pitch speed
	mPitch += mPitchSpeed * deltaTime;
	// clamp pitch to [-max, +max]
	mPitch = Math::Clamp(mPitch, -mMaxPitch, mMaxPitch);
	// make a quaternion representing pitch rotation, 
	// which is about owner's right vector
	Quaternion q(mOwner->GetRight(), mPitch);

	// rotation owner forward by pitch quaternion
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	// target position 100 units in front of view forward
	Vector3 target = cameraPos + viewForward * 100.0f;
	// also rotate up by pitch quaternion
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// create look at matrix, set as view
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
}