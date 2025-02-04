#include "Game.h"
#include "Renderer.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "PlaneActor.h"
#include "AudioSystem.h"
#include "FPSActor.h"
#include "TargetActor.h"
#include "BallActor.h"
#include "PhysWorld.h"
#include "Font.h"
#include "HUD.h"
#include "UIScreen.h"
#include "PauseMenu.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL.h"
#include "rapidjson/document.h"
#include <fstream>
#include <sstream>

Game::Game()
	:mRenderer(nullptr)
	, mUpdatingActors(false)
	, mAudioSystem(nullptr)
	, mPhysWorld(nullptr)
	, mGameState(EGameplay)
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

	// create the physics world
	mPhysWorld = new PhysWorld(this);

	// initialize SDL_ttf
	if (TTF_Init() != 0) {
		SDL_Log("Failed to initialize SDL_ttf");
		return false;
	}

	LoadData();

	mTicksCount = SDL_GetTicks();

	return true;
}

void Game::RunLoop() {
	while (mGameState != EQuit) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::AddPlane(PlaneActor* plane) {
	mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor* plane) {
	auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
	mPlanes.erase(iter);
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			mGameState = EQuit;
			break;
			// This fires when a key's initially pressed
		case SDL_KEYDOWN:
			if (!event.key.repeat) {
				if (mGameState == EGameplay) {
					HandleKeyPress(event.key.keysym.sym);
				}
				else if (!mUIStack.empty()) {
					mUIStack.back()->HandleKeyPress(event.key.keysym.sym);
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (mGameState == EGameplay)
			{
				HandleKeyPress(event.button.button);
			}
			else if (!mUIStack.empty())
			{
				mUIStack.back()->
					HandleKeyPress(event.button.button);
			}
			break;
		default:
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (mGameState == EGameplay)
	{
		for (auto actor : mActors)
		{
			if (actor->GetState() == Actor::EActive)
			{
				actor->ProcessInput(state);
			}
		}
	}
	else if (!mUIStack.empty())
	{
		mUIStack.back()->ProcessInput(state);
	}
}

void Game::HandleKeyPress(int key)
{
	switch (key) {
	case SDLK_ESCAPE:
		// create pause menu
		new PauseMenu(this);
		break;
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
	{
		// load English text
		LoadText("Assets/English.gptext");
		break;
	}
	case '2':
	{
		// load Russian text
		LoadText("Assets/Russian.gptext");
		break;
	}
	case SDL_BUTTON_LEFT:
	{
		// fire weapon
		mFPSActor->Shoot();
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

	if (mGameState == EGameplay) {
		// update all actor
		mUpdatingActors = true;
		for (auto actor : mActors) {
			actor->Update(deltaTime);
		}
		mUpdatingActors = false;

		// move any pending actors to mActors
		for (auto pending : mPendingActors) {
			pending->ComputeWorldTransform();
			mActors.emplace_back(pending);
		}
		mPendingActors.clear();

		// add any dead actors to a temp vector
		std::vector<Actor*> deadActors;
		for (auto actor : mActors) {
			if (actor->GetState() == Actor::EDead) {
				deadActors.emplace_back(actor);
			}
		}

		// delete dead actors
		for (auto actor : deadActors) {
			delete actor;
		}
	}

	mAudioSystem->Update(deltaTime);

	// update UI screens
	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}

	// delete any UIScreens that are closed
	auto iter = mUIStack.begin();
	while (iter != mUIStack.end())
	{
		if ((*iter)->GetState() == UIScreen::EClosing)
		{
			delete* iter;
			iter = mUIStack.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Game::GenerateOutput() {
	mRenderer->Draw();
}

void Game::LoadData() {
	// load English text
	LoadText("Assets/English.gptext");
	// create actor
	Actor* a = nullptr;
	Quaternion q;

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

	// UI elements
	mHUD = new HUD(this);

	// start music
	mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

	// Enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

	// different camera actors
	mFPSActor = new FPSActor(this);

	// create target actors
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
	a = new TargetActor(this);
	a->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
	a = new TargetActor(this);
	a->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
	a->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}

	// clear the UI stack
	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}

	if (mRenderer) {
		mRenderer->UnloadData();
	}
}

void Game::Shutdown() {
	UnloadData();
	TTF_Quit();
	delete mPhysWorld;

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

void Game::PushUI(UIScreen* screen) {
	mUIStack.emplace_back(screen);
}

Font* Game::GetFont(const std::string& fileName) {
	auto iter = mFonts.find(fileName);
	if (iter != mFonts.end()) {
		return iter->second;
	}
	else {
		Font* font = new Font(this);
		if (font->Load(fileName)) {
			mFonts.emplace(fileName, font);
		}
		else {
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}
}

void Game::LoadText(const std::string& fileName)
{
	// clear the existing map, if already loaded
	mText.clear();

	// try to open the file
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		SDL_Log("Text file %s not found", fileName.c_str());
		return;
	}

	// read the entire file to a string stream
	std::stringstream fileStream;
	fileStream << file.rdbuf();
	std::string contents = fileStream.str();

	// open this file in rapidJSON
	rapidjson::StringStream jsonStr(contents.c_str());
	rapidjson::Document doc;
	doc.ParseStream(jsonStr);
	if (!doc.IsObject())
	{
		SDL_Log("Text file %s is not valid JSON", fileName.c_str());
		return;
	}

	// parse the text map
	const rapidjson::Value& actions = doc["TextMap"];
	for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
		itr != actions.MemberEnd(); ++itr)
	{
		if (itr->name.IsString() && itr->value.IsString())
		{
			mText.emplace(itr->name.GetString(),
				itr->value.GetString());
		}
	}
}

const std::string& Game::GetText(const std::string& key)
{
	static std::string errorMsg("**KEY NOT FOUND**");
	// find this text in the map, if it exists
	auto iter = mText.find(key);
	if (iter != mText.end())
	{
		return iter->second;
	}
	else
	{
		return errorMsg;
	}
}