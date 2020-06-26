#pragma once
#include <iostream>
#include <list>
#include <algorithm>
#include "map/mapcontainer.h"
#include "trigger.h"
#include "event.h"

class Area
{
    protected:
        MapContainer* mapContainer = NULL;
        std::list<Trigger*> triggerList;

        std::string title;
        Event* event = NULL;

        uint32_t lowerX = 0;
        uint32_t lowerY = 0;
        
        uint32_t upperX = 0;
        uint32_t upperY = 0;

    public:
        Area(MapContainer* mapContainer, std::string title);
        Area(MapContainer* mapContainer, std::string title, uint32_t lowerX, uint32_t lowerY, uint32_t upperX, uint32_t upperY);
        ~Area();

        std::string GetTitle();
        uint32_t GetLowerX();
        uint32_t GetLowerY();

        uint32_t GetUpperX();
        uint32_t GetUpperY();

        bool IsInside(uint32_t x, uint32_t y);

        void SetLowerX(uint32_t lowerX);
        void SetLowerY(uint32_t lowerY);

        void SetUpperX(uint32_t upperX);
        void SetUpperY(uint32_t upperY);

        void SetPos(uint32_t lowerX, uint32_t lowerY, uint32_t upperX, uint32_t upperY);

        void SetEvent(Event* event);
        void AddTrigger(Trigger* trigger);
        void SetTriggerList(std::list<Trigger*> triggerList);

        std::vector<std::string> CreateConfig();

        void Do();
        void PerformAction();
        
};