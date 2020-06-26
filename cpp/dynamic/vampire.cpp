#include "vampire.h"

Vampire::Vampire(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    soundDie =      VAMPIRE_DIE;

    health =        VAMPIRE_HEALTH;
    maxHealth =     VAMPIRE_MAX_HEALTH;
    noclip =        VAMPIRE_NOCLIP;
    flying =        VAMPIRE_FLYING;
    state =         VAMPIRE_STATE;
    type =          VAMPIRE_TYPE;
    alive =         VAMPIRE_ALIVE;
    obstacle =      VAMPIRE_OBSTACLE;
    destructable =  VAMPIRE_DESTRUCTABLE;
    direction =     VAMPIRE_DIRECTION;
    ticksMove =     VAMPIRE_TICKS_MOVE;
    ticksAttack =   VAMPIRE_TICKS_ATTACK;
    damageAttack =  VAMPIRE_DAMAGE;
}


void Vampire::Do()
{
    Dynamic::Do();
    if(alive)
    {
        double playerDist = std::sqrt(std::pow((double)mapContainer->player->GetX() - x, 2) + std::pow((double)mapContainer->player->GetY() - y, 2));
        if(playerDist < triggerRange && state == 1 && mapContainer->player->IsAlive())
        {
            soundCommand->PlaySound(VAMPIRE_ENEMY_DETECTED, this);
            state = 2;
        }
        
        if(state == 2)
        {
            for(int i = 0; i < 4; i++)
            {
                if(CheckLine((Direction) i))
                {
                    int fireballX = x;
                    int fireballY = y;
                    Attack(fireballX, fireballY, (Direction) i);
                }
                else
                {
                    if(abs((int)mapContainer->player->GetX() - (int)x) <= abs((int)mapContainer->player->GetY() - (int)y))
                    {
                        if(mapContainer->player->GetX() > x)
                            Move(1, 0);
                        else if(mapContainer->player->GetX() < x)
                            Move(-1, 0);
                        
                    }
                    else if(abs((int)mapContainer->player->GetX() - (int)x) >= abs((int)mapContainer->player->GetY() - (int)y))
                    {
                        if(mapContainer->player->GetY() > y)
                            Move(0, 1);
                        else if(mapContainer->player->GetY() < y)
                            Move(0, -1);
                    }     
                }
                
            }
        }
    }
    else
    {
        state = 3;
        obstacle = false;
    }
    

    sprite = state;
}

bool Vampire::CheckLine(Direction direction)
{
    int lineX = this->x;
    int lineY = this->y;
    for(int i = 0; i < fireRange; i++)
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


bool Vampire::Attack(uint16_t x, uint16_t y, Direction direction)
{
    if((SDL_GetTicks() > ticksAttackNext && SDL_GetTicks() >= ticksMoveNext || ticksAttack == 0) && alive)
    {
        Fireball* fireball = new Fireball("", mapContainer, soundCommand);
        fireball->SetY(y);
        fireball->SetState(1);
        fireball->SetType(FIREBALL);
        fireball->SetMaxHealth(100);
        fireball->SetHealth(100);
        fireball->SetSpeed(200);
        fireball->SetX(x);
        fireball->SetDirection(direction);
        
        mapContainer->dynamicList.push_front(fireball);

        ticksAttackNext = SDL_GetTicks() + ticksAttack;
        return true;
    }
    else
        return false;    
}
