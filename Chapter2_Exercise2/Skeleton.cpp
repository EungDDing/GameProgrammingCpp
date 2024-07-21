#include "Skeleton.h"
#include "Game.h"
#include <string>

Skeleton::Skeleton(Game* game) 
	:Actor(game)
	, mRightSpeed(0.0f)
{
	asc = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> walkingAnims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png")
	};
	asc->SetAnimTextures(std::string("walk"), walkingAnims, true);
	std::vector<SDL_Texture*> jumpingAnims = {
		game->GetTexture("Assets/Character07.png"),
		game->GetTexture("Assets/Character08.png"),
		game->GetTexture("Assets/Character09.png"),
		game->GetTexture("Assets/Character10.png"),
		game->GetTexture("Assets/Character11.png"),
		game->GetTexture("Assets/Character12.png"),
		game->GetTexture("Assets/Character13.png"),
		game->GetTexture("Assets/Character14.png"),
		game->GetTexture("Assets/Character15.png"),
		game->GetTexture("Assets/Character01.png")
	};
	asc->SetAnimTextures(std::string("jump"), jumpingAnims, false);
	std::vector<SDL_Texture*> punchingAnims = {
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character18.png"),
		game->GetTexture("Assets/Character17.png"),
		game->GetTexture("Assets/Character16.png"),
		game->GetTexture("Assets/Character01.png")
	};
	asc->SetAnimTextures(std::string("punch"), punchingAnims, false);
}

void Skeleton::UpdateActor(float deltaTime) {
	Actor::UpdateActor(deltaTime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltaTime;
	if (pos.x < 500.0f) {
		pos.x = 500.0f;
	}
	else if (pos.x > 500.0f) {
		pos.x = 500.0f;
	}
	SetPosition(pos);
}

void Skeleton::ProcessKeyBoard(const uint8_t* state) {
	mRightSpeed = 0.0f;

	if (state[SDL_SCANCODE_D]) {	
		asc->SetCurrAnim(std::string("walk"));
	}
	if (state[SDL_SCANCODE_S]) {
		asc->SetCurrAnim(std::string("STOP"));
	}
	if (state[SDL_SCANCODE_SPACE]) {
		asc->SetCurrAnim(std::string("jump"));
	}
	if (state[SDL_SCANCODE_P]) {
		asc->SetCurrAnim(std::string("punch"));
	}
}