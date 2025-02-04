#pragma once
#include <vector>
#include <functional>
#include "Math.h"
#include "Collision.h"

class PhysWorld
{
public:
	PhysWorld(class Game* game);

	// used to give helpful information about collision results
	struct CollisionInfo
	{
		// point of collision
		Vector3 mPoint;
		// normal at collision
		Vector3 mNormal;
		// component collided with
		class BoxComponent* mBox;
		// owning actor of component
		class Actor* mActor;
	};

	// test a line segment against boxes
	// return true if it collides against a box
	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	// tests collisions using naive pairwise
	void TestPairwise(std::function<void(class Actor*, class Actor*)> f);
	// tests collision using sweep and prune
	void TestSweepAndPrune(std::function<void(class Actor*, class Actor*)> f);

	// add/remove box components form world
	void AddBox(class BoxComponent* box);
	void RemoveBox(class BoxComponent* box);
private:
	class Game* mGame;
	std::vector<class BoxComponent*> mBoxes;
};
