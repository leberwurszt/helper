// dynamic.cpp

#include "dynamic.h"
#include "alldynamics.h"

Dynamic::Dynamic(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand)
{
    this->mapContainer = mapContainer;
    this->soundCommand = soundCommand;
    this->title = title;

    health =        DYNAMIC_HEALTH;
    maxHealth =     DYNAMIC_MAX_HEALTH;
    noclip =        DYNAMIC_NOCLIP;
    flying =        DYNAMIC_FLYING;
    state =         DYNAMIC_STATE;
    type =          DYNAMIC_TYPE;
    alive =         DYNAMIC_ALIVE;
    obstacle =      DYNAMIC_OBSTACLE;
    destructable =  DYNAMIC_DESTRUCTABLE;
    direction =     DYNAMIC_DIRECTION;
    ticksMove =     DYNAMIC_TICKS_MOVE;
    ticksAttack =   DYNAMIC_TICKS_ATTACK;
    damageAttack =  DYNAMIC_DAMAGE;
}
// ---------------------- GET ------------------

std::string Dynamic::GetTitle()
{
    return title;
}

uint16_t Dynamic::GetX()
{
    return x;
}

uint16_t Dynamic::GetY()
{
    return y;
}

uint32_t Dynamic::GetSpeed()
{
    return ticksMove;
}

uint32_t Dynamic::GetAttackSpeed()
{
    return ticksAttack;
}

int16_t Dynamic::GetHealth()
{
    return health;
}

int16_t Dynamic::GetMaxHealth()
{
    return maxHealth;
}

bool Dynamic::GetNoclip()
{
    return noclip;
}

uint8_t Dynamic::GetState()
{
    return state;
}

DynamicType Dynamic::GetType()
{
    return type;
}

uint8_t Dynamic::GetDirection()
{
    return direction;
}

int16_t Dynamic::GetDamage()
{
    return damageAttack;
}

bool Dynamic::IsFlying()
{
    return flying;
}

bool Dynamic::IsDestructable()
{
    return destructable;
}

bool Dynamic::IsAlive()
{
    return alive;
}

bool Dynamic::IsObstacle()
{
    return obstacle;
}

double Dynamic::GetMovementPercentage()
{
    double movementPercentage;

    if(SDL_GetTicks() < ticksMoveNext)
        movementPercentage = (double) (ticksMoveNext - SDL_GetTicks()) / (double) ticksMove;
    else
    {
        movementPercentage = 0;
        movementX = 0;
        movementY = 0;
    }
    return movementPercentage;
}

double Dynamic::GetAttackPercentage()
{
    double attackPercentage;
    if(SDL_GetTicks() < ticksAttackNext)
        attackPercentage = (double) (ticksAttackNext - SDL_GetTicks()) / (double) ticksAttack;
    else
        attackPercentage = 0;
    return attackPercentage;
}

int16_t Dynamic::GetMovementX()
{
    return movementX;
}

int16_t Dynamic::GetMovementY()
{
    return movementY;
}

uint8_t Dynamic::GetSprite()
{
    return sprite;
}

Dynamic* Dynamic::GetDynamicByCoord(uint32_t x, uint32_t y, MapContainer* mapContainer)
{
    for(Dynamic* dynamic : mapContainer->dynamicList)
    {
        if(dynamic->GetX() == x && dynamic->GetY() == y)
            return(dynamic);
    }
    return NULL;
}

std::list<Dynamic*> Dynamic::GetDynamicsByCoord(uint32_t x, uint32_t y, MapContainer* mapContainer)
{
    std::list<Dynamic*> dynamicList;
    for(Dynamic* dynamic : mapContainer->dynamicList)
    {
        if(dynamic->GetX() == x && dynamic->GetY() == y)
            dynamicList.push_back(dynamic);
    }
    return dynamicList;
}

Dynamic* Dynamic::GetDynamicByName(std::string name, MapContainer* mapContainer)
{
    for(Dynamic* dynamic : mapContainer->dynamicList)
    {
        if(dynamic->GetTitle() == name)
            return(dynamic);
    }
    return NULL;
}

