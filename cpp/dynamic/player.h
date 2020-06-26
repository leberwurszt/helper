// player.h

#pragma once
#include "dynamic.h"
#include <list>
#include <iterator>
#include <algorithm>

#define PLAYER_INVENTORY_MAX_SIZE_X 5
#define PLAYER_INVENTORY_MAX_SIZE_Y 5
#define PLAYER_INVENTORY_MAX_SIZE PLAYER_INVENTORY_MAX_SIZE_X * PLAYER_INVENTORY_MAX_SIZE_Y

enum
{
    PLAYER_STATE_NULL,
    PLAYER_STATE_NORMAL,
    PLAYER_STATE_HONKLER,
    PLAYER_STATE_DEAD
    
};

class Player : public Dynamic
{
    private:
        Uint32 ticksMorphRemaining;
        const Uint32 ticksAttackNormal = ticksAttack;
        bool morphed = false;
        int score = 0;
        std::list<Item*> inventory;
        int weapon = 0;

    public:
        Player(std::string title, MapContainer* mapContainer, SoundCommand* SoundCommand);
        void Do() override;
        bool Use(uint16_t x, uint16_t y);
        bool Use(uint16_t x, uint16_t y, Item* item);
        int GetScore();
        void SetScore(int score);
        void AddScore(int score);

        int GetInventoryCount();
        bool AddToInventory(Item* item);
        Item* GetFromInventory(int index);
        bool UseFromInventory(int index);
        bool RemoveFromInventory(int index);
        bool TakeItem();
};