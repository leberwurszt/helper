#include "area.h"

Area::Area(MapContainer* mapContainer, std::string title)
{
    this->mapContainer = mapContainer;
    this->title = title;
}

Area::Area(MapContainer* mapContainer, std::string title, uint32_t lowerX, uint32_t lowerY, uint32_t upperX, uint32_t upperY): Area(mapContainer, title)
{
    SetPos(lowerX, lowerY, upperX, upperY);
}

Area::~Area()
{
    triggerList.clear();
}

uint32_t Area::GetLowerX()
{
    return lowerX;
}

uint32_t Area::GetLowerY()
{
    return lowerY;
}

uint32_t Area::GetUpperX()
{
    return upperX;
}

uint32_t Area::GetUpperY()
{
    return upperY;
}

bool Area::IsInside(uint32_t x, uint32_t y)
{
    if(x >= lowerX && x <= upperX && y >= lowerY && y <= upperY)
        return true;
    else
        return false;
}


void Area::SetLowerX(uint32_t lowerX)
{
    this->lowerX = lowerX;
}

void Area::SetLowerY(uint32_t lowerY)
{
    this->lowerY = lowerY;
}

void Area::SetUpperX(uint32_t upperX)
{
    this->upperX = upperX;
}

void Area::SetUpperY(uint32_t upperY)
{
    this->upperY = upperY;
}

void Area::SetPos(uint32_t lowerX, uint32_t lowerY, uint32_t upperX, uint32_t upperY)
{
    this->lowerX = lowerX;
    this->lowerY = lowerY;

    this->upperX = upperX;
    this->upperY = upperY;
}

void Area::SetEvent(Event* event)
{
    this->event = event;
}

void Area::AddTrigger(Trigger* trigger)
{
    triggerList.push_back(trigger);
}

void Area::SetTriggerList(std::list<Trigger*> triggerList)
{
    this->triggerList = triggerList;
}

void Area::Do()
{
        if(IsInside(mapContainer->player->GetX(), mapContainer->player->GetY()))
            PerformAction();
}

void Area::PerformAction()
{
    for(Trigger* trigger : triggerList)
        if(trigger != NULL)
            trigger->Activate();

    if(event != NULL)
        event->Activate();
}

std::vector<std::string> Area::CreateConfig()
{
    std::cout << "area" << std::endl;
    std::vector<std::string> vector;

    vector.push_back(title + ":");
    vector.push_back("lower_x=" + std::to_string(lowerX));
    vector.push_back("lower_y=" + std::to_string(lowerY));

    vector.push_back("upper_x=" + std::to_string(upperX));
    vector.push_back("upper_y=" + std::to_string(upperY));

    for(Trigger* trigger : triggerList)
        if(trigger)
            vector.push_back("trigger=" + trigger->GetTitle());
            
    std::cout << "area!" << std::endl;
    return vector;
}

std::string Area::GetTitle()
{
    return title;
}