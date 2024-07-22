#include "TileMapComponent.h"
#include "Actor.h"
#include <fstream>
#include <sstream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
{

}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{

	for (int row = 0; row < mapHeigthCount; ++row) {
		for (int col = 0; col < mapWidthCount; ++col) {
			int tileNum = mMap.at(row * mapWidthCount + col);

			if (tileNum != -1) {
				SDL_Rect srcrect;
				srcrect.w = tileWidth;
				srcrect.h = tileHeigth;
				srcrect.x = (tileNum % 8) * tileWidth;
				srcrect.y = (tileNum / 8) * tileHeigth;

				SDL_Rect dstrect;
				dstrect.w = tileWidth;
				dstrect.h = tileHeigth;
				dstrect.x = col * tileWidth;
				dstrect.y = row * tileHeigth;

				SDL_RenderCopyEx(renderer,
					mTexture,
					&srcrect,
					&dstrect,
					-Math::ToDegrees(mOwner->GetRotation()),
					nullptr,
					SDL_FLIP_NONE);
			}
		}
	}

}
void TileMapComponent::SetMap(const std::string& fileName) {

	std::ifstream fs;
	fs.open(fileName);
	std::string str;

	mMap.clear();

	while (!fs.eof()) {
		getline(fs, str, ',');
		int index = str.find('\n');
		if (index != std::string::npos) {
			str.erase(index, 1);
			std::string former = str.substr(0, index);
			std::string later = str.substr(index, str.size() - index);

			mMap.push_back(std::stoi(former));
			if (!later.empty()) {
				mMap.push_back(stoi(later));
			}
		}
		else {
			mMap.push_back(stoi(str));
		}
	}


	fs.close();
}