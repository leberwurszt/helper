// helperengine.cpp

#include "helperengine.h"

// Constructor
HelperEngine::HelperEngine(SDL_Window* window, uint32_t flags)
{

    editInputTextBoxes[EDIT_INPUT_TYPE_NONE] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_TILESET] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_MAP_NAME] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_X] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_Y] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_OPEN_MAP] = new Textbox();
    editInputTextBoxes[EDIT_INPUT_TYPE_SAVE_MAP] = new Textbox();

    InitSoundMap();
    this->window = window;
    this->configFlags = flags;

    // Window icon
    SDL_Surface* surfaceIcon = IMG_Load("icon.png");
    SDL_SetWindowIcon(window, surfaceIcon);
    SDL_FreeSurface(surfaceIcon);

    SDL_DisplayMode mode;

    bool accelerated = flags & ACCELERATION ? true : false;
    bool vsync = flags & VSYNC_ON ? true : false;

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED * accelerated | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC * vsync);

    SDL_GetWindowDisplayMode(window, &mode);
    SDL_GetWindowSize(window, &screenSizeX, &screenSizeY);
    screenRefresh = mode.refresh_rate;

    TTF_Init();
    font = TTF_OpenFont(FONT_FILE, FONT_SIZE);
    fontColour = FONT_COLOUR;
    
    soundCommand = new SoundCommand();
    messageCommand = new MessageCommand();

    for(int i = 0; i < MAX_MUSIC_FILES; i++)
        music[i] = NULL;

    for(int i = 0; i < MAX_SOUND_FILES; i++)
        sound[i] = NULL;

    for (std::vector<std::string> configVector : LoadConfig("dynamictype.config"))
        SetDynamicConfig(configVector);

    for(int i = 0; i < MAX_DYNAMIC_TEXTURES; i++)
        dynamicTextures[i] = NULL;

    SDL_Surface* surfaceDynamic = NULL;
    
    for(std::map<uint16_t, std::string>::iterator it = dynamicTextureMap.begin(); it != dynamicTextureMap.end(); it++)
        {
            surfaceDynamic = IMG_Load((DYNAMIC_TEXTURE_PATH + it->second + ".png").c_str());
            dynamicTextures[it->first] = SDL_CreateTextureFromSurface(render, surfaceDynamic);
            SDL_FreeSurface(surfaceDynamic);
        }

    SDL_Surface* surfaceTrigger = IMG_Load("trigger.png");
    SDL_Surface* surfaceInventory = IMG_Load("inventory_back.png");

    textureTrigger = SDL_CreateTextureFromSurface(render, surfaceTrigger);
    inventoryTexture = SDL_CreateTextureFromSurface(render, surfaceInventory);

    SDL_FreeSurface(surfaceTrigger);
    SDL_FreeSurface(surfaceInventory);
    
    LoadAudio();

    screenTileModuloX = SCREEN_TILE_MODULO_X;
    screenTileModuloY = SCREEN_TILE_MODULO_Y;

    screenTileModuloHalfX = screenTileModuloX / 2;
    screenTileModuloHalfY = screenTileModuloY / 2;

    for(int i = 0; i < 256; i++)
    {
        tile[i].x = TILESIZE_TILESET_X * (i % (TILESET_WIDTH));
        tile[i].y = TILESIZE_TILESET_Y * (i / (TILESET_WIDTH));

        tile[i].w = TILESIZE_TILESET_X;
        tile[i].h = TILESIZE_TILESET_Y;
    }

    imagePosition.w = tilesizeX;
    imagePosition.h = tilesizeY;
}

// Destructor
HelperEngine::~HelperEngine()
{

    for(MapContainer* mapContainer : mapList)
        DeleteMap(mapContainer);
    mapList.clear();

    ClearTextures();
    ClearAudio();
    Mix_CloseAudio();

    delete soundCommand;
    delete messageCommand;
}

