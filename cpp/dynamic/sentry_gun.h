#pragma once
#include "dynamic.h"
#include "player.h"
#include "fireball.h"

class SentryGun : public Dynamic
{
    private:
        uint16_t range = 10;

        bool CheckLine(int direction);

    public:
        SentryGun(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        //void GetUsed() override;
        void Do() override;
        bool Attack(uint16_t x, uint16_t y, Direction direction);
};