#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner, int updateOrder)
	:mOwner(owner)
	, mUpdateOrder(updateOrder)
{
	mOwner->AddComponent(this); // add to actor's vector of components
}

Component::~Component() {
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime) {

}