// --------------------- SET ------------------

void Dynamic::SetTitle(std::string title)
{
    this->title = title;
}

void Dynamic::SetX(uint16_t x)
{
    if(x < mapContainer->map->GetSizeX())
        this->x = x;
}

void Dynamic::SetY(uint16_t y)
{
    if(y < mapContainer->map->GetSizeY())
        this->y = y;
}

void Dynamic::SetPos(uint16_t x, uint16_t y)
{
    if(x < mapContainer->map->GetSizeX() && y < mapContainer->map->GetSizeY())
    {
        this->x = x;
        this->y = y;
    }
}

void Dynamic::SetHealth(int16_t health)
{
    this->health = health;

    // Check if health is greater than 0, otherwise set alive to false
    if(health <= 0)
        Die();
 
    // Check if health is greater than maximum Health, in that case, set health to maxHealth
    this->health = this->health > maxHealth ? maxHealth : this->health; 
}

void Dynamic::SetMaxHealth(int16_t maxHealth)
{
    this->maxHealth = maxHealth;
    health = maxHealth;
}

void Dynamic::SetNoclip(bool noclip)
{
    this->noclip = noclip;
}

void Dynamic::SetFlying(bool flying)
{
    this->flying = flying;
}

void Dynamic::SetState(uint8_t state)
{
    this->state = state;
}

void Dynamic::SetType(DynamicType type)
{
    this->type = type;
}

void Dynamic::SetObstacle(bool obstacle)
{
    this->obstacle = obstacle;
}

void Dynamic::SetDestructable(bool destructable)
{
    this->destructable = destructable;
}

void Dynamic::SetDirection(Direction direction)
{
    this->direction = direction;
}

void Dynamic::SetSpeed(uint32_t ticks)
{
    this->ticksMove = ticks;
}

void Dynamic::SetAttackSpeed(uint32_t ticks)
{
    this->ticksAttack = ticks;
}

void Dynamic::SetDamage(int16_t damageAttack)
{
    this->damageAttack = damageAttack;
}

void Dynamic::SetMessageCommand(MessageCommand* messageCommand)
{
    this->messageCommand = messageCommand;
}

void Dynamic::SetMapContainer(MapContainer* mapContainer)
{
    this->mapContainer = mapContainer;
}

// ------------------- runtime -------------------

bool Dynamic::GetUsed()
{
    return false;
}

bool Dynamic::GetUsed(Item* item)
{
    return false;
}

void Dynamic::ChangeHealth(int16_t health)
{
    SetHealth(this->health + health);   // Add health
}

void Dynamic::Die()
{
    if(alive && destructable)
    {
        alive = false;
        soundCommand->PlaySound(soundDie, this);

        SetObstacle(false);
        SortDynamicList(mapContainer);
    }
}

bool Dynamic::Attack(uint16_t x, uint16_t y, int16_t damage)
{
    if(((SDL_GetTicks() > ticksAttackNext && SDL_GetTicks() >= ticksMoveNext) || ticksAttack == 0) && alive)
    {
        // Set direction to attack direction
        if(x > this->x)
            direction = EAST;
        else if(x < this->x)
            direction = WEST;
        else if(y > this->y)
            direction = SOUTH;
        else if(y < this->y)
            direction = NORTH;

        for(Dynamic* dynamic : GetDynamicsByCoord(x, y, mapContainer))
            if(dynamic)
                if(dynamic->GetX() == x && dynamic->GetY() == y)
                    dynamic->Hit(this, damage);
                
        ticksAttackNext = SDL_GetTicks() + ticksAttack;
        return true;
    }
    else
        return false;    
}

// Checks obstacle (wall, item, dynamic) is on position and returns 1, if not dynamic, 2, if dynamic
uint8_t Dynamic::CheckObstacle(uint16_t x, uint16_t y)
{
    if(mapContainer->map->GetWall(x, y) != 0)
        return 1;

    if(x >= mapContainer->map->GetSizeX() || y >= mapContainer->map->GetSizeY())
        return 1;

    if(mapContainer->map->GetFloorType(x, y) == FLOOR_TYPE_WATER && !flying)
        return 1;
    
    for(Dynamic* dynamicOnTarget : GetDynamicsByCoord(x, y, mapContainer))
        if(dynamicOnTarget)
            if(dynamicOnTarget->IsObstacle())
                return 2;

    return 0;
}

