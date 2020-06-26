// lever.h

#pragma once
#include "dynamic.h"
#include "../trigger.h"

class Lever : public Dynamic
{
    private:

    public:
        Lever(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);
        bool GetUsed() override;
        void Do() override;
};