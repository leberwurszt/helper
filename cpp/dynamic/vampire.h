#pragma once

#include "dynamic.h"
#include "player.h"
#include "fireball.h"

class Vampire : public Dynamic
{
    private:
    int fireRange = 10;
    int triggerRange = 5;

    public:

        Vampire(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        void Do() override;
        bool Attack(uint16_t x, uint16_t y, Direction direction);
        bool CheckLine(Direction direction);
};