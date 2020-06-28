// dynamic.h

#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <list>
#include <cstdint>
#include <map>
#include <cmath>

#include "dynamic_sounds.h"
#include "dynamic_default.h"
#include "../map/map.h"
#include "../item.h"
#include "../map/mapcontainer.h"
#include "../sound_command.h"
#include "../message_command.h"
#include "../functions.h"



class Dynamic
{
    
    // ========== PROTECTED =========
    protected:
        
        uint16_t x = 0;
        uint16_t y = 0;
        MapContainer* mapContainer = NULL;
        SoundCommand* soundCommand = NULL;
        MessageCommand* messageCommand = NULL;

        std::string title = "";

        int16_t health = 100;
        int16_t maxHealth = 100;
        bool noclip = false;
        bool flying = false;
        uint8_t state = 1;
        DynamicType type = DYNAMIC;
        bool alive = true;
        bool obstacle = true;
        bool destructable = true;
        Direction direction = NORTH;
        int16_t damageAttack = 0;

        uint32_t ticksMove = 0;
        uint32_t ticksMoveNext = 0;
        int32_t movementX = 0;
        int32_t movementY = 0;

        Uint32 ticksAttack = 0;
        Uint32 ticksAttackNext = 0;

        uint8_t sprite = 1;

        uint16_t soundDie = 0;

        std::map<std::string, std::string> stumpen;

        

    // ========== PUBLIC =========
    public:
        Dynamic(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);

        std::string GetTitle();

        uint16_t GetX();
        uint16_t GetY();
        uint32_t GetSpeed();
        uint32_t GetAttackSpeed();

        int16_t GetHealth();
        int16_t GetMaxHealth();
        bool GetNoclip();
        uint8_t GetState();
        DynamicType GetType();
        uint8_t GetDirection();
        int16_t GetDamage();

        bool IsFlying();
        bool IsDestructable();
        bool IsAlive();
        bool IsObstacle();
        double GetMovementPercentage();
        double GetAttackPercentage();
        int16_t GetMovementX();
        int16_t GetMovementY();
        uint8_t GetSprite();

        void SetTitle(std::string title);
        void SetX(uint16_t x);
        void SetY(uint16_t y);
        void SetPos(uint16_t x, uint16_t y);

        void SetHealth(int16_t health);
        void SetMaxHealth(int16_t maxHealth);
        void SetNoclip(bool noclip);
        void SetFlying(bool flying);
        void SetState(uint8_t state);
        void SetType(DynamicType type);
        void SetObstacle(bool obstacle);
        void SetDestructable(bool destructable);
        void SetDirection(Direction direction);
        void SetSpeed(uint32_t ticks);
        void SetAttackSpeed(uint32_t ticks);
        void SetDamage(int16_t damageAttack);
        void SetMapContainer(MapContainer* mapContainer);


        void SetMessageCommand(MessageCommand* messageCommand);
        
        void ChangeHealth(int16_t health);
        uint8_t CheckObstacle(uint16_t x, uint16_t y);

        virtual bool Move(int16_t x, int16_t y);
        virtual bool Move(Direction direction);
        virtual bool Attack(uint16_t x, uint16_t y, int16_t damage);
        virtual void Hit(Dynamic* sender, int16_t damage);
        virtual void Do();
        virtual bool GetUsed();
        virtual bool GetUsed(Item* item);
        virtual void Die();
        virtual std::vector<std::string> CreateConfig();

        virtual std::map<uint16_t, std::string> GetValues();
        virtual void SetValues(std::map<uint16_t, std::string> values);

        static Dynamic* GetDynamicByCoord(uint16_t x, uint16_t y, MapContainer* mapContainer);
        static std::list<Dynamic*> GetDynamicsByCoord(uint16_t x, uint16_t y, MapContainer* mapContainer);
        static Dynamic* GetDynamicByName(std::string name, MapContainer* mapContainer);
        static Dynamic* CreateDynamicFromType(uint16_t type, std::map<uint16_t, uint16_t> dynamicTypeMap, MapContainer* mapContainer, SoundCommand* soundCommand);
        static void SortDynamicList(MapContainer* mapContainer);

        static float GetDistance(Dynamic* a, Dynamic* b);

        virtual void PrintValues();
};