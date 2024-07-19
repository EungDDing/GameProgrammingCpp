#pragma once

#include "SpriteComponent.h"
#include <vector>
#include "Math.h"

class BGSpriteComponent : public SpriteComponent {
public:
	BGSpriteComponent(class Actor* owner, int drawOrder = 10); // set draw order to default to lower
	// Update/Draw overridden from parent(SpriteComponent)
	void Update(float deltaTime) override;
	void Draw(SDL_Renderer* renderer) override;
	void SetBGTextures(const std::vector<SDL_Texture*>& textures); // set the texture used for the background
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed() const { return mScrollSpeed; }
private:
	struct BGTexture {
		SDL_Texture* mTexture;
		Vector2 mOffset; // (x, y)
	}; // struct to encapsulate each back ground image and its(back ground image's) offset
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize; // screen size(w, h)
	float mScrollSpeed;
};
