#pragma once

// default values for dynamic environment

enum
{
    DYNAMIC_MAP_TITLE,
    DYNAMIC_MAP_X,
    DYNAMIC_MAP_Y,
    DYNAMIC_MAP_TYPE,
    DYNAMIC_MAP_STATE,
    DYNAMIC_MAP_HEALTH,
    DYNAMIC_MAP_MAXHEALTH,
    DYNAMIC_MAP_FLYING,
    DYNAMIC_MAP_OBSTACLE,
    DYNAMIC_MAP_DESTRUCTABLE,
    DYNAMIC_MAP_DIRECTION,
    DYNAMIC_MAP_DAMAGE,
    DYNAMIC_MAP_SPEED,
    DYNAMIC_MAP_ATTACKSPEED
};

enum Direction : uint8_t
{
    NORTH,
    EAST,
    SOUTH,
    WEST
};

enum DynamicType : uint8_t
{
    DYNAMIC,
    PLAYER,
    NPC,
    SPURDO,
    LEVER,
    DOOR,
    PRESSUREPLATE,
    FIREBALL,
    SENTRYGUN,
    ROLLINGSTONE,
    VAMPIRE,
    GONDOLA,
    SPIDER
};

// dynamic
#define DYNAMIC_HEALTH              100
#define DYNAMIC_MAX_HEALTH          100
#define DYNAMIC_NOCLIP              false
#define DYNAMIC_FLYING              false
#define DYNAMIC_STATE               1
#define DYNAMIC_TYPE                DYNAMIC
#define DYNAMIC_ALIVE               true
#define DYNAMIC_OBSTACLE            true
#define DYNAMIC_DESTRUCTABLE        true
#define DYNAMIC_DIRECTION           NORTH
#define DYNAMIC_TICKS_MOVE          0
#define DYNAMIC_TICKS_ATTACK        0
#define DYNAMIC_DAMAGE              0

// player
#define PLAYER_HEALTH               100
#define PLAYER_MAX_HEALTH           100
#define PLAYER_NOCLIP               false
#define PLAYER_FLYING               false
#define PLAYER_STATE                1
#define PLAYER_TYPE                 PLAYER
#define PLAYER_ALIVE                true
#define PLAYER_OBSTACLE             true
#define PLAYER_DESTRUCTABLE         true
#define PLAYER_DIRECTION            NORTH
#define PLAYER_TICKS_MOVE           300
#define PLAYER_TICKS_ATTACK         500
#define PLAYER_DAMAGE               100

// npc
#define NPC_HEALTH                  100
#define NPC_MAX_HEALTH              100
#define NPC_NOCLIP                  false
#define NPC_FLYING                  false
#define NPC_STATE                   1
#define NPC_TYPE                    NPC
#define NPC_ALIVE                   true
#define NPC_OBSTACLE                true
#define NPC_DESTRUCTABLE            true
#define NPC_DIRECTION               NORTH
#define NPC_TICKS_MOVE              500
#define NPC_TICKS_ATTACK            500
#define NPC_DAMAGE                  30

// spurdo
#define SPURDO_HEALTH               100
#define SPURDO_MAX_HEALTH           100
#define SPURDO_NOCLIP               false
#define SPURDO_FLYING               false
#define SPURDO_STATE                1
#define SPURDO_TYPE                 SPURDO
#define SPURDO_ALIVE                true
#define SPURDO_OBSTACLE             true
#define SPURDO_DESTRUCTABLE         true
#define SPURDO_DIRECTION            NORTH
#define SPURDO_TICKS_MOVE           500
#define SPURDO_TICKS_ATTACK         0
#define SPURDO_DAMAGE               0

// lever
#define LEVER_HEALTH                0
#define LEVER_MAX_HEALTH            0
#define LEVER_NOCLIP                false
#define LEVER_FLYING                false
#define LEVER_STATE                 1
#define LEVER_TYPE                  LEVER
#define LEVER_ALIVE                 true
#define LEVER_OBSTACLE              true
#define LEVER_DESTRUCTABLE          false
#define LEVER_DIRECTION             NORTH
#define LEVER_TICKS_MOVE            0
#define LEVER_TICKS_ATTACK          0
#define LEVER_DAMAGE                0

// rolling stone
#define ROLLINGSTONE_HEALTH         0
#define ROLLINGSTONE_MAX_HEALTH     0
#define ROLLINGSTONE_NOCLIP         false
#define ROLLINGSTONE_FLYING         false
#define ROLLINGSTONE_STATE          1
#define ROLLINGSTONE_TYPE           ROLLINGSTONE
#define ROLLINGSTONE_ALIVE          true
#define ROLLINGSTONE_OBSTACLE       true
#define ROLLINGSTONE_DESTRUCTABLE   false
#define ROLLINGSTONE_DIRECTION      NORTH
#define ROLLINGSTONE_TICKS_MOVE     500
#define ROLLINGSTONE_TICKS_ATTACK   0
#define ROLLINGSTONE_DAMAGE         100

