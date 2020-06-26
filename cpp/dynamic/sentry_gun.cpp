// remotedoor.cpp
#include "sentry_gun.h"

SentryGun::SentryGun(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    health =        SENTRYGUN_HEALTH;
    maxHealth =     SENTRYGUN_MAX_HEALTH;
    noclip =        SENTRYGUN_NOCLIP;
    flying =        SENTRYGUN_FLYING;
    state =         SENTRYGUN_STATE;
    type =          SENTRYGUN_TYPE;
    alive =         SENTRYGUN_ALIVE;
    obstacle =      SENTRYGUN_OBSTACLE;
    destructable =  SENTRYGUN_DESTRUCTABLE;
    direction =     SENTRYGUN_DIRECTION;
    ticksMove =     SENTRYGUN_TICKS_MOVE;
    ticksAttack =   SENTRYGUN_TICKS_ATTACK;
    damageAttack =  SENTRYGUN_DAMAGE;
}

bool SentryGun::CheckLine(int direction)
{
    int16_t lineX = this->x;
    int16_t lineY = this->y;
    for(uint16_t i = 0; i < range; i++)
    {
        switch(direction)
        {
            case NORTH:
                lineY--;
                break;
            case EAST:
                lineX++;
                break;
            case SOUTH:
                lineY++;
                break;
            case WEST:
                lineX--;
                break;
        }
        if(mapContainer->player->GetX() == lineX && mapContainer->player->GetY() == lineY)
            return true;
        else if(CheckObstacle(lineX, lineY))
            return false;
    }
    return false;
}

void SentryGun::Do()
{
    Dynamic::Do();

    if(state == 2)
        if(CheckLine(direction))
            state = 3;
    
    if(state == 3)
    {
        uint16_t fireballX = x;
        uint16_t fireballY = y;
        Attack(fireballX, fireballY, direction);
    }

    sprite = 1;
}

bool SentryGun::Attack(uint16_t x, uint16_t y, Direction direction)
{
    if((SDL_GetTicks() > ticksAttackNext && SDL_GetTicks() >= ticksMoveNext || ticksAttack == 0) && alive)
    {
        Fireball* fireball = new Fireball("", mapContainer, soundCommand);
        fireball->SetY(y);
        fireball->SetX(x);
        fireball->SetDirection(direction);
        
        mapContainer->dynamicList.push_front(fireball);

        ticksAttackNext = SDL_GetTicks() + ticksAttack;
        return true;
    }
    else
        return false;    
}