// npc.cpp

#include "npc.h"

Npc::Npc(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    soundDie =      NPC_DIE;

    health =        NPC_HEALTH;
    maxHealth =     NPC_MAX_HEALTH;
    noclip =        NPC_NOCLIP;
    flying =        NPC_FLYING;
    state =         NPC_STATE;
    type =          NPC_TYPE;
    alive =         NPC_ALIVE;
    obstacle =      NPC_OBSTACLE;
    destructable =  NPC_DESTRUCTABLE;
    direction =     NPC_DIRECTION;
    ticksMove =     NPC_TICKS_MOVE;
    ticksAttack =   NPC_TICKS_ATTACK;
    damageAttack =  NPC_DAMAGE;
}


void Npc::Do()
{
    Dynamic::Do();

    if(!alive)
        state = NPC_STATE_DEAD;

    double playerDist = std::sqrt(std::pow((double)mapContainer->player->GetX() - x, 2) + std::pow((double)mapContainer->player->GetY() - y, 2));
    
    switch(state)
    {
        case NPC_STATE_NORMAL:
            if(playerDist < 5 && mapContainer->player->IsAlive())
            {
                soundCommand->PlaySound(NPC_ENEMY_DETECTED, this);
                state = NPC_STATE_ANGRY;
            }
            sprite = 1;
            break;

        case NPC_STATE_ANGRY: // angry, approach to player
            if(mapContainer->player->GetState() == 2) // If player is Honkler, NPC gets in panic
                state = NPC_STATE_PANIC;
            else if(!mapContainer->player->IsAlive()) // If player is dead, NPC is normal again
                state = NPC_STATE_NORMAL;
            else
            {
                if(mapContainer->player->GetX() > x)
                    Move(1, 0);
                if(mapContainer->player->GetX() < x)
                    Move(-1, 0);
                if(mapContainer->player->GetY() > y)
                    Move(0, 1);
                if(mapContainer->player->GetY() < y)
                    Move(0, -1);

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
            sprite = 2;
            break;

        case NPC_STATE_DEAD:
            sprite = 3;
            break;

        case NPC_STATE_PANIC:
            if(mapContainer->player->GetState() != 2)
                state = NPC_STATE_ANGRY;
            else
            {
                if(mapContainer->player->GetX() > x)
                    Move(-1,0);
                else if(mapContainer->player->GetX() < x)
                    Move(1,0);
                if(mapContainer->player->GetY() > y)
                    Move(0,-1);
                else if(mapContainer->player->GetY() < y)
                    Move(0,1);
            }
            sprite = 4;
            break;

        default:
            break;
    }
}