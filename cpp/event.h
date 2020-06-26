#pragma once

#include "map/mapcontainer.h"
#include "dynamic/player.h"
#include "dynamic/dynamic.h"

class Event
{
    protected:
        
    public:
        MapContainer* mapContainer;
        Event(MapContainer* mapContainer);
        void Activate();
};