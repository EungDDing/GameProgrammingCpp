#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mIsRunning(true)
	, mTicksCount(0)
	, mPaddleLeftDir(0)
	, mPaddleRightDir(0)
{

}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)",
		100,
		100,
		1024,
		768,
		0
	);

	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(
		mWindow,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	mPaddlePosLeft.x = 10.0f;
	mPaddlePosLeft.y = 768.0f / 2.0f;
	mPaddlePosRight.x = 1024.0f - 25.0f;
	mPaddlePosRight.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;
}

void Game::Shutdown() {
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOuput();
	}
}
void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	mPaddleLeftDir = 0;
	mPaddleRightDir = 0;

	if (state[SDL_SCANCODE_W]) {
		mPaddleLeftDir -= 1;
	}

	if (state[SDL_SCANCODE_S]) {
		mPaddleLeftDir += 1;
	}

	if (state[SDL_SCANCODE_I]) {
		mPaddleRightDir -= 1;
	}

	if (state[SDL_SCANCODE_K]) {
		mPaddleRightDir += 1;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16)) //wait until 16ms has elapsed since last frame
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	mTicksCount = SDL_GetTicks();

	if (mPaddleLeftDir != 0) {
		mPaddlePosLeft.y += mPaddleLeftDir * 300.0f * deltaTime;

		if (mPaddlePosLeft.y < paddleH / 2.0f + thickness) {
			mPaddlePosLeft.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePosLeft.y > (768.0f - (paddleH / 2.0f + thickness))) {
			mPaddlePosLeft.y = 768.0f - (paddleH / 2.0f + thickness);
		}
	}

	if (mPaddleRightDir != 0) {
		mPaddlePosRight.y += mPaddleRightDir * 300.0f * deltaTime;

		if (mPaddlePosRight.y < paddleH / 2.0f + thickness) {
			mPaddlePosRight.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePosRight.y > (768.0f - (paddleH / 2.0f + thickness))) {
			mPaddlePosRight.y = 768.0f - (paddleH / 2.0f + thickness);
		}
	}

	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime; //update ball position based on ball velocity


	float diffLeft = mPaddlePosLeft.y - mBallPos.y;
	diffLeft = (diffLeft > 0.0f) ? diffLeft : -diffLeft;

	float diffRight = mPaddlePosRight.y - mBallPos.y;
	diffRight = (diffRight > 0.0f) ? diffRight : -diffRight;

	if (
		diffLeft < paddleH / 2.0f &&
		mBallPos.x <= 25.0f && mBallPos.x >= 10.0f &&
		mBallVel.x < 0.0f) {
		mBallVel.x *= -1.0f;
	} //paddle Left
	else if (mBallPos.x <= 0.0f) {
		mIsRunning = false;
	} //game over

	if (
		diffRight < paddleH / 2.0f &&
		mBallPos.x >= (1024.0f - 25.0f) && mBallPos.x <= (1024.0f - 10.0f) &&
		mBallVel.x > 0.0f) {
		mBallVel.x *= -1.0f;
	} //paddle Right
	else if (mBallPos.x >= 1024.0f) {
		mIsRunning = false;
	} //game over

	if (mBallPos.y <= thickness && mBallVel.y < 0.0f) {
		mBallVel.y *= -1.0f;
	} //top
	else if (mBallPos.y >= (768.0f - thickness) && mBallVel.y > 0.0f) {
		mBallVel.y *= -1.0f;
	} //bottom
}

void Game::GenerateOuput() {
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(
		mRenderer,
		255,
		255,
		255,
		255
	);

	SDL_Rect wall{
		0,
		0,
		1024, //width(w)
		thickness //height(h)
	};
	SDL_RenderFillRect(mRenderer, &wall); //top

	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall); //bottom

	SDL_Rect paddleLeft{
		static_cast<int>(mPaddlePosLeft.x),
		static_cast<int>(mPaddlePosLeft.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddleLeft);

	SDL_Rect paddleRight{
		static_cast<int>(mPaddlePosRight.x),
		static_cast<int>(mPaddlePosRight.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};

	SDL_RenderFillRect(mRenderer, &paddleRight);

	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};

	SDL_RenderFillRect(mRenderer, &ball); //ball

	SDL_RenderPresent(mRenderer);
}