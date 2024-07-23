#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	, mTexture(nullptr)
	, mDrawOrder(drawOrder)
	, mTexWidth(0)
	, mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer) {
	if (mTexture) {
		SDL_Rect r;
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		// draw (have to convert angle from radians to degrees, and clockwise to counter)
		SDL_RenderCopyEx(renderer, // (renderer)
			mTexture, // the source texture (texture)
			nullptr, // the source SDL_Rect structure or NULL for the entire texture (srcrect) 
			&r, // the destination SDL_Rect structure or NULL for the entire rendering target (dstrect)
			-Math::ToDegrees(mOwner->GetRotation()), // an angle in degree that indicates the rotation tha will be applied to dstrect, rotating it in a clockwise direction
			nullptr, // apointer to a point indicating the point around which dstrect will be rotated (if NULL, rotation will be done around dstrect.w / 2, dstrect.h / 2) 
			SDL_FLIP_NONE); // flip
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture) {
	mTexture = texture;
	// set width/height
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}