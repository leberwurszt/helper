#include "rolling_stone.h"

RollingStone::RollingStone(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    ticksMorph =        0;
    ticksMorphNext =    SDL_GetTicks() + ticksMorph;

    health =            ROLLINGSTONE_HEALTH;
    maxHealth =         ROLLINGSTONE_MAX_HEALTH;
    noclip =            ROLLINGSTONE_NOCLIP;
    flying =            ROLLINGSTONE_FLYING;
    state =             ROLLINGSTONE_STATE;
    type =              ROLLINGSTONE_TYPE;
    alive =             ROLLINGSTONE_ALIVE;
    obstacle =          ROLLINGSTONE_OBSTACLE;
    destructable =      ROLLINGSTONE_DESTRUCTABLE;
    direction =         ROLLINGSTONE_DIRECTION;
    ticksMove =         ROLLINGSTONE_TICKS_MOVE;
    ticksAttack =       ROLLINGSTONE_TICKS_ATTACK;
    damageAttack =      ROLLINGSTONE_DAMAGE;
}

void RollingStone::Do()
{
    if(state == 1)
        ticksMorphNext = SDL_GetTicks() + ticksMorph;
    if(state == 2)
    {
        int moveX = 0;
        int moveY = 0;

        switch(direction)
        {
            case NORTH:
                moveX = 0;
                moveY = -1;
                break;
            case EAST:
                moveX = 1;
                moveY = 0;
                break;
            case SOUTH:
                moveX = 0;
                moveY = 1;
                break;
            case WEST:
                moveX = -1;
                moveY = 0;
                break;
        }
        if(SDL_GetTicks() >= ticksMoveNext)
            if(GetDynamicByCoord(x + moveX, y + moveY, mapContainer))
            {
                Attack(x + moveX, y + moveY, damageAttack);
                Dynamic* dynamic = GetDynamicByCoord(x + moveX, y + moveY, mapContainer);
                
                if(!dynamic->IsAlive())
                {
                    mapContainer->map->SetItem(x + moveX, y + moveY, 9);
                }
                else if(CheckObstacle(x + moveX, y + moveY))
                {
                    soundCommand->PlaySound(ROCK_HIT, this);
                    state = 1;
                }
                
            }
            else if(CheckObstacle(x + moveX, y + moveY))
            {
                soundCommand->PlaySound(ROCK_HIT, this);
                state = 1;
            }

        //std::cout << Move(moveX, moveY) << std::endl;
        if(Move(moveX, moveY))
            soundCommand->PlaySound(ROCK_ROLLING, this);

        if(SDL_GetTicks() >= ticksMorphNext)
        {
            ticksMorph = (ticksMove * M_PI) / 8;
            Uint32 ticksOver = SDL_GetTicks() - ticksMorphNext;
            
            rollAnimationSprite = 1 + (rollAnimationSprite + (ticksOver / ticksMorph)) % 8;
            ticksMorphNext = SDL_GetTicks() + ticksMorph - ticksOver % ticksMorph;
        }
    }
    sprite = rollAnimationSprite;
}