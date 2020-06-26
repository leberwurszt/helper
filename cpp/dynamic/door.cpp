// remotedoor.cpp
#include "door.h"

Door::Door(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{

    health =            DOOR_HEALTH;
    maxHealth =         DOOR_MAX_HEALTH;
    noclip =            DOOR_NOCLIP;
    flying =            DOOR_FLYING;
    state =             DOOR_STATE;
    type =              DOOR_TYPE;
    alive =             DOOR_ALIVE;
    obstacle =          DOOR_OBSTACLE;
    destructable =      DOOR_DESTRUCTABLE;
    direction =         DOOR_DIRECTION;
    ticksMove =         DOOR_TICKS_MOVE;
    ticksAttack =       DOOR_TICKS_ATTACK;
    damageAttack =      DOOR_DAMAGE;
    openType =          DOOR_OPEN_TYPE;
    
    isOpen = state == 2 ? true : false;
}

void Door::Do()
{
    Dynamic::Do();

    switch (openType)
    {
    case DOOR_OPEN_TYPE_REMOTE:
        if(state == 1)
            Close();
        else
            Open();
        break;

    case DOOR_OPEN_TYPE_AUTO:
        if(CheckObstacle(x - 1, y) == 2 || CheckObstacle(x + 1, y) == 2 || CheckObstacle(x, y - 1) == 2 || CheckObstacle(x, y + 1) == 2)
            Open();
        else
            Close();
        break;

    default:
        break;
    }

    obstacle = isOpen ? false : true;
    sprite = (isOpen ? 2 : 1) + 2 * ((direction + 1) % 2);
}

bool Door::GetUsed()
{
    switch (openType)
    {
        case DOOR_OPEN_TYPE_MANUAL:
            if(isOpen)
                return Close();
            else
                return Open();
            break;
            
        case DOOR_OPEN_TYPE_KEY:
            messageCommand->PushMessage("Key required.");
            break;

        case DOOR_OPEN_TYPE_REMOTE:
            messageCommand->PushMessage("This door is opened elsewhere.");
            break;

        case DOOR_OPEN_TYPE_NONE:
            messageCommand->PushMessage("You can't open this door.");

        default:
            break;
    }
    return false;
}

bool Door::GetUsed(Item* item)
{
    if(openType == DOOR_OPEN_TYPE_KEY && item->key == key)
    {
        openType = DOOR_OPEN_TYPE_MANUAL;
        if(isOpen)
            return Close();
        else
            return Open();
    }
    else
        return false;
}

bool Door::Open()
{
    if(!isOpen)
    {
        soundCommand->PlaySound(DOOR_OPEN, this);
        isOpen = true;
        state = 2;
        SetObstacle(false);
        return true;
    }
    else
        return false;
}

bool Door::Close()
{
    if(!CheckObstacle(x, y) && isOpen)
    {
        soundCommand->PlaySound(DOOR_OPEN, this);
        isOpen = false;
        state = 1;
        SetObstacle(true);
        return true;
    }
    else
        return false;
    
}

void Door::SetKey(uint8_t key)
{
    this->key = key;
}

void Door::SetOpenType(uint8_t openType)
{
    this->openType = openType;
}

std::vector<std::string> Door::CreateConfig()
{
    std::vector<std::string> vector = Dynamic::CreateConfig();
    vector.push_back("key=" + std::to_string(key));
    vector.push_back("open_type=" + std::to_string(openType));

    return vector;
}

std::map<uint16_t, std::string> Door::GetValues()
{
    std::map<uint16_t, std::string> values = Dynamic::GetValues();

    values.insert(std::pair<uint16_t, std::string>(DOOR_MAP_OPEN_TYPE, std::to_string(openType)));
    values.insert(std::pair<uint16_t, std::string>(DOOR_MAP_KEY, std::to_string(key)));

    return values;
}

void Door::SetValues(std::map<uint16_t, std::string> values)
{
    Dynamic::SetValues(values);
    
    for(std::map<uint16_t, std::string>::iterator it = values.begin(); it != values.end(); it++)
    {
        switch(it->first)
        {
            case DOOR_MAP_OPEN_TYPE:
                openType = std::stoi(it->second);
                break;

            case DOOR_MAP_KEY:
                key = std::stoi(it->second);
                break;

            default:
                break;
        }
    }
}


void Door::PrintValues()
{
    Dynamic::PrintValues();
    std::cout <<
        "OpenType: " << (int) openType << std::endl <<
        "Key: " << (int) key << std::endl;
}