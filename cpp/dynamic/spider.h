// spider.h

#pragma once
#include <cmath>
#include "dynamic.h"
#include "player.h"

enum
{
    SPIDER_STATE_NONE,
    SPIDER_STATE_NORMAL,
    SPIDER_STATE_ANGRY,
    SPIDER_STATE_DEAD
};

class Spider : public Dynamic
{
    private:

    public:
        Spider(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        //bool Attack(uint32_t x, uint32_t y, int damage);
        void Do() override;
};