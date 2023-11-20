#pragma once
#include "json.h"
#include <fmod.hpp>
#include <fmod_common.h>
#include <string>

enum class soundType
{
	soundEffect = 0,
	song,
};

class sound
{
public:
	//Ctors/dtors
	sound();
	sound(const char* soundName);
	~sound();

	void play(); //Play a sound
	void pause(); //Pause a sound
	void resume(); //Resume sound if paused
	void setSound(const char* soundName); //Used to set sound to specfic sound (of name 'soundName')

	//Get/Set
	std::string getName();
	std::string getLocation();
	soundType getSoundType();
	FMOD::Sound* getFmodSound();

	void forceQuit(); //Forces sound to stop and be free'd from mem
private:
	std::string soundName_; //Sound name
	std::string soundLoc_; //Sound loc location
	soundType sType_; //Sound type (song or effect)
	FMOD::Sound* fmodSound; //actual sound 'file'

	soundType readSoundType(std::string soundString); //read if sound is song or effect
	FMOD_MODE readFMOD_MODE(soundType sType); //read if sound is song or effect (for FMOD)
};