// delete all loaded sound and music
void HelperEngine::ClearAudio()
{
    for(Mix_Music* m : music)
        if(m != NULL)
            Mix_FreeMusic(m);

    for(Mix_Chunk* c : sound)
        if(c != NULL)
            Mix_FreeChunk(c);
}

// delete all map textures
void HelperEngine::ClearMapTextures(MapContainer* mapContainer)
{
    if(mapContainer->textureFloor != NULL)
        SDL_DestroyTexture(mapContainer->textureFloor);

    if(mapContainer->textureWall != NULL)
        SDL_DestroyTexture(mapContainer->textureWall);

    if(mapContainer->textureItem != NULL)
        SDL_DestroyTexture(mapContainer->textureItem);
}

// delete additional textures
void HelperEngine::ClearTextures()
{

    for(uint16_t i = 0; i < MAX_DYNAMIC_TEXTURES; i++)
    {
        if(dynamicTextures[i] != NULL)
            SDL_DestroyTexture(dynamicTextures[i]);
        dynamicTextures[i] = NULL;

        if(textureTrigger != NULL)
            SDL_DestroyTexture(textureTrigger);

        if(inventoryTexture != NULL)
            SDL_DestroyTexture(inventoryTexture);
    }
}

// delete map
void HelperEngine::ClearMap(MapContainer* mapContainer)
{
    for(Dynamic* dynamic : mapContainer->dynamicList)
        delete dynamic;
    mapContainer->dynamicList.clear();

    for(Trigger* trigger : mapContainer->triggerList)
        delete trigger;
    mapContainer->triggerList.clear();

    for(Area* area : mapContainer->areaList)
        delete area;
    mapContainer->areaList.clear();

    delete mapContainer->map;
}

