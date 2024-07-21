#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <map>

class AnimSpriteComponent : public SpriteComponent {
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override; // update animation every frame (overriden from component)
	void SetAnimTextures(std::string action, const std::vector<SDL_Texture*>& textures, bool isLoop); // set the textures used for animation

	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	void SetCurrAnim(std::string anim) {
		mCurrAnim = anim;
		mCurrFrame = 0.0f;
	}
private:
	std::map<std::string, std::pair<std::vector<SDL_Texture*>, bool>> mAnimMap; // mapping skeleton's state(string) with texture
	std::string mCurrAnim;
	float mCurrFrame; // current frame display
	float mAnimFPS; // animation frame rate
};