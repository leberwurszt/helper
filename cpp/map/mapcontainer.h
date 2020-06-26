#pragma once

#include "../item.h"
#include <SDL2/SDL.h>
#include <list>

#define MAX_DYNAMIC_TEXTURES 256

class Map;
class Player;
class Trigger;
class Dynamic;
class Area;
class Event;

struct MapContainer
{
    std::string mapDir = "";
    Map* map = NULL;
    
    uint8_t mapMusic = 1;
    Player* player = NULL;

    std::list<Dynamic*> dynamicList;
    std::list<Trigger*> triggerList;

    std::list<Area*> areaList;
    std::list<Event*> eventList;

    Item itemConfig[256];
    SDL_Texture* textureWall = NULL;
    SDL_Texture* textureFloor = NULL;
    SDL_Texture* textureItem = NULL;
};