// pressure_plate.h

#pragma once
#include "dynamic.h"
#include "../trigger.h"

class PressurePlate : public Dynamic
{
    private:
    bool switched = false;

    void TriggerIt();

    public:
        PressurePlate(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        void Do() override;
};