#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override; // update animation every frame (overriden from component)
	void SetAnimTexture(const std::vector<SDL_Texture*>& textures); // set the textures used for animation
	
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
private:
	std::vector<SDL_Texture*> mAnimTextures; // all textures in the animation
	float mCurrFrame; // current frame display
	float mAnimFPS; // animation frame rate
};
