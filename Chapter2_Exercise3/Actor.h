#pragma once

#include <vector>
#include "Math.h"

class Actor {
public:
	enum State {
		EActive,
		EPaused,
		EDead
	}; // state of actor

	Actor(class Game* game); // constructor
	virtual ~Actor(); // Destructor

	void Update(float deltaTime); // Update function called from Game (not override)
	void UpdateComponents(float deltaTime); // Updates all the components attached to the actor (not override)
	virtual void UpdateActor(float deltaTime); // Any actor-specific update code (override)

	// Getters/Setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;

	// transform
	Vector2 mPosition; // center of actor
	float mScale; // scale of actor (100% = 1.0f)
	float mRotation; // rotation angle (radian)

	std::vector<class Component*> mComponents;
	class Game* mGame;
};