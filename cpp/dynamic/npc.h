// npc.h

#pragma once
#include <cmath>
#include "dynamic.h"
#include "player.h"

enum
{
    NPC_STATE_NULL,
    NPC_STATE_NORMAL,
    NPC_STATE_ANGRY,
    NPC_STATE_DEAD,
    NPC_STATE_PANIC
};

class Npc : public Dynamic
{
    private:

    public:
        Npc(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        //bool Attack(uint32_t x, uint32_t y, int damage);
        void Do() override;
};