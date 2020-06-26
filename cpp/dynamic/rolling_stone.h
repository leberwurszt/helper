#pragma once
#include "dynamic.h"

class RollingStone : public Dynamic
{
    private:
    Uint32 ticksMorph = 0;
    Uint32 ticksMorphNext = 0;
    int rollAnimationSprite = 1;
    Uint32 ticksAttack = 0;

    public:
        RollingStone(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        //void GetUsed() override;
        void Do() override;
};