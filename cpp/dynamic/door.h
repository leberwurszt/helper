// door.h

#pragma once
#include "dynamic.h"

#define DOOR_MAP_OPEN_TYPE 64
#define DOOR_MAP_KEY 65

#define DOOR_OPEN_TYPE_NONE         0x00
#define DOOR_OPEN_TYPE_AUTO         0x01
#define DOOR_OPEN_TYPE_MANUAL       0x02
#define DOOR_OPEN_TYPE_KEY          0x03
#define DOOR_OPEN_TYPE_REMOTE       0x04

class Door : public Dynamic
{
    private:
    uint8_t openType = 0;
    bool isOpen = false;
    uint8_t key = 0;

    public:
        Door(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand);

        void SetKey(uint8_t key);
        void SetOpenType(uint8_t openType);

        bool GetUsed() override;
        bool GetUsed(Item* item) override;
        bool Open();
        bool Close();
        void Do() override;
        std::vector<std::string> CreateConfig() override;

        std::map<uint16_t, std::string> GetValues() override;
        void SetValues(std::map<uint16_t, std::string> values) override;

        void PrintValues() override;
};