#include "Ship.h"
#include "SpriteComponent.h"
#include "Game.h"
#include "Laser.h"
#include "InputSystem.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mSpeed(400.0f)
	, mLaserCooldown(0.0f)
{
	// create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));
}

void Ship::UpdateActor(float deltaTime) {
	mLaserCooldown -= deltaTime;

	// update position based on velocity
	Vector2 pos = GetPosition();
	pos += mVelocityDir * mSpeed * deltaTime;
	SetPosition(pos);

	// update rotation
	float angle = Math::Atan2(mRotationDir.y, mRotationDir.x);
	SetRotation(angle);
}

void Ship::ActorInput(const InputState& state) {
	if (state.Controller.GetRightTrigger() > 0.25f
		&& mLaserCooldown <= 0.0f) {
		// create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// reset laser cooldown
		mLaserCooldown = 0.25f;
	}

	if (state.Controller.GetIsConnected()) {
		mVelocityDir = state.Controller.GetLeftStick();
		if (!Math::NearZero(state.Controller.GetRightStick().Length())) {
			mRotationDir = state.Controller.GetRightStick();
		}
	}
}