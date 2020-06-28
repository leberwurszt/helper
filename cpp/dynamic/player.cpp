// player.cpp

#include "player.h"

Player::Player(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    soundDie =      PLAYER_DIE;

    health =        PLAYER_HEALTH;
    maxHealth =     PLAYER_MAX_HEALTH;
    noclip =        PLAYER_NOCLIP;
    flying =        PLAYER_FLYING;
    state =         PLAYER_STATE;
    type =          PLAYER_TYPE;
    alive =         PLAYER_ALIVE;
    obstacle =      PLAYER_OBSTACLE;
    destructable =  PLAYER_DESTRUCTABLE;
    direction =     PLAYER_DIRECTION;
    ticksMove =     PLAYER_TICKS_MOVE;
    ticksAttack =   PLAYER_TICKS_ATTACK;
    damageAttack =  PLAYER_DAMAGE;
}

void Player::Do()
{
    Dynamic::Do();
    if(!alive)
        state = PLAYER_STATE_DEAD;

        switch(state)
        {
            case PLAYER_STATE_NULL:
                sprite = 0;
                break;

            case PLAYER_STATE_NORMAL:
                switch(direction)
                {
                    case NORTH:
                        sprite = 16;
                        break;
                    case EAST:
                        sprite = 4;
                        break;
                    case SOUTH:
                        sprite = 12;
                        break;
                    case WEST:
                        sprite = 8;
                        break;
                }
                sprite += (int) (GetMovementPercentage() * 4);
                break;
            
            case PLAYER_STATE_DEAD:
                sprite = 3;
                break;
            
            case PLAYER_STATE_HONKLER:
                soundCommand->PlayMusic(2);
                ticksAttack = 0;

                Attack(x + 1, y, damageAttack);
                Attack(x - 1, y, damageAttack);
                
                Attack(x, y + 1, damageAttack);
                Attack(x, y - 1, damageAttack);

                if(ticksMorphRemaining <= SDL_GetTicks())
                {
                    state = PLAYER_STATE_NORMAL;
                    ticksAttack = ticksAttackNormal;
                }
                break;

            default:
                break;
        }
}

bool Player::Use(uint16_t x, uint16_t y)
{
    bool success = true;


        for(Dynamic* dynamic : mapContainer->dynamicList)
            if(dynamic->GetX() == x && dynamic->GetY() == y)
                success = dynamic->GetUsed();
                
        if(success)
            ticksAttackNext = SDL_GetTicks() + ticksAttack;

    return success;
}
bool Player::Use(uint16_t x, uint16_t y, Item* item)
{
    bool success = true;
    return success;
}

int Player::GetScore()
{
    return score;
}

void Player::SetScore(int score)
{
    this->score = score;
}

void Player::AddScore(int score)
{
    this->score += score;
}

bool Player::AddToInventory(Item* item)
{
    if(inventory.size() < PLAYER_INVENTORY_MAX_SIZE)
    {
        inventory.push_back(item);
        return true;
    }
    else
        return false;
    
}

Item* Player::GetFromInventory(int index)
{
    int i = 0;

    for(Item* item : inventory)
        if(i++ == index)
            return item;

    return NULL;
}

bool Player::UseFromInventory(int index)
{
    Item* item = GetFromInventory(index);

    if(item == NULL)
        return false;

    if(item->key == 0)
    {
        ChangeHealth(item->health);

        if(item->morph)
        {
            this->state = item->morph;
            ticksMorphRemaining = SDL_GetTicks() + item->morphTime;
            morphed = true;
        }

        RemoveFromInventory(index);
    }
    else
    {
        bool success = false;
        Dynamic* left = GetDynamicByCoord(x - 1, y, mapContainer);
        Dynamic* right = GetDynamicByCoord(x + 1, y, mapContainer);

        Dynamic* up = GetDynamicByCoord(x, y - 1, mapContainer);
        Dynamic* down = GetDynamicByCoord(x, y + 1, mapContainer);

        if(left != NULL)
                success += left->GetUsed(item);

        if(right != NULL)
                success += right->GetUsed(item);

        if(up != NULL)
                success += up->GetUsed(item);

        if(down != NULL)
                success += down->GetUsed(item);

        if(success)
            RemoveFromInventory(index);
    }
    
    return true;
}

bool Player::RemoveFromInventory(int index)
{
    int i = 0;

    for(Item* item : inventory)
        if(i++ == index)
        {
            inventory.erase(std::remove(inventory.begin(), inventory.end(), item), inventory.end());
            delete item;
            return true;
        }

    return false;
}

bool Player::TakeItem()
{
    // check if item is on players position
    Item* item = new Item(mapContainer->itemConfig[mapContainer->map->GetItem(this->x, this->y)]);
    item->index = mapContainer->map->GetItem(this->x, this->y);

    if(item != NULL)
    {
        if(item->collectable)
            if(AddToInventory(item))
            {
                messageCommand->PushMessage(item->name + " collected");
                mapContainer->map->SetItem(this->x, this->y, 0);
                return true;
            }
    }
    return false;
}

int Player::GetInventoryCount()
{
    return inventory.size();
}