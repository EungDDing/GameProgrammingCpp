#pragma once
#include <unordered_map>
#include <string>
#include "SoundEvent.h"
#include "Math.h"

// forward declarations to avoid including FMOD header
namespace FMOD 
{
	class System;
	namespace Studio 
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	};
};

class AudioSystem {
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void Shutdown();
	
	// load/unload banks
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	SoundEvent PlayEvent(const std::string& name);

	void Update(float deltaTime);

	// for Positional audio
	void SetListener(const Matrix4& viewMatrix);
	// control buses
	float GetBusVolume(const std::string& name) const;
	bool GetBusPaused(const std::string& name) const;
	void SetBusVolume(const std::string& name, float volume);
	void SetBusPaused(const std::string& name, bool pause);
protected:
	friend class SoundEvent;
	FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
private:
	// track the next ID to use for event instance
	static unsigned int sNextID;

	class Game* mGame;
	// map of loaded banks
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// map of event name to EventDescription
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
	// map of event id to EventInstance
	std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
	// map of buses
	std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;
	// FMOD studio system
	FMOD::Studio::System* mSystem;
	// FMOD Low-level system (in case needed)
	FMOD::System* mLowLevelSystem;
};