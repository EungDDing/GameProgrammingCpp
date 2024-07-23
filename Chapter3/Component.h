#pragma once

#include <cstdint>

class Component {
public:
	Component(class Actor* owner, int updateOrder = 100); // the lower the update order, the earlier the component updates
	virtual ~Component();

	virtual void Update(float deltaTime); // update component by delta time

	virtual void ProcessInput(const uint8_t* keyState) {}

	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner; // owning actor

	int mUpdateOrder; // update order of component
};
