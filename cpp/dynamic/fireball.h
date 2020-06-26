#pragma once
#include "dynamic.h"
#include "player.h"

class Fireball:public Dynamic
{
    private:
    Uint32 ticksMorph = 25;
    Uint32 ticksMorphNext = 0;
    bool destruct = false;

    public:
        Fireball(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        //void GetUsed() override;
        void Do() override;
};