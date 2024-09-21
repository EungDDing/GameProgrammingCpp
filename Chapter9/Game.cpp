#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "AudioSystem.h"
#include "AudioComponent.h"
#include "FPSActor.h"
#include "FollowActor.h"
#include "OrbitActor.h"
#include "SplineActor.h"

Game::Game()
	:mRenderer(nullptr)
	, mIsRunning(true)
	, mUpdatingActors(false)
	, mAudioSystem(nullptr)
{

}

bool Game::Initialize() {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	// create the renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f)) {
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	// create the audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize()) {
		SDL_Log("Failed to initialize audio system");
		mAudioSystem->Shutdown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mIsRunning = false;
			break;
			// This fires when a key's initially pressed
		case SDL_KEYDOWN:
			if (!event.key.repeat) {
				HandleKeyPress(event.key.keysym.sym);
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			HandleKeyPress(event.button.button);
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}

	for (auto actor : mActors) {
		actor->ProcessInput(state);
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key) {
	case '-':
	{
		// reduce master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Max(0.0f, volume - 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '=':
	{
		// increase master volume
		float volume = mAudioSystem->GetBusVolume("bus:/");
		volume = Math::Min(1.0f, volume + 0.1f);
		mAudioSystem->SetBusVolume("bus:/", volume);
		break;
	}
	case '1':
	case '2':
	case '3':
	case '4':
		ChangeCamera(key);
		break;
	case SDL_BUTTON_LEFT:
	{
		// get start point (in center of screen on near plane)
		Vector3 screenPoint(0.0f, 0.0f, 0.0f);
		Vector3 start = mRenderer->Unproject(screenPoint);
		// get end point (in center of screen, between near and far)
		screenPoint.z = 0.9f;
		Vector3 end = mRenderer->Unproject(screenPoint);
		// get spheres to points
		mStartSphere->SetPosition(start);
		mEndSphere->SetPosition(end);
		break;
	}
	default:
		break;
	}
}

void Game::UpdateGame() {
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	mUpdatingActors = true;
	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false; // update all actor

	for (auto pending : mPendingActors) {
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear(); // move any pending actors tp mActors

	std::vector<Actor*> deadActors;
	for (auto actor : mActors) {
		if (actor->GetState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	} // add any dead actors to a temp vector

	for (auto actor : deadActors) {
		delete actor;
	} // delete dead actors

	mAudioSystem->Update(deltaTime);
}

void Game::GenerateOutput() {
	mRenderer->Draw();
}

void Game::LoadData() {
	Actor* a = new Actor(this);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);

	// setup floor
	const float start = -1250.0f;
	const float size = 250.0f;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			a = new PlaneActor(this);
			a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
		}
	}

	// left/right walls
	q = Quaternion(Vector3::UnitX, Math::PiOver2);
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
		a->SetRotation(q);
	}

	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
	// forward/back walls
	for (int i = 0; i < 10; i++) {
		a = new PlaneActor(this);
		a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
		a->SetRotation(q);

		a = new PlaneActor(this);
		a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
		a->SetRotation(q);
	}

	// setup light
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	a = new Actor(this);
	a->SetScale(2.0f);
	mCrosshair = new SpriteComponent(a);
	mCrosshair->SetTexture(mRenderer->GetTexture("Assets/Crosshair.png"));

	// enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// different camera actors
	mFPSActor = new FPSActor(this);
	mFollowActor = new FollowActor(this);
	mOrbitActor = new OrbitActor(this);
	mSplineActor = new SplineActor(this);

	ChangeCamera('1');

	// spheres for demonstrating unprojection
	mStartSphere = new Actor(this);
	mStartSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));
	mStartSphere->SetScale(0.25f);
	MeshComponent* mc = new MeshComponent(mStartSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mEndSphere = new Actor(this);
	mEndSphere->SetPosition(Vector3(10000.0f, 0.0f, 0.0f));
	mEndSphere->SetScale(0.25f);
	mc = new MeshComponent(mEndSphere);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	mc->SetTextureIndex(1);
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}

	if (mRenderer) {
		mRenderer->UnloadData();
	}
}

void Game::Shutdown() {
	UnloadData();
	if (mRenderer) {
		mRenderer->Shutdown();
	}
	if (mAudioSystem) {
		mAudioSystem->Shutdown();
	}
	SDL_Quit();
}

void Game::AddActor(Actor* actor) {
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()) {
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::ChangeCamera(int mode) {
	// disable everything
	mFPSActor->SetState(Actor::EPaused);
	mFPSActor->SetVisible(false);
	mCrosshair->SetVisible(false);
	mFollowActor->SetState(Actor::EPaused);
	mFollowActor->SetVisible(false);
	mOrbitActor->SetState(Actor::EPaused);
	mOrbitActor->SetVisible(false);
	mSplineActor->SetState(Actor::EPaused);

	// enable the camera specified by the mode
	switch (mode)
	{
	case '1':
	default:
		mFPSActor->SetState(Actor::EActive);
		mFPSActor->SetVisible(true);
		mCrosshair->SetVisible(true);
		break;
	case '2':
		mFollowActor->SetState(Actor::EActive);
		mFollowActor->SetVisible(true);
		break;
	case '3':
		mOrbitActor->SetState(Actor::EActive);
		mOrbitActor->SetVisible(true);
		break;
	case '4':
		mSplineActor->SetState(Actor::EActive);
		mSplineActor->RestartSpline();
		break;
	}
}