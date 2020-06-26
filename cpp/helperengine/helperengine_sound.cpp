// SOUND FUNCTIONS

#include "helperengine.h"

// play ingame music
bool HelperEngine::PlayMusic(uint8_t musicIndex)
{
    if(musicIndex != 0)
    {
        if(musicIndex != musicPlaying)
        {
            //Mix_VolumeMusic(musicVolume);
            Mix_PlayMusic(music[musicIndex], -1);
            musicPlaying = musicIndex;
            return true;
        }
        else
            return false;
    }
    else
    {
        musicPlaying = 0;
        Mix_HaltMusic();
        return false;
    }    
}

void HelperEngine::SetMusicVolume(uint8_t volume)
{
    musicVolume = volume;
}

// play ingame sounds
bool HelperEngine::PlaySounds()
{
    Mix_Volume(0,128);
    std::pair<int, Dynamic*> s = std::pair<int, Dynamic*>(0, NULL); // declare s
    int sf = -1;

    while(sf != 0)
    {
        s = soundCommand->PopSound();
        sf = s.first;

        if(sf != 0)
        {
            // checks which channel is free and sets channel
            int channel = 0;
                for(int i = 1; i < SOUND_CHANNELS; ++i)
                {
                    if(Mix_Playing(i) == 0)
                    {
                        channel = i;
                        i = SOUND_CHANNELS;
                    }
                }
            

            Dynamic* ss = s.second;

            // if a dynamic is the source, set channel volume depending on distance
            if(ss != NULL)
            {
                double distance = sqrt(pow((int)ss->GetX() - (int)mapContainer->player->GetX(), 2) + pow((int)ss->GetY() - (int)mapContainer->player->GetY(), 2));
                Mix_Volume(channel, (MIX_MAX_VOLUME * 16) / (distance * distance));
            }
            // otherwise, set full volume
            else
                Mix_Volume(channel, MIX_MAX_VOLUME);
            
            // play the sound
            Mix_PlayChannel(channel, sound[s.first], 0 );
        }
    } 
    return true;
}