#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	// control points for spline
	// (requires n + 2 points where n is number of points in segment)
	std::vector<Vector3> mControlPoints;
	// given spline segment where startIdx = P1,
	// compute position based on t value
	Vector3 Compute(size_t startIdx, float t) const;
	// returns number of control points
	size_t GetNumPoints() const { return mControlPoints.size(); }
};

class SplineCamera : public CameraComponent {
public:
	SplineCamera(class Actor* owner);

	void Update(float deltaTime) override;
	// restart the spline
	void Restart();

	void SetSpeed(float speed) { mSpeed = speed; }
	void SetSpline(const Spline& spline) { mPath = spline; }
	void SetPaused(bool pause) { mPaused = pause; }
private:
	// apline path camera follows
	Spline mPath;
	// current control point index and t
	size_t mIndex;
	float mT;
	// amount t changes per sec
	float mSpeed;
	// whether to move the camera long the path
	bool mPaused;
};