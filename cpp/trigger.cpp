// trigger.cpp

#include "trigger.h"

Trigger::Trigger(std::string title, int type, MapContainer* mapContainer)
{
    this->title = title;
    this->type = type;
    this->mapContainer = mapContainer;
}

void Trigger::SetMap(std::string newMapName, uint32_t newX, uint32_t newY)
{
    this->newMapName = newMapName;
    this->newX = newX;
    this->newY = newY;
}

void Trigger::Do()
{
    if((type & TRIGGER_ON_CONTACT) && mapContainer->player->GetX() == x && mapContainer->player->GetY() == y)
        Activate();
}


bool Trigger::Activate()
{
    
    bool success = false;
    if(type & TRIGGER_SWITCH_MAP)
    {
        active = true;
        success = true;
    }

    if(type & TRIGGER_CHANGE_STATE && targetDynamic != NULL)
        if(!triggered)
        {
            targetDynamic->SetState(state1);
            triggered = true;
            success = true;
        }
        else if(type & TRIGGER_SWITCH)
        {
            targetDynamic->SetState(state2);
            triggered = false;
            success = true;
        }
    return success;
}

bool Trigger::IsTriggered()
{
    return triggered;
}

bool Trigger::IsActive()
{
    if(active)
    {
        triggered = true;
        active = false;
        return true;
    }
    return false;
        
}

std::string Trigger::GetNewMap()
{
    return newMapName;
}

void Trigger::SetPos(uint32_t x, uint32_t y)
{
    this->x = x;
    this->y = y;
}

void Trigger::SetTargetDynamic(Dynamic* targetDynamic)
{
    this->targetDynamic = targetDynamic;
}

void Trigger::SetState1(uint8_t state)
{
    this->state1 = state;
}

void Trigger::SetState2(uint8_t state)
{
    this->state2 = state;
}

uint32_t Trigger::GetX()
{
    return x;
}
uint32_t Trigger::GetY()
{
    return y;
}

uint32_t Trigger::GetNewX()
{
    return newX;
}
uint32_t Trigger::GetNewY()
{
    return newY;
}

std::string Trigger::GetTitle()
{
    return title;
}

int Trigger::GetType()
{
    return type;
}

Trigger* Trigger::GetTriggerByCoord(uint16_t x, uint16_t y, MapContainer* mapContainer)
{
    for(Trigger* trigger : mapContainer->triggerList)
    {
        if(trigger->GetX() == x && trigger->GetY() == y)
            return(trigger);
    }
    return NULL;
}

Trigger* Trigger::GetTriggerByName(std::string name, MapContainer* mapContainer)
{
    for(Trigger* trigger : mapContainer->triggerList)
    {
        if(trigger->GetTitle() == name)
            return(trigger);
    }
    return NULL;
}

std::vector<std::string> Trigger::CreateConfig()
{
    std::cout << "trigger" << std::endl;
    std::vector<std::string> vector;

    vector.push_back(title + ":");
    vector.push_back("x=" + std::to_string(x));
    vector.push_back("y=" + std::to_string(y));

    if(targetDynamic)
        vector.push_back("target_name=" + targetDynamic->GetTitle());

    vector.push_back("switch_map=" + BoolToYesNoString(type & TRIGGER_SWITCH_MAP));
    vector.push_back("change_state=" + BoolToYesNoString(type & TRIGGER_CHANGE_STATE));
    vector.push_back("teleport=" + BoolToYesNoString(type & TRIGGER_TELEPORT));

    vector.push_back("on_contact=" + BoolToYesNoString(type & TRIGGER_ON_CONTACT));
    vector.push_back("on_use=" + BoolToYesNoString(type & TRIGGER_ON_USE));
    vector.push_back("switch=" + BoolToYesNoString(type & TRIGGER_SWITCH));

    vector.push_back("state_1=" + std::to_string(state1));
    vector.push_back("state_2=" + std::to_string(state2));

    vector.push_back("new_map=" + newMapName);
    
    vector.push_back("new_x=" + std::to_string(newX));
    vector.push_back("new_y=" + std::to_string(newY));

    return vector;
}

std::map<uint16_t, std::string> Trigger::GetValues()
{
    std::map<uint16_t, std::string> values;

    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_TITLE, title));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_X, std::to_string(x)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_Y, std::to_string(y)));

    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_NEW_MAP, newMapName));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_NEW_X, std::to_string(newX)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_NEW_Y, std::to_string(newY)));

    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_SWITCH_MAP, std::to_string((type & TRIGGER_SWITCH_MAP) != 0)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_SWITCH, std::to_string((type & TRIGGER_SWITCH) != 0)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_CHANGE_STATE, std::to_string((type & TRIGGER_CHANGE_STATE) != 0)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_TELEPORT, std::to_string((type & TRIGGER_TELEPORT) != 0)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_ON_CONTACT, std::to_string((type & TRIGGER_ON_CONTACT) != 0)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_ON_USE, std::to_string((type & TRIGGER_ON_USE) != 0)));

    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_STATE_1, std::to_string(state1)));
    values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_STATE_2, std::to_string(state2)));

    if(targetDynamic)
        values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_TARGET_DYNAMIC, targetDynamic->GetTitle()));
    else
        values.insert(std::pair<uint16_t, std::string>(TRIGGER_MAP_TARGET_DYNAMIC, ""));

    return values;
}

void Trigger::SetValues(std::map<uint16_t, std::string> values)
{
    std::map<uint16_t, std::string>::iterator it = values.begin();
    uint32_t newType = 0x00000000;

    for(std::map<uint16_t, std::string>::iterator it = values.begin(); it != values.end(); it++)
    {
        try
        {
            switch(it->first)
            {
            case TRIGGER_MAP_TITLE:
                title = it->second;
                break;

            case TRIGGER_MAP_X:
                x = std::stoi(it->second);
                break; 

            case TRIGGER_MAP_Y:
                y = std::stoi(it->second);
                break;

            case TRIGGER_MAP_NEW_MAP:
                newMapName = it->second;
                break;

            case TRIGGER_MAP_NEW_X:
                newX = std::stoi(it->second);
                break;

            case TRIGGER_MAP_NEW_Y:
                newY = std::stoi(it->second);
                break;

            case TRIGGER_MAP_SWITCH_MAP:
                newType += std::stoi(it->second) * TRIGGER_SWITCH_MAP;
                break;

            case TRIGGER_MAP_CHANGE_STATE:
                newType += std::stoi(it->second) * TRIGGER_CHANGE_STATE;
                break;

            case TRIGGER_MAP_TELEPORT:
                newType += std::stoi(it->second) * TRIGGER_TELEPORT;
                break;

            case TRIGGER_MAP_ON_CONTACT:
                newType += std::stoi(it->second) * TRIGGER_ON_CONTACT;
                break;

            case TRIGGER_MAP_ON_USE:
                newType += std::stoi(it->second) * TRIGGER_ON_USE;
                break;

            case TRIGGER_MAP_SWITCH:
                newType += std::stoi(it->second) * TRIGGER_SWITCH;
                break;

            case TRIGGER_MAP_STATE_1:
                state1 = std::stoi(it->second);
                break;

            case TRIGGER_MAP_STATE_2:
                state2 = std::stoi(it->second);
                break;

            case TRIGGER_MAP_TARGET_DYNAMIC:
                targetDynamic = Dynamic::GetDynamicByName(it->second, mapContainer);
                break;

                default:
                    break;
            }
            type = newType;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}