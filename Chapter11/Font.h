#pragma once
#include <string>
#include <unordered_map>
#include "SDL2/SDL_ttf.h"
#include "Math.h"

class Font
{
public:
	Font(class Game* game);
	~Font();

	// load/unload from a file
	bool Load(const std::string& fileName);
	void Unload();

	// given string and this font, draw to a texture
	class Texture* RenderText(const std::string& textKey, const Vector3& color = Color::White, int pointSize = 30);

private:
	// map of point sizes to font data
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};