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
	const Vector3& GetPosition() const { return mPosition; }
	void SetPosition(const Vector3& pos) { mPosition = pos; mRecomputeWorldTransform = true; }
	float GetScale() const { return mScale; }
	void SetScale(float scale) { mScale = scale; mRecomputeWorldTransform = true; }
	const Quaternion& GetRotation() const { return mRotation; }
	void SetRotation(const Quaternion& rotation) { mRotation = rotation; mRecomputeWorldTransform = true; }

	void ComputeWorldTransform();
	const Matrix4& GetWorldTransform() const { return mWorldTransform; }

	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, mRotation); }
	Vector3 GetRight() const { return Vector3::Transform(Vector3::UnitY, mRotation); }

	State GetState() const { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	// add/remove components
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;

	// transform
	Matrix4 mWorldTransform;
	Vector3 mPosition;
	float mScale;
	Quaternion mRotation;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
};