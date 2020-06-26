// spurdo.h

#pragma once
#include <iostream>
#include <cmath>
#include "dynamic.h"

#define SPURDO_MOVE_X ((hangingWall < 3) * (hangingWall - 1))
#define SPURDO_MOVE_Y ((hangingWall > 0) * (hangingWall - 2))
#define SPURDO_WALL_LOST_NEW_WALL ((hangingWall < 3) * (hangingWall + 1))
#define SPURDO_OBSTACLE_NEW_WALL ((hangingWall - 1) + 4 * (hangingWall == 0))

class Spurdo : public Dynamic
{
    private:
        Uint32 ticksMorph = 0;
        Uint32 ticksMorphNext = 0;
        int8_t hangingWall = 4;

        int8_t spurdoMoveX = 0;
        int8_t spurdoMoveY = 0;

        bool destructable = false;
        //Uint32 rollTime;

    public:
        Spurdo(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        void SetTicksMorph(Uint32 ticksMorph);
        void Do() override;
};