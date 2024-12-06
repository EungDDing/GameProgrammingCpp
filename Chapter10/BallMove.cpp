#include "BallMove.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "TargetActor.h"
#include "BallActor.h"

BallMove::BallMove(Actor* owner)
	:MoveComponent(owner)
{

}

void BallMove::Update(float deltaTime) {
	// consturct segment in direction of travel
	const float segmentLength = 30.0f;
	Vector3 start = mOwner->GetPosition();
	Vector3 dir = mOwner->GetForward();
	Vector3 end = start + dir * segmentLength;

	// create line segment
	LineSegment l(start, end);

	// test segment vs world
	PhysWorld* phys = mOwner->GetGame()->GetPhysWorld();
	PhysWorld::CollisionInfo info;
	// don't collide vs player
	if (phys->SegmentCast(l, info) && info.mActor != mPlayer) {
		// if we collided, reflect the ball about normal
		dir = Vector3::Reflect(dir, info.mNormal);
		mOwner->RotateToNewForward(dir);
		// did we hit a target?
		TargetActor* target = dynamic_cast<TargetActor*>(info.mActor);
		if (target) {
			static_cast<BallActor*>(mOwner)->HitTarget();
		}
	}

	// base class update moves based on forward speed
	MoveComponent::Update(deltaTime);
}