// spider.cpp

#include "spider.h"

Spider::Spider(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    soundDie =      SPIDER_DIE;

    health =        SPIDER_HEALTH;
    maxHealth =     SPIDER_MAX_HEALTH;
    noclip =        SPIDER_NOCLIP;
    flying =        SPIDER_FLYING;
    state =         SPIDER_STATE;
    type =          SPIDER_TYPE;
    alive =         SPIDER_ALIVE;
    obstacle =      SPIDER_OBSTACLE;
    destructable =  SPIDER_DESTRUCTABLE;
    direction =     SPIDER_DIRECTION;
    ticksMove =     SPIDER_TICKS_MOVE;
    ticksAttack =   SPIDER_TICKS_ATTACK;
    damageAttack =  SPIDER_DAMAGE;
}


void Spider::Do()
{
    Dynamic::Do();

    if(!alive)
        state = SPIDER_STATE_DEAD;

    double playerDist = std::sqrt(std::pow((double)mapContainer->player->GetX() - x, 2) + std::pow((double)mapContainer->player->GetY() - y, 2));

    switch(state)
    {
        case SPIDER_STATE_NORMAL:
        
            if(playerDist < 5 && mapContainer->player->IsAlive())
            {
                soundCommand->PlaySound(SPIDER_ENEMY_DETECTED, this);
                state = 2;
            }
            sprite = 1;
            break;

        case SPIDER_STATE_ANGRY:

            if(!mapContainer->player->IsAlive()) // If player is dead, NPC is normal again
                state = SPIDER_STATE_NORMAL;
            else
            {
                if(mapContainer->player->GetX() > x)
                    Move(1,0);
                if(mapContainer->player->GetX() < x)
                    Move(-1,0);
                if(mapContainer->player->GetY() > y)
                    Move(0,1);
                if(mapContainer->player->GetY() < y)
                    Move(0,-1);


                // If player is 1 square away, attack
                if(mapContainer->player->GetY() == y)
                {

                    if(mapContainer->player->GetX() == x - 1)
                        Attack(x - 1, y, damageAttack);
                    else if(mapContainer->player->GetX() == x + 1)
                        Attack(x + 1, y, damageAttack);
                }
                else if(mapContainer->player->GetX() == x)
                {
                    if(mapContainer->player->GetY() == y - 1)
                        Attack(x, y - 1, damageAttack);
                    else if(mapContainer->player->GetY() == y + 1)
                        Attack(x, y + 1, damageAttack);
                }
            }

            if(GetMovementPercentage() > .5)
                sprite = 2;
            else if(GetAttackPercentage() > .5)
                sprite = 3;
            else
                sprite = 1;
            break;

        case SPIDER_STATE_DEAD:
            sprite = 4;
            break;

        default:
            break;
    }
}