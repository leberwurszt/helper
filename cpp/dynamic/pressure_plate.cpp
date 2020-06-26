// pressure_plate.cpp

#include "pressure_plate.h"

PressurePlate::PressurePlate(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    health =            PRESSUREPLATE_HEALTH;
    maxHealth =         PRESSUREPLATE_MAX_HEALTH;
    noclip =            PRESSUREPLATE_NOCLIP;
    flying =            PRESSUREPLATE_FLYING;
    state =             PRESSUREPLATE_STATE;
    type =              PRESSUREPLATE_TYPE;
    alive =             PRESSUREPLATE_ALIVE;
    obstacle =          PRESSUREPLATE_OBSTACLE;
    destructable =      PRESSUREPLATE_DESTRUCTABLE;
    direction =         PRESSUREPLATE_DIRECTION;
    ticksMove =         PRESSUREPLATE_TICKS_MOVE;
    ticksAttack =       PRESSUREPLATE_TICKS_ATTACK;
    damageAttack =      PRESSUREPLATE_DAMAGE;
}

void PressurePlate::Do()
{
    if(CheckObstacle(x, y))
    {
        state = 2;

        if(!switched)
        {
            soundCommand->PlaySound(PRESSURE_PLATE_USE, this);
            TriggerIt();
            
            switched = true;
        }
    }
    else
    {
        state = 1;
        if(switched)
        {
            soundCommand->PlaySound(PRESSURE_PLATE_USE, this);
            TriggerIt();
            switched = false;
        }
    }
    sprite = state;
}

void PressurePlate::TriggerIt()
{
    for(Trigger* trigger : mapContainer->triggerList)
    {
        if(trigger->GetX() == x && trigger->GetY() == y)
            trigger->Activate();
    }
}