// pairs enumeration of sound with sound files and inserts them into sound map
void HelperEngine::InitSoundMap()
{
    soundMap.insert(std::pair<uint16_t, std::string>(PLAYER_DIE, PLAYER_DIE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(FIREBALL_HIT, FIREBALL_HIT_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(NPC_DIE, NPC_DIE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(NPC_ENEMY_DETECTED, NPC_ENEMY_DETECTED_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(ROCK_ROLLING, ROCK_ROLLING_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(PRESSURE_PLATE_USE, PRESSURE_PLATE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(VAMPIRE_ENEMY_DETECTED, VAMPIRE_ENEMY_DETECTED_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(VAMPIRE_DIE, VAMPIRE_DIE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(ROCK_HIT, ROCK_HIT_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(DOOR_OPEN, DOOR_OPEN_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(SPURDO_DIE, SPURDO_DIE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(OPEN_INVENTORY, OPEN_INVENTORY_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(LEVER_USE, LEVER_USE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(SPIDER_DIE, SPIDER_DIE_SOUND));
    soundMap.insert(std::pair<uint16_t, std::string>(SPIDER_ENEMY_DETECTED, SPIDER_ENEMY_DETECTED_SOUND));
}

bool HelperEngine::SetDynamicConfig(std::vector<std::string> configVector)
{
    int index = 0;
    std::string type;
    std::string value;

    std::string sprite;
    std::string dynamicType;

    try
    {    
        for(std::string line : configVector)
        {
            if(line != "")
            {
                if(line.at(line.length() - 1) == ':')
                    index = std::stoi(line.substr(0, line.length() - 1));
                else
                {
                    std::istringstream iss(line);
                    
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), ::tolower);
                    transform(value.begin(), value.end(), value.begin(), ::tolower);

                    if(type == "sprite")
                        sprite = value;

                    if(type == "type")
                        dynamicType = value;
                }
            }
        }
    }
    catch(const std::exception& e)
	{
		std::cerr << "Error loading config file: " << e.what() << std::endl;
        return false;
	}

    uint8_t i = 0;

    for(std::string title : dynamicTitleArray)
    {
        if(title == dynamicType)
        {
            dynamicTypeMap.insert(std::pair<int, int>(index, i));
            dynamicTextureMap.insert(std::pair<int, std::string>(index, sprite));

            return true;
        }
        i++;
    }

    return false;
}

Dynamic* HelperEngine::CreateDynamic(std::vector<std::string> dynamicVector)
{
    return CreateDynamic(dynamicVector, mapContainer, dynamicTypeMap, soundCommand, messageCommand);
}

Dynamic* HelperEngine::CreateDynamic(std::vector<std::string> dynamicVector, MapContainer* mapContainer, std::map <uint16_t, uint16_t> dynamicTypeMap, SoundCommand* soundCommand, MessageCommand* messageCommand)
{


    Dynamic* newDynamic;

    // default values
    std::string dynamicTitle = "";
    uint32_t x = 0;
    uint32_t y = 0;
    
    DynamicType dynamicSubtype = DYNAMIC;
    int dynamicType = 0;

    int* health = NULL;
    int* maxHealth = NULL;
    bool* noclip = NULL;
    bool* flying = NULL;

    int* state = NULL;
    bool* obstacle = NULL;
    bool* destructable = NULL;
    Direction* direction = NULL;

    int* damageAttack = NULL;
    Uint32* ticksMove = NULL;
    Uint32* ticksAttack = NULL;

    uint8_t* key = NULL;
    uint8_t* openType = NULL;
    uint8_t* music = NULL;

    std::string type;
    std::string value;

    // fetch configuration for one single dynamic
    try
    {    
        for(std::string line : dynamicVector)
        {
            if(line != "")
            {
                if(line.at(line.length() - 1) == ':')
                    dynamicTitle = line.substr(0, line.length() - 1);
                else
                {
                    std::istringstream iss(line);
                    
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), tolower);
                    transform(value.begin(), value.end(), value.begin(), tolower);

                    if(type == "type")
                        dynamicSubtype = (DynamicType) std::stoi(value);

                    if(type == "x")
                        x = std::stoi(value);

                    if(type == "y")
                        y = std::stoi(value);

                    if(type == "health")
                        health = new int(std::stoi(value));

                    if(type == "max_health")
                        maxHealth = new int(std::stoi(value));

                    if(type == "flying")
                        flying = new bool(value == "yes" ? true : false);

                    if(type == "state")
                        state = new int(std::stoi(value));

                    if(type == "obstacle")
                        obstacle = new bool(value == "yes" ? true : false);

                    if(type == "destructable")
                        destructable = new bool(value == "yes" ? true : false);

                    if(type == "direction")
                        direction = new Direction((Direction) std::stoi(value));

                    if(type == "damage")
                        damageAttack = new int(std::stoi(value));
                    
                    if(type == "speed")
                        ticksMove = new Uint32(std::stoi(value));

                    if(type == "attack_speed")
                        ticksAttack = new Uint32(std::stoi(value));

                    if(type == "key")
                        key = new uint8_t(std::stoi(value));

                    if(type == "open_type")
                        openType = new uint8_t(std::stoi(value));

                    if(type == "music")
                        music = new uint8_t(std::stoi(value));

                }
            }
        }
    }
    catch(const std::exception& e)
	{
		std::cerr << "Error creating dynamic: " << e.what() << std::endl;
        return NULL;
	}

    for(std::map<uint16_t, uint16_t>::iterator it = dynamicTypeMap.begin(); it != dynamicTypeMap.end(); it++)
    {
        if(it->first == dynamicSubtype)
            dynamicType = it->second;
        
    }

    newDynamic = CreateDynamicFromType(dynamicType, dynamicTypeMap, dynamicTitle, mapContainer, soundCommand);
    // Set values

    newDynamic->SetType(dynamicSubtype);
    newDynamic->SetPos(x, y);
    newDynamic->SetMessageCommand(messageCommand);

    if(maxHealth != NULL)
        newDynamic->SetMaxHealth(*maxHealth);

    if(health != NULL)
        newDynamic->SetHealth(*health);

    if(noclip != NULL)
        newDynamic->SetNoclip(*noclip);

    if(flying != NULL)
        newDynamic->SetFlying(*flying);

    if(state != NULL)
        newDynamic->SetState(*state);

    if(obstacle != NULL)
        newDynamic->SetObstacle(*obstacle);

    if(destructable != NULL)
        newDynamic->SetDestructable(*destructable);

    if(direction != NULL)
        newDynamic->SetDirection(*direction);

    if(damageAttack != NULL)
        newDynamic->SetDamage(*damageAttack);

    if(ticksMove != NULL)
        newDynamic->SetSpeed(*ticksMove);

    if(ticksAttack != NULL)
        newDynamic->SetAttackSpeed(*ticksAttack);

    // if type is door
    if(dynamicType == DOOR)
        {
            Door* door = (Door*)newDynamic;

            if(key != NULL)
                door->SetKey(*key);

            if(openType != NULL)
                door->SetOpenType(*openType);
        }

    if(dynamicType == GONDOLA)
        {
            Gondola* gondola = (Gondola*)newDynamic;

            if(music != NULL)
                gondola->SetMusic(*music);
        }

    delete health;
    delete maxHealth;
    delete noclip;
    delete flying;
    delete state;
    delete obstacle;
    delete destructable;
    delete direction;
    delete damageAttack;
    delete ticksMove;
    delete ticksAttack;
    delete key;
    delete openType;
    delete music;

    return newDynamic;
}

Trigger* HelperEngine::CreateTrigger(std::vector<std::string> triggerVector, MapContainer* mapContainer)
{
    Trigger* newTrigger = NULL;
    
    // default values
    std::string triggerTitle = "";
    std::string targetName = "";
    uint32_t x = 0;
    uint32_t y = 0;
    
    bool switchMap = false;
    bool changeState = false;
    bool teleport = false;

    bool onContact = false;
    bool onUse = false;
    bool triggerSwitch = false;

    int state1 = 0;
    int state2 = 0;

    uint32_t targetX = 0;
    uint32_t targetY = 0;

    std::string newMapName = "";
    uint32_t newX;
    uint32_t newY;


    std::string type;
    std::string value;

    // fetch configuration for one single dynamic
    try
    {    
        for(std::string line : triggerVector)
        {
            if(line != "")
            {
                if(line.at(line.length() - 1) == ':')
                    triggerTitle = line.substr(0, line.length() - 1);
                else
                {
                    std::istringstream iss(line);
                    
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), ::tolower);
                    transform(value.begin(), value.end(), value.begin(), ::tolower);

                    if(type == "target_name")
                        targetName = value;

                    if(type == "x")
                        x = std::stoi(value);

                    if(type == "y")
                        y = std::stoi(value);

                    if(type == "switch_map")
                        switchMap = value == "yes" ? true : false;

                    if(type == "change_state")
                        changeState = value == "yes" ? true : false;

                    if(type == "teleport")
                        teleport = value == "yes" ? true : false;

                    if(type == "on_contact")
                        onContact = value == "yes" ? true : false;

                    if(type == "on_use")
                        onUse = value == "yes" ? true : false;

                    if(type == "switch")
                        triggerSwitch = value == "yes" ? true : false;
                    
                    if(type == "state_1")
                        state1 = std::stoi(value);

                    if(type == "state_2")
                        state2 = std::stoi(value);

                    if(type == "target_x")
                        targetX = std::stoi(value);

                    if(type == "target_y")
                        targetY = std::stoi(value);

                    if(type == "new_map")
                        newMapName = value;

                    if(type == "new_x")
                        newX = std::stoi(value);

                    if(type == "new_y")
                        newY = std::stoi(value);

                }
            }
        }
            int type = (switchMap * TRIGGER_SWITCH_MAP)
                + (changeState * TRIGGER_CHANGE_STATE)
                + (teleport * TRIGGER_TELEPORT)
                + (onContact * TRIGGER_ON_CONTACT)
                + (onUse * TRIGGER_ON_USE)
                + (triggerSwitch * TRIGGER_SWITCH)
            ;

            Dynamic* targetDynamic = NULL;
            if(targetName != "")
                targetDynamic = Dynamic::GetDynamicByName(targetName, mapContainer);
            else
                targetDynamic = Dynamic::GetDynamicByCoord(targetX, targetY, mapContainer);

            // Create trigger object
            newTrigger = new Trigger(triggerTitle, type, mapContainer);

            // Set values
            newTrigger->SetPos(x, y);
            newTrigger->SetTargetDynamic(targetDynamic);
            newTrigger->SetState1(state1);
            newTrigger->SetState2(state2);
            newTrigger->SetMap(newMapName, newX, newY);
    }
    catch(const std::exception& e)
	{
		std::cerr << "Error creating trigger: " << e.what() << std::endl;
        return NULL;
    }
    return newTrigger;
}

Area* HelperEngine::CreateArea(std::vector<std::string> areaVector, MapContainer* mapContainer)
{
    Area* newArea = NULL;

    // default values
    std::string areaTitle = "";
    std::list<std::string> targetTriggerNameList;
    std::string targetEventName = "";

    Trigger* targetTrigger = NULL;
    Event* targetEvent = NULL;

    uint32_t lowerX = 0;
    uint32_t lowerY = 0;
    uint32_t upperX = 0;
    uint32_t upperY = 0;
    

    std::string type;
    std::string value;
    
    // fetch configuration for one single dynamic
    try
    {    
        for(std::string line : areaVector)
        {
            if(line != "")
            {
                if(line.at(line.length() - 1) == ':')
                    areaTitle = line.substr(0, line.length() - 1);
                else
                {
                    std::istringstream iss(line);
                    
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), ::tolower);
                    transform(value.begin(), value.end(), value.begin(), ::tolower);

                    if(type == "trigger");
                        targetTriggerNameList.push_back(value);

                    if(type == "event")
                        targetEventName = value;

                    if(type == "lower_x")
                        lowerX = std::stoi(value);

                    if(type == "lower_y")
                        lowerY = std::stoi(value);

                    if(type == "upper_x")
                        upperX = std::stoi(value);

                    if(type == "upper_y")
                        upperY = std::stoi(value);
                }
            }
        }
        
        // Create trigger object
        newArea = new Area(mapContainer, areaTitle);
        // Set values
        newArea->SetPos(lowerX, lowerY, upperX, upperY);

        for(std::string targetTriggerName : targetTriggerNameList)
            newArea->AddTrigger(Trigger::GetTriggerByName(targetTriggerName, mapContainer));

    }
    catch(const std::exception& e)
	{
		std::cerr << "Error creating area: " << e.what() << std::endl;
        return NULL;
    }
    return newArea;
}

Item* HelperEngine::CreateItemConfig(std::vector<std::string> itemVector)
{
    Item* newItem = new Item;

    // default values    
    std::string name;
    int collectable = 0;
    bool destructable = false;

    int morph = 0;
    int morphTime = 0;
    int health = 0;

    int useable = 0;
    int score = 0;
    int key = 0;
    

    std::string type;
    std::string value;

    // fetch configuration for one single item
    try
    {    
        for(std::string line : itemVector)
        {
            if(line != "")
            {
                if(line.at(line.length() - 1) == ':')
                    name = line.substr(0, line.length() - 1);
                else
                {
                    std::istringstream iss(line);
                    
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), ::tolower);
                    transform(value.begin(), value.end(), value.begin(), ::tolower);

                    if(type == "collectable")
                        collectable = std::stoi(value);

                    if(type == "morph")
                        morph = std::stoi(value);

                    if(type == "morph_time")
                        morphTime = std::stoi(value);

                    if(type == "destructable")
                        destructable = value == "yes" ? true : false;

                    if(type == "useable")
                        useable = std::stoi(value);
                    
                    if(type == "health")
                        health = std::stoi(value);

                    if(type == "score")
                        score = std::stoi(value);
                    if(type == "key")
                        key = std::stoi(value);

                }
            }
        }
        newItem->name = name;
        newItem->collectable = collectable;
        newItem->destructable = destructable;
        newItem->health = health;
        newItem->morph = morph;
        newItem->morphTime = morphTime;
        newItem->score = score;
        newItem->useable = useable;
        newItem->key = key;
    }
    catch(const std::exception& e)
	{
		std::cerr << "Error creating item: " << e.what() << std::endl;
        return newItem;
    }
    return newItem;
}

// fills the whole map with one tile code
void HelperEngine::FillMap(uint8_t tileType, uint8_t tileValue)
{
    for(uint16_t i = 0; i < mapContainer->map->GetSizeX(); i++)
        for(uint16_t k = 0; k < mapContainer->map->GetSizeY(); k++)
        {
            switch(tileType)
            {
            case 0: // Floor
                mapContainer->map->SetFloor(i, k, tileValue);
                break;

            case 1: // Wall
                mapContainer->map->SetWall(i, k, tileValue);
                break;

            case 2: // Item
                mapContainer->map->SetItem(i, k, tileValue);
                break;
            }
        }
}

// returns dynamic on specified position, or NULL if there is none
Dynamic* HelperEngine::GetDynamicByCoord(uint32_t x, uint32_t y)
{
    return(Dynamic::GetDynamicByCoord(x, y, mapContainer));
}

// returns dynamic with specified name, or NULL if there is none
Dynamic* HelperEngine::GetDynamicByName(std::string name)
{
    return(Dynamic::GetDynamicByName(name, mapContainer));
}

// Add dynamic to list in map container
void HelperEngine::AddDynamic(Dynamic* dynamic, MapContainer* mapContainer)
{
    mapContainer->dynamicList.push_back(dynamic);
    Dynamic::SortDynamicList(mapContainer);
}

// Add trigger to list in map container
void HelperEngine::AddTrigger(Trigger* trigger, MapContainer* mapContainer)
{
    mapContainer->triggerList.push_back(trigger);
}

// Add area to list in map container
void HelperEngine::AddArea(Area* area, MapContainer* mapContainer)
{
    mapContainer->areaList.push_back(area);
}

// Fetches the player to the map
void HelperEngine::SetPlayer(Player* player, MapContainer* mapContainer)
{
    if(mapContainer->player == NULL)
    {
        mapContainer->player = player;
        player->SetPos(mapContainer->map->GetStartX(), mapContainer->map->GetStartY());
        mapContainer->player->SetMessageCommand(messageCommand);

        AddDynamic(player, mapContainer);
    }
}

// Returns the map container
MapContainer* HelperEngine::GetMapContainer()
{
    return mapContainer;
}

// removes a specified dynamic from list and deletes it
void HelperEngine::RemoveDynamic(Dynamic* dynamic)
{
    if (dynamic != NULL && dynamic != mapContainer->player)
    {
        mapContainer->dynamicList.erase(std::remove(mapContainer->dynamicList.begin(), mapContainer->dynamicList.end(), dynamic), mapContainer->dynamicList.end());

        if(dynamic == dynamicEdit)
        {
            editDynamic = false;
            dynamicEdit = NULL;
        }
        delete dynamic;
    }
}

// checks if name of dynamic already exists
bool HelperEngine::DynamicTitleExist(std::string title)
{
    for(Dynamic* dynamic : mapContainer->dynamicList)
        if(dynamic->GetTitle() == title)
            return true;

    return false;
}

// add map to map list
void HelperEngine::AddMap(MapContainer* mapContainer)
{
    mapList.push_back(mapContainer);
}

// remove map from map list and delete it
void HelperEngine::RemoveMap(MapContainer* mapContainer)
{
    if (mapContainer != NULL)
    {
        mapList.erase(std::remove(mapList.begin(), mapList.end(), mapContainer), mapList.end());
        DeleteMap(mapContainer);
    }
}

// set active map by name
void HelperEngine::SetMap(std::string name)
{
    for(MapContainer* mapContainer : mapList)
        if(mapContainer->mapDir == name)
        {
            SetMap(mapContainer);
            return;
        }
    std::cerr << "map \"" << name << "\" does not exist!" << std::endl;
}

// set active map by mapContainer
void HelperEngine::SetMap(MapContainer* mapContainer)
{
    this->mapContainer = mapContainer;
    
    ClearMapTextures(mapContainer);
    LoadMapTextures(mapContainer);

    SetPlayer(new Player("player", mapContainer, soundCommand), mapContainer);
    SDL_SetWindowTitle(window, ("Helper Quest - " + mapContainer->map->GetTitle()).c_str());
}

void HelperEngine::SwitchMap(std::string name)
{
    for(MapContainer* mapContainer : mapList)
        if(mapContainer->mapDir == name)
        {
            SwitchMap(mapContainer);
            return;
        }
    // if map is not loaded, load it
    std::cout << "Loading map " << name << std::endl;
    MapContainer* mapContainer = LoadMap(name);

    if(mapContainer != NULL)
    {
        AddMap(mapContainer);
        SwitchMap(mapContainer);
    }
    else
        std::cerr << "Map not found: " << name << std::endl; 
}

void HelperEngine::SwitchMap(MapContainer* mapContainer)
{
    Player* player = this->mapContainer->player;
    player->SetMapContainer(mapContainer);

    this->mapContainer->dynamicList.erase(std::remove(this->mapContainer->dynamicList.begin(), this->mapContainer->dynamicList.end(), player), this->mapContainer->dynamicList.end());
    ClearMapTextures(this->mapContainer);

    this->mapContainer = mapContainer;
    LoadMapTextures(mapContainer);

    mapContainer->player = player;
    AddDynamic(player, mapContainer);
    SDL_SetWindowTitle(window, ("Helper Quest - " + mapContainer->map->GetTitle()).c_str());
}

void HelperEngine::DeleteMap(MapContainer* mapContainer)
{
    ClearMap(mapContainer);
    ClearMapTextures(mapContainer);
    delete mapContainer;
}

Dynamic* HelperEngine::CreateDynamicFromType(uint16_t type, std::map<uint16_t, uint16_t> dynamicTypeMap, std::string name, MapContainer* mapContainer, SoundCommand* soundCommand)
{
    Dynamic* newDynamic = CreateDynamicFromType(type, dynamicTypeMap, mapContainer, soundCommand);
    newDynamic->SetTitle(name);
    return newDynamic;
}

// create dynamic depending on type
Dynamic* HelperEngine::CreateDynamicFromType(uint16_t type, std::map<uint16_t, uint16_t> dynamicTypeMap, MapContainer* mapContainer, SoundCommand* soundCommand)
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

void HelperEngine::EnumerateName(Dynamic* dynamic)
{
    std::string name = dynamic->GetTitle();

    std::string tempNumber = "";
    std::string tempName = "";

    for(int i = 0; i < name.length(); i++)
        if(isdigit(name[i]))
            tempNumber += name[i];
        else
            tempName += name[i];

    if(tempNumber.length() == 0)
        tempNumber = "0";

    do
    {
        tempNumber = std::to_string(std::stoi(tempNumber) + 1);
        name = tempName + tempNumber;
    } while(DynamicTitleExist(name));

    dynamic->SetTitle(name);
}

MapContainer* HelperEngine::CreateMap(std::string name, std::string tileset)
{
    MapContainer* mapContainer = new MapContainer();
    mapContainer->map = new Map(64, 64);
    mapContainer->map->SetTitle(name);

    mapContainer->map->SetTileset(tileset);
    mapContainer->map->SetStart(1, 1);

    return mapContainer;
}