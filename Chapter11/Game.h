#pragma once
#include "SDL2/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"
#include "SoundEvent.h"
#include "SDL2/SDL_types.h"

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	class Renderer* GetRenderer() { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }
	class PhysWorld* GetPhysWorld() { return mPhysWorld; }
	class HUD* GetHUD() { return mHUD; }
	// manage UI stack
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
	void PushUI(class UIScreen* screen);

	class FPSActor* GetPlayer() { return mFPSActor; }
	enum GameState
	{
		EGameplay,
		EPaused,
		EQuit
	};

	GameState GetState() const { return mGameState; }
	void SetState(GameState state) { mGameState = state; }

	class Font* GetFont(const std::string& fileName);

	void LoadText(const std::string& fileName);
	const std::string& GetText(const std::string& key);

	void AddPlane(class PlaneActor* plane);
	void RemovePlane(class PlaneActor* plane);
	std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }
private:
	void ProcessInput();
	void HandleKeyPress(int key);
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::unordered_map<std::string, class Font*> mFonts;
	std::unordered_map<std::string, std::string> mText;
	std::vector<class UIScreen*> mUIStack;

	class Renderer* mRenderer;
	class AudioSystem* mAudioSystem;
	class PhysWorld* mPhysWorld;
	class HUD* mHUD;

	Uint32 mTicksCount;
	GameState mGameState;
	bool mUpdatingActors;

	class FPSActor* mFPSActor;
	class SpriteComponent* mCrosshair;
	SoundEvent mMusicEvent;
	std::vector <class PlaneActor*> mPlanes;
};