#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0) {
		mCurrFrame += mAnimFPS * deltaTime; // update the current frame based on frame rate and delta time

		while (mCurrFrame >= mAnimTextures.size()) {
			mCurrFrame -= mAnimTextures.size();
		} // wrap current frame if the current frame over size of animation's texture

		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]); // set the current texture
	}
}

void AnimSpriteComponent::SetAnimTexture(const std::vector<SDL_Texture*>& textures) {
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0) {
		//set active texture to first frame
		mCurrFrame = 0.0f; 
		SetTexture(mAnimTextures[0]);
	}
}