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

	class Texture* GetTexture(const std::string& filename);

	// game specific
	void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroid() { return mAsteroids; }
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	bool LoadShaders();
	void CreateSpriteVerts();
	void LoadData();
	void UnloadData();

	// map of textures loaded
	std::unordered_map<std::string, class Texture*> mTextures; 

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;

	//all the sprite components drawn
	std::vector<class SpriteComponent*> mSprites; 

	// sprite shader
	class Shader* mSpriteShader;
	// sprite vertex array
	class VertexArray* mSpriteVerts;

	SDL_Window* mWindow;
	SDL_GLContext mContext;
	Uint32 mTicksCount;
	bool mIsRunning;
	bool mUpdatingActors;

	// game specific
	class Ship* mShip;
	std::vector<class Asteroid*> mAsteroids;
};