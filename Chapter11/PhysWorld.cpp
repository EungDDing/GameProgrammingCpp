#include "PhysWorld.h"
#include <algorithm>
#include "BoxComponent.h"
#include "SDL2/SDL.h"

PhysWorld::PhysWorld(Game* game)
	:mGame(game)
{

}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl) {
	bool collided = false;
	// initialize closestT to infinity, so first intersection will always update closestT
	float closestT = Math::Infinity;
	Vector3 norm;
	// test aganist all boxes
	for (auto box : mBoxes) {
		float t;
		// does segment intersect with the box>
		if (Intersect(l, box->GetWorldBox(), t, norm)) {
			// is this closer than previous intersect?
			if (t < closestT) {
				closestT = t;
				outColl.mPoint = l.PointOnSegment(t);
				outColl.mNormal = norm;
				outColl.mBox = box;
				outColl.mActor = box->GetOwner();
				collided = true;
			}
		}
	}
	return collided;
}

void PhysWorld::TestPairwise(std::function<void(Actor*, Actor*)> f) {
	// naive implementation O(n^2)
	for (size_t i = 0; i < mBoxes.size(); i++) {
		// don't need to test vs itself and any previous i value
		for (size_t j = i + 1; j < mBoxes.size(); j++) {
			BoxComponent* a = mBoxes[i];
			BoxComponent* b = mBoxes[j];
			if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
				// call supplied function to handle intersection
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::TestSweepAndPrune(std::function<void(Actor* a, Actor* b)> f) {
	// sort by min.x
	std::sort(mBoxes.begin(), mBoxes.end(),
		[](BoxComponent* a, BoxComponent* b) {
			return a->GetWorldBox().mMin.x <
				b->GetWorldBox().mMin.x;
		});

	for (size_t i = 0; i < mBoxes.size(); i++) {
		// get max.x for current box
		BoxComponent* a = mBoxes[i];
		float max = a->GetWorldBox().mMax.x;
		for (size_t j = i + 1; j < mBoxes.size(); j++) {
			BoxComponent* b = mBoxes[j];
			// if AABB[j] min is past the max bounds of AABB[i],
			// then there aren't any other possible intersections against AABB[i]
			if (b->GetWorldBox().mMin.x > max) {
				break;
			}
			else if (Intersect(a->GetWorldBox(), b->GetWorldBox())) {
				f(a->GetOwner(), b->GetOwner());
			}
		}
	}
}

void PhysWorld::AddBox(BoxComponent* box) {
	mBoxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box) {
	auto iter = std::find(mBoxes.begin(), mBoxes.end(), box);
	if (iter != mBoxes.end()) {
		// swap to end of vector and pop off (avoid erase copies) 
		std::iter_swap(iter, mBoxes.end() - 1);
		mBoxes.pop_back();
	}
}