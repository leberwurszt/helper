#pragma once
#include <SDL2/SDL_mixer.h>
#include <cmath>
#include <vector>

class Dynamic;

class SoundCommand
{
    private:
        uint16_t destX = 0;
        uint16_t destY = 0;

        std::vector<std::pair<uint16_t, Dynamic*>> soundList;
        uint8_t music = 0;

    public:
        SoundCommand();
        ~SoundCommand();

        bool PlaySound(uint16_t index);
        bool PlayMusic(uint8_t index);

        bool PlaySound(uint16_t index, Dynamic* sender);
        void SetDest(uint16_t x, uint16_t y);
        
        std::pair<uint16_t, Dynamic*> PopSound();
        uint8_t PopMusic();
};