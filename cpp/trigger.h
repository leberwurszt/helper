// trigger.h

#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include "area.h"
#include "dynamic/dynamic.h"
#include "dynamic/player.h"
#include "map/map.h"
#include "map/mapcontainer.h"

#define TRIGGER_NONE            0x0000
#define TRIGGER_SWITCH_MAP      0x0001
#define TRIGGER_CHANGE_STATE    0x0002
#define TRIGGER_TELEPORT        0x0004
#define TRIGGER_ON_CONTACT      0x0008
#define TRIGGER_ON_USE          0x0010
#define TRIGGER_SWITCH          0x0020
#define TRIGGER_CREATE_ITEM     0x0040
#define TRIGGER_TARGET_DYNAMIC  0x0080
#define TRIGGER_TARGET_AREA     0x0100
#define TRIGGER_SCRIPT          0x0200



class Trigger
{
    // ========= PRIVATE =========
    private:

    uint32_t type;
    std::string title;

    std::string newMapName;
    MapContainer* mapContainer = NULL;

    uint32_t newX;
    uint32_t newY;

    uint32_t x;
    uint32_t y;

    bool triggered = false;
    bool active = false;

    Dynamic* targetDynamic = NULL;
    int state1 = 0;
    int state2 = 0;
    
    Map* newMap = NULL;

    // ========= PRIVATE ==========
    public:

    Trigger(std::string title, int type, MapContainer* mapContainer);
    void SetMap(std::string newMapName, uint32_t newX, uint32_t newY);
    void SetPos(uint32_t x, uint32_t y);

    void SetTargetDynamic(Dynamic* targetDynamic);
    void SetState1(int state);
    void SetState2(int state);

    std::string GetTitle();
    uint32_t GetX();
    uint32_t GetY();

    uint32_t GetNewX();
    uint32_t GetNewY();

    int GetType();

    void Do();
    bool Activate();

    bool IsTriggered();
    bool IsActive();
    
    std::string GetNewMap();

    std::vector<std::string> CreateConfig();

    static Trigger* GetTriggerByCoord(uint32_t x, uint32_t y, MapContainer* mapContainer);
    static Trigger* GetTriggerByName(std::string name, MapContainer* mapContainer);
};