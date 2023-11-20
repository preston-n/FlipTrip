#include "sound.h"
#include "Trace.h"
#include "audio.h"

extern audio* audioSystem; //Global audio system so sounds can be played anywhere

//ctors/dtors
sound::sound(const char* soundName)
{
	this->soundName_ = soundName;
	std::string soundString;
	soundString = audioSystem->jsonSoundList.readJSONValue<const char*>(soundName_.c_str(), "soundType"); // MUST BE CALLED LIKE THIS OR ELSE ERROR WITH STRING
	this->sType_ = readSoundType(soundString);
	this->soundLoc_ = audioSystem->jsonSoundList.readJSONValue<const char*>(soundName_.c_str(), "soundLocation");
	audioSystem->system->createSound(this->soundLoc_.c_str(), readFMOD_MODE(this->sType_), 0, &this->fmodSound);
	this->fmodSound->setSoundGroup(audioSystem->unmuted);
}

sound::sound()
{
	LogWarning("Deafult sound made, must be set soon!");
	this->sType_ = soundType::soundEffect;
	this->soundName_ = "error";
	this->soundLoc_ = "error";
}

sound::~sound()
{
	if (fmodSound)
	{
		this->fmodSound->release();
		this->fmodSound = nullptr;
	}
}


//forceQuit
void sound::forceQuit()
{
	if (this->fmodSound)
	{
		this->fmodSound->release();
		this->fmodSound = nullptr;
	}
}


//play
void sound::play()
{
	if (this->fmodSound)
	{
		FMOD::ChannelGroup* cGroup;
		switch (this->sType_)
		{
		case soundType::song:
			cGroup = audioSystem->channelGroupMusic;
			break;
		case soundType::soundEffect:
			cGroup = audioSystem->channelGroupSFX;
			break;
		default:
			cGroup = audioSystem->channelGroupSFX;
			this->sType_ = soundType::soundEffect;
			LogWarning("{} Sound Possible Error with soundType DEFAULTING TO SFX!", this->soundName_.c_str());
			break;
		}
		audioSystem->system->playSound(this->fmodSound, cGroup, false, &audioSystem->channel);
	}
	else
	{
		LogError("{} Sound Does Not Exist!", this->soundName_.c_str());
	}
}


void sound::pause()
{
	if (this->fmodSound)
	{
		this->fmodSound->setMusicSpeed(0.00001f);
		this->fmodSound->setSoundGroup(audioSystem->muted);
	}
	else
	{
		LogError("{} Sound Does Not Exist!", this->soundName_.c_str());
	}
}

void sound::resume()
{
	if (this->fmodSound)
	{
		this->fmodSound->setMusicSpeed(1.0f);
		this->fmodSound->setSoundGroup(audioSystem->unmuted);
	}
	else
	{
		LogError("{} Sound Does Not Exist!", this->soundName_.c_str());
	}
}

// get/set
std::string sound::getName()
{
	return this->soundName_;
}

void sound::setSound(const char* soundName)
{
	this->soundName_ = soundName;
	std::string soundString;
	soundString = audioSystem->jsonSoundList.readJSONValue<const char*>(soundName_.c_str(), "soundType"); 
	this->sType_ = readSoundType(soundString);
	this->soundLoc_ = audioSystem->jsonSoundList.readJSONValue<const char*>(soundName_.c_str(), "soundLocation");
	audioSystem->system->createSound(this->soundLoc_.c_str(), readFMOD_MODE(this->sType_), 0, &this->fmodSound);
}

std::string sound::getLocation()
{
	return this->soundLoc_;
}

soundType sound::getSoundType()
{
	return this->sType_;
}

FMOD::Sound* sound::getFmodSound()
{
	return this->fmodSound;
}


//readSounds
soundType sound::readSoundType(std::string soundString)
{
	if (soundString == "Song" || soundString == "song")
	{
		return soundType::song;
	}
	else if (soundString == "sound" || soundString == "Sound")
	{
		return soundType::soundEffect;
	}
	else
	{
		LogWarning("ERROR reading soundType, deafulting to SFX");
		return soundType::soundEffect;
	}
}

FMOD_MODE sound::readFMOD_MODE(soundType sType)
{
	switch (sType)
	{
	case soundType::song:
		return FMOD_LOOP_NORMAL;
	case soundType::soundEffect:
		return FMOD_DEFAULT;
	default:
		LogWarning("ERROR reading soundType, deafulting to SFX");
		return FMOD_DEFAULT;
	}
}