bool Dynamic::Move(int16_t x, int16_t y)
{
    if(SDL_GetTicks() >= ticksMoveNext && alive)
    {
        // Set direction to moving direction
        if(x > 0)
            direction = EAST;
        else if(x < 0)
            direction = WEST;
        else if(y > 0)
            direction = SOUTH;
        else if(y < 0)
            direction = NORTH;

        if(!CheckObstacle(this->x + x, this->y + y) || noclip)
        {

            //Uint32 ticksOver = SDL_GetTicks() - ticksMoveNext;
            SetPos(this->x + x, this->y + y);

            movementX = x;
            movementY = y;

            ticksMoveNext = SDL_GetTicks() + ticksMove;
            return true;
        }
    }
    return false;
}

void Dynamic::Do()
{
    if(state == 0)
        alive = false;
}

void Dynamic::Hit(Dynamic* sender, int16_t damage)
{
    ChangeHealth(-damage);
}

std::map<uint16_t, std::string> Dynamic::GetValues()
{
    std::map<uint16_t, std::string> values;

    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_TITLE, title));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_X, std::to_string(x)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_Y, std::to_string(y)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_TYPE, std::to_string(type)));

    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_STATE, std::to_string(state)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_HEALTH, std::to_string(health)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_MAXHEALTH, std::to_string(maxHealth)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_FLYING, std::to_string(flying)));
    
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_OBSTACLE, std::to_string(obstacle)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_DESTRUCTABLE, std::to_string(destructable)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_DIRECTION, std::to_string(direction)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_DAMAGE, std::to_string(damageAttack)));

    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_SPEED, std::to_string(ticksMove)));
    values.insert(std::pair<uint16_t, std::string>(DYNAMIC_MAP_ATTACKSPEED, std::to_string(ticksAttack)));

    return values;
}

