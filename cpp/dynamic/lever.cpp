// lever.cpp

#include "lever.h"

Lever::Lever(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    health =            LEVER_HEALTH;
    maxHealth =         LEVER_MAX_HEALTH;
    noclip =            LEVER_NOCLIP;
    flying =            LEVER_FLYING;
    state =             LEVER_STATE;
    type =              LEVER_TYPE;
    alive =             LEVER_ALIVE;
    obstacle =          LEVER_OBSTACLE;
    destructable =      LEVER_DESTRUCTABLE;
    direction =         LEVER_DIRECTION;
    ticksMove =         LEVER_TICKS_MOVE;
    ticksAttack =       LEVER_TICKS_ATTACK;
    damageAttack =      LEVER_DAMAGE;
}

void Lever::Do()
{
    sprite = state;
}

bool Lever::GetUsed()
{
    soundCommand->PlaySound(LEVER_USE, this);
    if(state == 1)
        state = 2;
    else
        state = 1;

    for(Trigger* trigger : mapContainer->triggerList)
    {
        if(trigger->GetX() == x && trigger->GetY() == y)
            trigger->Activate();
    }
    return true;
}
