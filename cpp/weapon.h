#pragma once

#include <iostream>
#include <string>
#include "dynamic/dynamic_default.h"

enum WeaponType : uint8_t
{
    WEAPON_TYPE_NONE,
    WEAPON_TYPE_MELEE,
    WEAPON_TYPE_RANGED
};

enum AmmoType : uint8_t
{
    AMMO_TYPE_NONE,
    AMMO_TYPE_BULLETS,
    AMMO_TYPE_FIREBALL
};

struct Weapon
{
    std::string name = "";
    uint16_t maxAmmo = 0;
    uint16_t ammo = 0;
    WeaponType weaponType = WEAPON_TYPE_NONE;
    AmmoType AmmoType = AMMO_TYPE_NONE;
    int16_t damage = 0;
    DynamicType projectile = DYNAMIC;
};