#pragma once
#include "Component.h"
#include "SDL2/SDL.h"

class SpriteComponent : public Component {
public:
	// lower draw order corresponds with futher back
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(class Shader* shader);
	virtual void SetTexture(SDL_Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWigth() const { return mTexWidth; }
protected:
	SDL_Texture* mTexture;
	int mDrawOrder;
	// width and height of texture
	int mTexWidth;
	int mTexHeight;
};