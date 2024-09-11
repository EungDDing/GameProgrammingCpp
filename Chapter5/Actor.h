#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

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


	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// any actor-specific input code(overridable)
	virtual void ActorInput(const uint8_t* keyState);

	// Getters/Setters
	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() const { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector2 GetForward() const { return Vector2(Math::Cos(mRotation), Math::Sin(mRotation)); }
	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;

	// transform
	Matrix4 mWorldTransform;// store a world transform matrix
	Vector2 mPosition;		// center of actor
	float mScale;			// scale of actor (100% = 1.0f)
	float mRotation;		// rotation angle (radian)
	bool mRecomputeWorldTransform; // store if the world transform matrix needs to be recompute

	std::vector<class Component*> mComponents;
	class Game* mGame;
};