// pressure plate
#define PRESSUREPLATE_HEALTH        0
#define PRESSUREPLATE_MAX_HEALTH    0
#define PRESSUREPLATE_NOCLIP        false
#define PRESSUREPLATE_FLYING        false
#define PRESSUREPLATE_STATE         1
#define PRESSUREPLATE_TYPE          PRESSUREPLATE
#define PRESSUREPLATE_ALIVE         true
#define PRESSUREPLATE_OBSTACLE      false
#define PRESSUREPLATE_DESTRUCTABLE  false
#define PRESSUREPLATE_DIRECTION     NORTH
#define PRESSUREPLATE_TICKS_MOVE    0
#define PRESSUREPLATE_TICKS_ATTACK  0
#define PRESSUREPLATE_DAMAGE        0

// sentry gun
#define SENTRYGUN_HEALTH            0
#define SENTRYGUN_MAX_HEALTH        0
#define SENTRYGUN_NOCLIP            false
#define SENTRYGUN_FLYING            false
#define SENTRYGUN_STATE             1
#define SENTRYGUN_TYPE              SENTRYGUN
#define SENTRYGUN_ALIVE             true
#define SENTRYGUN_OBSTACLE          true
#define SENTRYGUN_DESTRUCTABLE      false
#define SENTRYGUN_DIRECTION         NORTH
#define SENTRYGUN_TICKS_MOVE        0
#define SENTRYGUN_TICKS_ATTACK      1000
#define SENTRYGUN_DAMAGE            50

// vampire
#define VAMPIRE_HEALTH              100
#define VAMPIRE_MAX_HEALTH          100
#define VAMPIRE_NOCLIP              false
#define VAMPIRE_FLYING              true
#define VAMPIRE_STATE               1
#define VAMPIRE_TYPE                VAMPIRE
#define VAMPIRE_ALIVE               true
#define VAMPIRE_OBSTACLE            true
#define VAMPIRE_DESTRUCTABLE        true
#define VAMPIRE_DIRECTION           NORTH
#define VAMPIRE_TICKS_MOVE          700
#define VAMPIRE_TICKS_ATTACK        1000
#define VAMPIRE_DAMAGE              50

// fireball
#define FIREBALL_HEALTH             0
#define FIREBALL_MAX_HEALTH         0
#define FIREBALL_NOCLIP             true
#define FIREBALL_FLYING             true
#define FIREBALL_STATE              1
#define FIREBALL_TYPE               FIREBALL
#define FIREBALL_ALIVE              true
#define FIREBALL_OBSTACLE           false
#define FIREBALL_DESTRUCTABLE       false
#define FIREBALL_DIRECTION          NORTH
#define FIREBALL_TICKS_MOVE         250
#define FIREBALL_TICKS_ATTACK       0
#define FIREBALL_DAMAGE             50

// gondola
#define GONDOLA_HEALTH              0
#define GONDOLA_MAX_HEALTH          0
#define GONDOLA_NOCLIP              false
#define GONDOLA_FLYING              false
#define GONDOLA_STATE               1
#define GONDOLA_TYPE                GONDOLA
#define GONDOLA_ALIVE               true
#define GONDOLA_OBSTACLE            true
#define GONDOLA_DESTRUCTABLE        false
#define GONDOLA_DIRECTION           NORTH
#define GONDOLA_TICKS_MOVE          0
#define GONDOLA_TICKS_ATTACK        0
#define GONDOLA_DAMAGE              0
#define GONDOLA_MUSIC               GONDOLA_MUSIC_1

// door
#define DOOR_HEALTH                 0
#define DOOR_MAX_HEALTH             0
#define DOOR_NOCLIP                 false
#define DOOR_FLYING                 false
#define DOOR_STATE                  1
#define DOOR_TYPE                   DOOR
#define DOOR_ALIVE                  true
#define DOOR_OBSTACLE               true
#define DOOR_DESTRUCTABLE           false
#define DOOR_DIRECTION              NORTH
#define DOOR_TICKS_MOVE             0
#define DOOR_TICKS_ATTACK           0
#define DOOR_DAMAGE                 0
#define DOOR_OPEN_TYPE              DOOR_OPEN_TYPE_MANUAL

// spider
#define SPIDER_HEALTH               100
#define SPIDER_MAX_HEALTH           100
#define SPIDER_NOCLIP               false
#define SPIDER_FLYING               false
#define SPIDER_STATE                1
#define SPIDER_TYPE                 SPIDER
#define SPIDER_ALIVE                true
#define SPIDER_OBSTACLE             true
#define SPIDER_DESTRUCTABLE         true
#define SPIDER_DIRECTION            NORTH
#define SPIDER_TICKS_MOVE           350
#define SPIDER_TICKS_ATTACK         500
#define SPIDER_DAMAGE               40