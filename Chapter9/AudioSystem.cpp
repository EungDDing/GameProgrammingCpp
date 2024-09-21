#include "AudioSystem.h"
#include "SDL2/SDL_log.h"
#include "fmod_studio.hpp"
#include "fmod_errors.h"
#include <vector>

unsigned int AudioSystem::sNextID = 0;

AudioSystem::AudioSystem(Game* game)
	:mGame(game)
	, mSystem(nullptr)
	, mLowLevelSystem(nullptr)
{

}

AudioSystem::~AudioSystem()
{

}

bool AudioSystem::Initialize() {
	// initialize debug logging
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // log only error
		FMOD_DEBUG_MODE_TTY //output to stdout
	);

	// create FMOD studio system object
	FMOD_RESULT result;
	result = FMOD::Studio::System::create(&mSystem);
	if (result != FMOD_OK) {
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// initialize FMOD studio system
	result = mSystem->initialize(
		512, // max number of concurrent sounds
		FMOD_STUDIO_INIT_NORMAL, // use default settings
		FMOD_INIT_NORMAL, // use default settings
		nullptr // usually null
	);
	if (result != FMOD_OK) {
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	// save the low-level system pointer
	mSystem->getLowLevelSystem(&mLowLevelSystem);

	// load the master banks (string first)
	LoadBank("Assets/Master Bank.strings.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Shutdown() {
	// unload all banks
	UnloadAllBanks();
	// shutdown FMOD system
	if (mSystem) {
		mSystem->release();
	}
}

void AudioSystem::LoadBank(const std::string& name) {
	// prevent double-loading
	if (mBanks.find(name) != mBanks.end()) {
		return;
	}

	// try to load bank
	FMOD::Studio::Bank* bank = nullptr;
	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(), // file name of bank
		FMOD_STUDIO_LOAD_BANK_NORMAL, // normal loading
		&bank // svae pointer to bank
	);

	const int maxPathLength = 512;
	if (result == FMOD_OK) {
		// add bank to map
		mBanks.emplace(name, bank);
		// load all non-streaming sample data
		bank->loadSampleData();
		// get the number of events in this bank
		int numEvents = 0;
		bank->getEventCount(&numEvents);
		if (numEvents > 0) {
			// get list of event descriptions in this bank
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++) {
				FMOD::Studio::EventDescription* e = events[i];
				// get the path of this event (like event:/Explosion2D)
				e->getPath(eventName, maxPathLength, nullptr);
				// add to event map
				mEvents.emplace(eventName, e);
			}
		}
		// get the number of buses in this bank
		int numBuses = 0;
		bank->getBusCount(&numBuses);
		if (numBuses > 0) {
			// get list of buses in this bank
			std::vector<FMOD::Studio::Bus*> buses(numBuses);
			bank->getBusList(buses.data(), numBuses, &numBuses);
			char busName[512];
			for (int i = 0; i < numBuses; i++) {
				FMOD::Studio::Bus* bus = buses[i];
				// get the path of this bus (like bus:/SFX)
				bus->getPath(busName, 512, nullptr);
				// add to buses map
				mBuses.emplace(busName, bus);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name) {
	// ignore if not loaded
	auto iter = mBanks.find(name);
	if (iter == mBanks.end()) {
		return;
	}

	// first we need to remove all events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);
	if (numEvents > 0) {
		// get event descriptions for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		// get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];
		for (int i = 0; i < numEvents; i++) {
			FMOD::Studio::EventDescription* e = events[i];
			// get the path of this event
			e->getPath(eventName, 512, nullptr);
			// remove this event
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end()) {
				mEvents.erase(eventi);
			}
		}
	}
	// get the number of buses in this bank
	int numBuses = 0;
	bank->getBusCount(&numBuses);
	if (numBuses > 0) {
		// get list of buses in this bank
		std::vector<FMOD::Studio::Bus*> buses(numBuses);
		bank->getBusList(buses.data(), numBuses, &numBuses);
		char busName[512];
		for (int i = 0; i < numBuses; i++) {
			FMOD::Studio::Bus* bus = buses[i];
			// get the path of this bus (like bus:/SFX)
			bus->getPath(busName, 512, nullptr);
			// remove this bus
			auto busi = mBuses.find(busName);
			if (busi != mBuses.end()) {
				mBuses.erase(busi);
			}
		}
	}

	// unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	// remove from banks map
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks() {
	for (auto& iter : mBanks) {
		// unload the sample data then the bank itself
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	// no banks means no events
	mEvents.clear();
}

SoundEvent AudioSystem::PlayEvent(const std::string& name) {
	unsigned int retID = 0;
	auto iter = mEvents.find(name);
	if (iter != mEvents.end()) {
		// create instance of event
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event) {
			// start the event instance
			event->start();
			// get the next id, and add to map
			sNextID++;
			retID = sNextID;
			mEventInstances.emplace(retID, event);
		}
	}
	return SoundEvent(this, retID);
}

void AudioSystem::Update(float deltaTime) {
	// find any stopped event instances
	std::vector<unsigned int> done;
	for (auto& iter : mEventInstances) {
		FMOD::Studio::EventInstance* e = iter.second;
		// get the state of this event
		FMOD_STUDIO_PLAYBACK_STATE state;
		e->getPlaybackState(&state);
		if (state == FMOD_STUDIO_PLAYBACK_STOPPED) {
			// release the event add id to done
			e->release();
			done.emplace_back(iter.first);
		}
	}

	// remove done event instances from map
	for (auto id : done) {
		mEventInstances.erase(id);
	}

	// update FMOD
	mSystem->update();
}

namespace
{
	FMOD_VECTOR VecToFMOD(const Vector3& in) {
		// convert from our coordinates (+x forward, +y right, +z up) 
		// to FMOD (+z forward, +x right, +y up)
		FMOD_VECTOR v;
		v.x = in.y;
		v.y = in.z;
		v.z = in.x;
		return v;
	}
}

void AudioSystem::SetListener(const Matrix4& viewMatrix) {
	// invert the view matrix to get the correct vectors
	Matrix4 invView = viewMatrix;
	invView.Invert();
	FMOD_3D_ATTRIBUTES listener;
	// set position, forward, up
	listener.position = VecToFMOD(invView.GetTranslation());
	// in the inverted view, third row is forward
	listener.forward = VecToFMOD(invView.GetZAxis());
	// in the inverted view, second row is up
	listener.up = VecToFMOD(invView.GetYAxis());
	// set velocity to zero (fix it using Doppler effect)
	listener.velocity = { 0.0f, 0.0f, 0.0f };
	// send to FMOD
	mSystem->setListenerAttributes(0, &listener);
}

float AudioSystem::GetBusVolume(const std::string& name) const {
	float retVal = 0.0f;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end()) {
		iter->second->getVolume(&retVal);
	}
	return retVal;
}

bool AudioSystem::GetBusPaused(const std::string& name) const {
	bool retVal = false;
	const auto iter = mBuses.find(name);
	if (iter != mBuses.end()) {
		iter->second->getPaused(&retVal);
	}
	return retVal;
}

void AudioSystem::SetBusVolume(const std::string& name, float volume) {
	auto iter = mBuses.find(name);
	if (iter != mBuses.end()) {
		iter->second->setVolume(volume);
	}
}

void AudioSystem::SetBusPaused(const std::string& name, bool pause) {
	auto iter = mBuses.find(name);
	if (iter != mBuses.end()) {
		iter->second->setPaused(pause);
	}
}

FMOD::Studio::EventInstance* AudioSystem::GetEventInstance(unsigned int id) {
	FMOD::Studio::EventInstance* event = nullptr;
	auto iter = mEventInstances.find(id);
	if (iter != mEventInstances.end()) {
		event = iter->second;
	}
	return event;
}