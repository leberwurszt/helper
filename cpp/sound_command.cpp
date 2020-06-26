#include "sound_command.h"

    SoundCommand::SoundCommand()
    {}

    SoundCommand::~SoundCommand()
    {}

    // Play global sound without sender
    bool SoundCommand::PlaySound(uint16_t index)
    {
        soundList.push_back(std::pair<uint16_t, Dynamic*>(index, NULL));
        return true;
    }

    // Play sound with specified sender
    bool SoundCommand::PlaySound(uint16_t index, Dynamic* sender)
    {
        soundList.push_back(std::pair<uint16_t, Dynamic*>(index, sender));
        return true;
    }

    // set music to play
    bool SoundCommand::PlayMusic(uint8_t index)
    {
        music = index;
        return true;
    }

    // return music to play
    uint8_t SoundCommand::PopMusic()
    {
        uint8_t musicReturn = music;
        music = 0;
        return musicReturn;
    }

    // Destination of sound (for sound distance)
    void SoundCommand::SetDest(uint16_t x, uint16_t y)
    {
        this->destX = x;
        this->destY = y;
    }

    // Return sound from list
    std::pair<uint16_t, Dynamic*> SoundCommand::PopSound()
    {
        if(soundList.size() > 0)
        {
            std::pair<uint16_t, Dynamic*> sound = soundList.back();
            soundList.pop_back();
            return sound;
        }
        else
            return std::pair<uint16_t, Dynamic*>(0, NULL);
        
    }