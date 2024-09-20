#pragma once
#include "Actor.h"

class Ship : public Actor {
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const struct InputState& state) override;
private:
	Vector2 mVelocityDir;
	Vector2 mRotationDir;
	float mSpeed;
	float mLaserCooldown;
};