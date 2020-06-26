#include "fireball.h"

Fireball::Fireball(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    soundDie =          FIREBALL_HIT;

    health =            FIREBALL_HEALTH;
    maxHealth =         FIREBALL_MAX_HEALTH;
    noclip =            FIREBALL_NOCLIP;
    flying =            FIREBALL_FLYING;
    state =             FIREBALL_STATE;
    type =              FIREBALL_TYPE;
    alive =             FIREBALL_ALIVE;
    obstacle =          FIREBALL_OBSTACLE;
    destructable =      FIREBALL_DESTRUCTABLE;
    direction =         FIREBALL_DIRECTION;
    ticksMove =         FIREBALL_TICKS_MOVE;
    ticksAttack =       FIREBALL_TICKS_ATTACK;
    damageAttack =      FIREBALL_DAMAGE;
    
}

void Fireball::Do()
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
    if(state == 1)
    {
        sprite = 1;
        Move(moveX, moveY);

        if((CheckObstacle(x, y)))
        {
            Attack(x, y, 50);
            soundCommand->PlaySound(soundDie, this);

            state = 2;
            ticksMorphNext = SDL_GetTicks();
        }
    }
    if(SDL_GetTicks() >= ticksMorphNext && state == 2)
    {
        Uint32 ticksOver = SDL_GetTicks() - ticksMorphNext;
        sprite = (sprite + (1 + ticksOver / ticksMorph)) % 5;

        if(sprite == 0)
            state = 0;
            
        ticksMorphNext = SDL_GetTicks() + ticksMorph - ticksOver % ticksMorph;
    }
}