void Dynamic::SetValues(std::map<uint16_t, std::string> values)
{
    std::map<uint16_t, std::string>::iterator it = values.begin();

    for(std::map<uint16_t, std::string>::iterator it = values.begin(); it != values.end(); it++)
    {
        try
        {
            switch(it->first)
            {
                case DYNAMIC_MAP_TITLE:
                    title = it->second;
                    break;

                case DYNAMIC_MAP_X:
                    x = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_Y:
                    y = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_TYPE:
                    type = (DynamicType) std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_STATE:
                    state = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_HEALTH:
                    health = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_MAXHEALTH:
                    maxHealth = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_FLYING:
                    flying = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_OBSTACLE:
                    obstacle = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_DESTRUCTABLE:
                    destructable = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_DIRECTION:
                    direction = (Direction) std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_DAMAGE:
                    damageAttack = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_SPEED:
                    ticksMove = std::stoi(it->second);
                    break;

                case DYNAMIC_MAP_ATTACKSPEED:
                    ticksAttack = std::stoi(it->second);
                    break;

                default:
                    break;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

std::vector<std::string> Dynamic::CreateConfig()
{
    std::vector<std::string> vector;

    vector.push_back(title + ":");
    vector.push_back("x=" + std::to_string(x));
    vector.push_back("y=" + std::to_string(y));
    vector.push_back("type=" + std::to_string(type));
    vector.push_back("state=" + std::to_string(state));

    vector.push_back("health=" + std::to_string(health));
    vector.push_back("max_health=" + std::to_string(maxHealth));
    vector.push_back("flying=" + BoolToYesNoString(flying));
    vector.push_back("obstacle=" + BoolToYesNoString(obstacle));

    vector.push_back("destructable=" + BoolToYesNoString(destructable));
    vector.push_back("direction=" + std::to_string(direction));
    vector.push_back("damage=" + std::to_string(damageAttack));

    vector.push_back("speed=" + std::to_string(ticksMove));
    vector.push_back("attack_speed=" + std::to_string(ticksAttack));
    
    return vector;
}

void Dynamic::PrintValues()
{
        std::cout <<
            "Name: " << title << std::endl <<
            "X: " << x << std::endl <<
            "Y: " << y << std::endl <<
            "Type: " << type << std::endl <<
            "Health: " << health << std::endl <<
            "Max Health: " << maxHealth << std::endl <<
            "Noclip: " << noclip << std::endl <<
            "Flying: " << flying << std::endl <<
            "State: " << state << std::endl <<
            "Obstacle: " << obstacle << std::endl <<
            "Destructable: " << destructable << std::endl <<
            "Direction: " << direction << std::endl <<
            "Attack damage: " << damageAttack << std::endl <<
            "Move speed: " << ticksMove << std::endl <<
            "Attack speed: " << ticksAttack << std::endl;
}

// create dynamic depending on type
Dynamic* Dynamic::CreateDynamicFromType(uint16_t type, std::map<uint16_t, uint16_t> dynamicTypeMap, MapContainer* mapContainer, SoundCommand* soundCommand)
{
    int realType;
    
    for(std::map<uint16_t, uint16_t>::iterator it = dynamicTypeMap.begin(); it != dynamicTypeMap.end(); it++)
    {
        if(it->first == type)
            realType = it->second;
    }

    Dynamic* newDynamic = NULL;
    switch(realType)
    {
        case PLAYER: // player
            newDynamic = new Player("player", mapContainer, soundCommand);
            break;
        case NPC: // NPC
            newDynamic = new Npc("npc", mapContainer, soundCommand);
            break;
        case SPURDO: // Spinning Spurdo Spärde
            newDynamic = new Spurdo("spurdo", mapContainer, soundCommand);
            break;
        case LEVER: // lever
            newDynamic = new Lever("lever", mapContainer, soundCommand);
            break;
        case DOOR: // door
            newDynamic = new Door("door", mapContainer, soundCommand);
            break;
        case PRESSUREPLATE: // pressure plate
            newDynamic = new PressurePlate("pressure_plate", mapContainer, soundCommand);
            break;
        case FIREBALL:
            newDynamic = new Fireball("fireball", mapContainer, soundCommand);
            break;
        case SENTRYGUN:
            newDynamic = new SentryGun("sentry_gun", mapContainer, soundCommand);
            break;
        case ROLLINGSTONE:
            newDynamic = new RollingStone("rolling_stone", mapContainer, soundCommand);
            break;
        case VAMPIRE:
            newDynamic = new Vampire("vampire", mapContainer, soundCommand);
            break;
        case GONDOLA:
            newDynamic = new Gondola("gondola", mapContainer, soundCommand);
            break;
        case SPIDER:
            newDynamic = new Spider("spider", mapContainer, soundCommand);
            break;
        default: // generic dynamic
            newDynamic = new Dynamic("dynamic", mapContainer, soundCommand);
            break;
    }
    return newDynamic;
}

void Dynamic::SortDynamicList(MapContainer* mapContainer)
{
    mapContainer->dynamicList.sort([](Dynamic* dynamic1, Dynamic* dynamic2)
    {
        int priority1 = 10;
        int priority2 = 10;

        if(dynamic1->GetType() == PRESSUREPLATE)
            priority1 = 1;
        else if(!dynamic1->IsAlive())
            priority1 = 2;
        else if(dynamic1->GetType() == PLAYER)
            priority1 = 3;
        if(dynamic1->IsFlying())
            priority1 = 20;

        if(dynamic2->GetType() == PRESSUREPLATE)
            priority2 = 1;
        else if(!dynamic2->IsAlive())
            priority2 = 2;
        else if(dynamic2->GetType() == PLAYER)
            priority2 = 3;
        if(dynamic2->IsFlying())
            priority2 = 20;

        if(priority1 != priority2)
            return priority1 < priority2;
        else
            return dynamic1->GetTitle() < dynamic2->GetTitle();
    });
}