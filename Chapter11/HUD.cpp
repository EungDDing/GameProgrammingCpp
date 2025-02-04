#include "HUD.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "PhysWorld.h"
#include "FPSActor.h"
#include <algorithm>
#include "TargetComponent.h"

HUD::HUD(Game* game)
	:UIScreen(game)
	, mRadarRange(2000.0f)
	, mRadarRadius(92.0f)
	, mTargetEnemy(false)
{
	Renderer* r = mGame->GetRenderer();
	mHealthBar = r->GetTexture("Assets/HealthBar.png");
	mRadar = r->GetTexture("Assets/Radar.png");
	mCrosshair = r->GetTexture("Assets/Crosshair.png");
	mCrosshairEnemy = r->GetTexture("Assets/CrosshairRed.png");
	mBlipTex = r->GetTexture("Assets/Blip.png");
	mRadarArrow = r->GetTexture("Assets/RadarArrow.png");
}

HUD::~HUD()
{

}

void HUD::Update(float deltaTime) {
	UIScreen::Update(deltaTime);

	UpdateCrosshair(deltaTime);
	UpdateRadar(deltaTime);
}

void HUD::Draw(Shader* shader) {
	// crosshair
	Texture* cross = mTargetEnemy ? mCrosshairEnemy : mCrosshair;
	DrawTexture(shader, cross, Vector2::Zero, 2.0f);

	// radar
	const Vector2 cRadarPos(-390.0f, 275.0f);
	DrawTexture(shader, mRadar, cRadarPos, 1.0f);
	// blips
	for (Vector2& blip : mBlips) {
		DrawTexture(shader, mBlipTex, cRadarPos + blip, 1.0f);
	}
	// radar arrow
	DrawTexture(shader, mRadarArrow, cRadarPos);

	//// Health bar
	// DrawTexture(shader, mHealthBar, Vector2(-350.0f, -350.0f);
}

void HUD::AddTargetComponent(TargetComponent* tc) {
	mTargetComps.emplace_back(tc);
}

void HUD::RemoveTargetComponent(TargetComponent* tc) {
	auto iter = std::find(mTargetComps.begin(), mTargetComps.end(), tc);
	mTargetComps.erase(iter);
}

void HUD::UpdateCrosshair(float deltaTime) {
	// reset to regular cursor
	mTargetEnemy = false;
	// make a line segment
	const float cAimDist = 5000.0f;
	Vector3 start, dir;
	mGame->GetRenderer()->GetScreenDirection(start, dir);
	LineSegment l(start, start + dir * cAimDist);
	// segment cast
	PhysWorld::CollisionInfo info;
	if (mGame->GetPhysWorld()->SegmentCast(l, info)) {
		for (auto tc : mTargetComps) {
			if (tc->GetOwner() == info.mActor) {
				mTargetEnemy = true;
				break;
			}
		}
	}
}

void HUD::UpdateRadar(float deltaTime) {
	// clear blip positions from last frame
	mBlips.clear();

	// convert player position to radar coordinates (x forward, z up)
	Vector3 playerPos = mGame->GetPlayer()->GetPosition();
	Vector2 playerPos2D(playerPos.y, playerPos.x);
	// ditto for player forward
	Vector3 playerForward = mGame->GetPlayer()->GetForward();
	Vector2 playerForward2D(playerForward.x, playerForward.y);

	// use atan2 to get rotation of radar
	float angle = Math::Atan2(playerForward2D.y, playerForward2D.x);
	// make a 2D rotation matrix
	Matrix3 rotMat = Matrix3::CreateRotation(angle);

	// get positions of blips
	for (auto tc : mTargetComps) {
		Vector3 targetPos = tc->GetOwner()->GetPosition();
		Vector2 actorPos2D(targetPos.y, targetPos.x);

		// calculate vector between player and target
		Vector2 playerToTarget = actorPos2D - playerPos2D;

		// see if within range
		if (playerToTarget.LengthSq() <= (mRadarRange * mRadarRange)) {
			// convert playerToTarget into offset from the center of the on-screen radar
			Vector2 blipPos = playerToTarget;
			blipPos *= mRadarRadius / mRadarRange;
			// rotate blipPos
			blipPos = Vector2::Transform(blipPos, rotMat);
			mBlips.emplace_back(blipPos);
		}
	}
}
