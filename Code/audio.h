#pragma once
#include <fmod.hpp>
#include <fmod_common.h>
#include <fmod_errors.h>
#include "json.h"
#include "sound.h"

class audio
{
public:
	//Ctors/Dtors
	audio();
	~audio();

	int audioUpdate(); //Updates audio

	//Set/Get
	int toggleMuteChannelGroup(FMOD::ChannelGroup* channelGroup); //Toggles Mute on Soundeffect channel (Song or Effect soundchannel)
	int setChannelGroupVol(FMOD::ChannelGroup* channelGroup, float vol); //Set Soundeffect channel vol (Song or Effect soundchannel)
	float getChannelGroupVol(FMOD::ChannelGroup* channelGroup); //Get Soundeffect channel vol (Song or Effect soundchannel)


	void playSound(const char* soundName); //Play sound (of name)
	void stopSound(const char* soundName); //Stop sound (of name)
	void resumeSound(const char* soundName); //Resume paused sound (of name)
	void pauseSound(const char* soundName); //Pause sound (of name)

	bool isChannelPlayingSound(FMOD::ChannelGroup* channelGroup); //Returns if sound comming from channel (Song or Effect soundchannel)

	//Vars
	FMOD::System* system; //actual FMOD system (see FMOD doc for more info)
	FMOD::Channel* channel; //actual FMOD channel (see FMOD doc for more info)
	FMOD::ChannelGroup* channelGroupSFX; //SFX Channel
	FMOD::ChannelGroup* channelGroupMusic; //Music Channel
	FMOD::SoundGroup* unmuted; //Unmute group
	FMOD::SoundGroup* muted; //Muted group
	JSON jsonSoundList; //JSON list with all sounds
	float lastVolume; //Last volume that has been updated

private:
	JSON jsonAudio_; //JSON file
	std::vector<sound*> sounds_; //List of all sounfs
	float groupSFXVol_; //SFX vol
	float groupMusicVol_; //Music vol
};
