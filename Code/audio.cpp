#include "audio.h"

//Ctors/Dtors
//Creates the audio system
//Uses json files 'audio' and  'sounds'
audio::audio() :jsonAudio_("audio"), jsonSoundList("sounds")
{
	this->system->createSoundGroup("muted", &muted);
	this->system->createSoundGroup("unmuted", &unmuted);
	this->channel = NULL;
	FMOD::System_Create(&this->system);      // Create the main system object.
	this->system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	this->system->createChannelGroup("groupMusic", &this->channelGroupMusic); //FMOD create group for Music
	this->system->createChannelGroup("groupSFX", &this->channelGroupSFX); //FMOD create group for SFX
	this->groupMusicVol_ = jsonAudio_.readJSONValue<int>("groupMusicVol") / 100.0f; //Get volume of Music group
	this->groupSFXVol_ = jsonAudio_.readJSONValue<int>("groupSFXVol") / 100.0f; //Get volume of SFX group
	setChannelGroupVol(this->channelGroupSFX, this->groupSFXVol_); //Set Volume for SFX group
	setChannelGroupVol(this->channelGroupMusic, this->groupMusicVol_); //Set Volume for Music group
	muted->setVolume(0.0f);//Mutes all sounds in the muted soundgroup;
	unmuted->setVolume(1.0f);//Plays all sounds in unmuted soundgroup at 100%;
	lastVolume = 0.0f;
}

audio::~audio()
{
	for (int i = 0; i < sounds_.size(); i++) // Go through all sounds in list and delete them
	{
		if (sounds_[i])
		{
			delete sounds_[i];
		}
	}
	sounds_.clear();
	//Free FMOD systems
	this->system->release();
	this->channelGroupSFX->release();
	this->channelGroupMusic->release();
}

int audio::audioUpdate()
{
	if (this->system->update() != FMOD_RESULT::FMOD_OK)
	{
		LogError("ERROR UPDATING AUDIO!");
		return -1;
	}
	return 0;
}

//Toggle mute for Channel group
//Returns 1 if the channel is now muted
//Returns 0 if the channel is now unmuted
int audio::toggleMuteChannelGroup(FMOD::ChannelGroup* channelGroup)
{
	bool* isMute = NULL;
	if (channelGroup)
	{
		channelGroup->getMute(isMute);
		channelGroup->setMute(!isMute);
		if (!isMute == true)
		{
			return 0;
		}
		else
		{
			return  1;
		}
	}
	LogError("ERROR with trying to mute Channel!,Channel is not valid!");
	return -1;
}

//set/get Vol
int audio::setChannelGroupVol(FMOD::ChannelGroup* channelGroup, float vol)
{
	if (vol > 1.0f || vol < 0.0f)
	{
		LogError("ERROR with changing volume! Value must be between 1.0f-0.0f");
		return -1;
	}
	int vol2 = static_cast<int>(vol * 100);
	if (channelGroup)
	{
		channelGroup->setVolume(vol);
		if (channelGroup == this->channelGroupMusic)
		{
			lastVolume = this->groupMusicVol_;
			this->groupMusicVol_ = vol;
		}
		else if (channelGroup == this->channelGroupSFX)
		{
			lastVolume = this->groupSFXVol_;
			this->groupSFXVol_ = vol;
		}
		return 0;
	}
	LogError("ERROR with trying to mute Channel!,Channel is not valid!");
	return -1;
}

float audio::getChannelGroupVol(FMOD::ChannelGroup* channelGroup)
{
	if (channelGroup)
	{
		if (channelGroup == this->channelGroupMusic)
		{
			return this->groupMusicVol_;
		}
		else if (channelGroup == this->channelGroupSFX)
		{
			return this->groupSFXVol_;
		}
		LogError("ERROR trying to get volume Channel!");
		return -1;
	}
	LogError("ERROR Channel does not exist!");
	return -2;
}

//playSound
void audio::playSound(const char* soundName)
{
	//audioUpdate();
	for (int i = 0; i < sounds_.size(); i++)
	{
		if (sounds_[i])
		{
			if (sounds_[i]->getName() == soundName)
			{
				sounds_[i]->play();
				return;
			}
		}
	}
	LogInfo("Init new sound, '{}'", soundName)
		sound* newSound;
	newSound = new sound;
	newSound->setSound(soundName);
	newSound->play();
	sounds_.push_back(newSound);
}


//playSound
void audio::stopSound(const char* soundName)
{
	//audioUpdate();
	for (int i = 0; i < sounds_.size(); i++)
	{
		if (sounds_[i])
		{
			if (sounds_[i]->getName() == soundName)
			{
				sounds_[i]->forceQuit();
				sounds_[i] = NULL;
				return;
			}
		}
	}
	LogError("Sound '{}' not found! Nothing was stopped!", soundName)
}

bool audio::isChannelPlayingSound(FMOD::ChannelGroup* channelGroup)
{
	bool f = false;;
	channelGroup->isPlaying(&f);
	return f;
}

void audio::pauseSound(const char* soundName)
{
	//audioUpdate();
	for (int i = 0; i < sounds_.size(); i++)
	{
		if (sounds_[i])
		{
			if (sounds_[i]->getName() == soundName)
			{
				sounds_[i]->pause();
				return;
			}
		}
	}
	LogError("Sound '{}' not found! Nothing was stopped!", soundName)
}
void audio::resumeSound(const char* soundName)
{
	//audioUpdate();
	for (int i = 0; i < sounds_.size(); i++)
	{
		if (sounds_[i])
		{
			if (sounds_[i]->getName() == soundName)
			{
				sounds_[i]->resume();
				return;
			}
		}
	}
	LogError("Sound '{}' not found! Nothing was resumed!", soundName)
}
