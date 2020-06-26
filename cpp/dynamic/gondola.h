// gondola.h

#pragma once
#include "dynamic.h"
#include "player.h"
#include <cmath>

#define GONDOLA_MAP_MUSIC 66

class Gondola : public Dynamic
{
    private:
    int range = 5;
    bool found = false;
    uint8_t music = 0;
    public:
        Gondola(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        void Do() override;
        void SetMusic(uint8_t music);
        uint8_t GetMusic();

        std::vector<std::string> CreateConfig() override;
        std::map<uint16_t, std::string> GetValues() override;
        void SetValues(std::map<uint16_t, std::string> values) override;
        void PrintValues() override;
};