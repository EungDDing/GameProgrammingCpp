#pragma once

#include "SpriteComponent.h"
#include <vector>
#include <string>

class TileMapComponent : public SpriteComponent {
public:
	TileMapComponent(class Actor* owner, int drawOrder = 10);

	void Draw(SDL_Renderer* renderer) override;
	void SetMap(const std::string& fileName);
private:
	std::vector<int> mMap;
	int tileWidth = 32;
	int tileHeigth = 32;
	int mapWidthCount = 32;
	int mapHeigthCount = 24;
};