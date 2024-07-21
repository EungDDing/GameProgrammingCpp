#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mCurrAnim("STOP")
{
}

void AnimSpriteComponent::Update(float deltaTime) {
	SpriteComponent::Update(deltaTime);

	auto& currAnim = mAnimMap[mCurrAnim];

	if (mCurrAnim != "STOP" && currAnim.first.size() > 0) {
		mCurrFrame += mAnimFPS * deltaTime;

		while (mCurrFrame >= currAnim.first.size()) {
			if (currAnim.second) {
				mCurrFrame -= currAnim.first.size();
			}
			else {
				mCurrFrame = currAnim.first.size() - 1;
			}
		}

		SetTexture(currAnim.first[static_cast<int>(mCurrFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::string action, const std::vector<SDL_Texture*>& textures, bool isLoop) {
	mAnimMap[action].first = textures;
	mAnimMap[action].second = isLoop;
	if (mAnimMap[action].first.size() > 0)
	{
		mCurrFrame = 0.0f;
		SetTexture(mAnimMap[action].first[0]);
	}
}