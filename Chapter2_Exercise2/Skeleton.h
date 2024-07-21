#pragma once

#include "Actor.h"
#include "AnimSpriteComponent.h"

class Skeleton : public Actor {
public:
	Skeleton(class Game* game);
	void UpdateActor(float deltaTime) override;
	void ProcessKeyBoard(const uint8_t* state);

	void SetAction(int state, std::vector<AnimSpriteComponent*> action) { mAction.insert({ state, action }); }
	float GetRightSpeed() const { return mRightSpeed; }
private:
	float mRightSpeed;
	std::map<int, std::vector<AnimSpriteComponent*>> mAction;
	AnimSpriteComponent* asc;
};