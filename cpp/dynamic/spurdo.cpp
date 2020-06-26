// spurdo.cpp

#include "spurdo.h"


Spurdo::Spurdo(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    sprite =            1;
    ticksMorph =        75;
    ticksMorphNext =    SDL_GetTicks() + ticksMorph;

    soundDie =          SPURDO_DIE;

    health =            SPURDO_HEALTH;
    maxHealth =         SPURDO_MAX_HEALTH;
    noclip =            SPURDO_NOCLIP;
    flying =            SPURDO_FLYING;
    state =             SPURDO_STATE;
    type =              SPURDO_TYPE;
    alive =             SPURDO_ALIVE;
    obstacle =          SPURDO_OBSTACLE;
    destructable =      SPURDO_DESTRUCTABLE;
    direction =         SPURDO_DIRECTION;
    ticksMove =         SPURDO_TICKS_MOVE;
    ticksAttack =       SPURDO_TICKS_ATTACK;
    damageAttack =      SPURDO_DAMAGE;
    
}

void Spurdo::SetTicksMorph(Uint32 ticksMorph)
{
    this->ticksMorph = ticksMorph;
}

void Spurdo::Do()
{
    Dynamic::Do();
    // Spurdish Spurdo Spärde initalization depending on direction of next wall
    if(hangingWall == 4)
    {
        if(CheckObstacle(x - 1, y))
            hangingWall = 3;
        if(CheckObstacle(x + 1, y))
            hangingWall = 1;
        if(CheckObstacle(x, y - 1))
            hangingWall = 0;
        if(CheckObstacle(x, y + 1))
            hangingWall = 2;
    }
    if(alive && state == 1)
    {
        // Spurdish Spurdo Spärde movement, he's rolling on the wall!
        if(SDL_GetTicks() >= ticksMoveNext)
        {
            spurdoMoveX = SPURDO_MOVE_X;
            spurdoMoveY = SPURDO_MOVE_Y;

            Move(spurdoMoveX, spurdoMoveY);
            if (!CheckObstacle(x - spurdoMoveY, y + spurdoMoveX))
                hangingWall = SPURDO_WALL_LOST_NEW_WALL;
            else if (CheckObstacle(x + spurdoMoveX, y + spurdoMoveY))
                hangingWall = SPURDO_OBSTACLE_NEW_WALL;
            
        }

        // Spurdish Spurdo Spärde roll animation
        if(SDL_GetTicks() >= ticksMorphNext)
        {
            Uint32 ticksOver = SDL_GetTicks() - ticksMorphNext;
            sprite = (sprite + (1 + ticksOver / ticksMorph)) % 15;
            sprite++;
            ticksMorphNext = SDL_GetTicks() + ticksMorph - ticksOver % ticksMorph;
        }
    }
}