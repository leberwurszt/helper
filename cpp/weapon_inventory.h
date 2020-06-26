#pragma once
#include <list>
#include "weapon.h"

struct WeaponInventory
{
    Weapon* weapon = NULL;
    uint16_t ammo = 0;
};
