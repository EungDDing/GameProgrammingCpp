#pragma once
#include <cstdint>

class Component {
public:
	// the lower the update order, the earlier the component updates
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	// update component by delta time
	virtual void Update(float deltaTime);
	// process input for this component
	virtual void ProcessInput(const uint8_t* keyState) {}
	// called when world transform change
	virtual void OnUpdateWorldTransform() { }

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; // owning actor

	int mUpdateOrder; // update order of component
};