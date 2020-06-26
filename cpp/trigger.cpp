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

void Trigger::SetState1(int state)
{
    this->state1 = state;
}

void Trigger::SetState2(int state)
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

Trigger* Trigger::GetTriggerByCoord(uint32_t x, uint32_t y, MapContainer* mapContainer)
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