// item.h

#pragma once
#include <string>

struct Item
{
    std::string name = "";
    uint8_t index;

    int collectable = 0;
    bool destructable = false;

    int morph = 0;
    int morphTime = 0;
    int health = 0;
    
    int useable = 0;
    int score = 0;
    int key = 0;
};