#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "CameraActor.h"
#include "PlaneActor.h"
#include "AudioSystem.h"
#include "AudioComponent.h"

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
	case 'e':
		// play explosion
		mAudioSystem->PlayEvent("event:/Explosion2D");
		break;
	case 'm':
		// toggle music pause state
		mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
		break;
	case 'r':
		// stop or start reverb snapshot
		if (!mReverbSnap.IsValid())
		{
			mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
		}
		else
		{
			mReverbSnap.Stop();
		}
		break;
	case '1':
		// set default footstep surface
		mCameraActor->SetFootstepSurface(0.0f);
		break;
	case '2':
		// set grass footstep surface
		mCameraActor->SetFootstepSurface(0.5f);
		break;
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
	// create actors
	Actor* a = new Actor(this);
	a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
	a->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	a->SetRotation(q);
	MeshComponent* mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	a = new Actor(this);
	a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
	a->SetScale(3.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

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

	mCameraActor = new CameraActor(this);

	a = new Actor(this);
	a->SetPosition(Vector3(500.0f, -75.0f, 0.0f));
	a->SetScale(1.0f);
	mc = new MeshComponent(a);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
	AudioComponent* ac = new AudioComponent(a);
	ac->PlayEvent("event:/FireLoop");

	// start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");
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