#pragma once

#include "SDL2/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& filename);

	// game specific
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroid() { return mAsteroids; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> mTextures; // map of textures loaded

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	std::vector<class SpriteComponent*> mSprites; //all the sprite components drawn

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;
	
	// game specific
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids; // vector of asteroid for check collision
};