#pragma once
#include "SDL2/SDL.h"

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOuput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	Uint32 mTicksCount;

	int mPaddleLeftDir;
	int mPaddleRightDir;
	Vector2 mPaddlePosLeft;
	Vector2 mPaddlePosRight;
	Vector2 mBallPos;
	Vector2 mBallVel;
};