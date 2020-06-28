// helperengine.h

#pragma once

// main libraries
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <iterator>
#include <string>
#include <unistd.h>
#include <chrono>
#include <vector>
#include <algorithm>
#include <map>
#include <array>
#include <filesystem>

// SDL libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

// additional classes
#include "../map/map.h"
#include "../trigger.h"
#include "../sound_command.h"
#include "../message_command.h"
#include "../area.h"
#include "../event.h"
#include "../textbox.h"
#include "../config_table.h"

// additional structures
#include "../map/mapcontainer.h"
#include "../item.h"

// definition lists
#include "../dynamic/alldynamics.h"
#include "../dynamic/dynamic_sounds.h"

#include "../functions.h"


// size of tile
#define TILESIZE_TILESET_X 64
#define TILESIZE_TILESET_Y 64

#define TILESET_WIDTH 4
#define TILESET_HEIGHT 64

// tile count on screen
#define VIEWSIZE_X screenSizeX / tilesizeX
#define VIEWSIZE_Y screenSizeY / tilesizeY

// font
#define FONT_SIZE 32
#define FONT_COLOUR {0, 255, 0}
#define FONT_FILE "font.ttf"

#define MESSAGE_POS_X 0
#define MESSAGE_POS_Y 0

// rest pixels
#define SCREEN_TILE_MODULO_X screenSizeX % tilesizeX
#define SCREEN_TILE_MODULO_Y screenSizeY % tilesizeY

// folder paths
#define MAP_PATH "./maps/"                // path of map folder
#define DYNAMIC_TEXTURE_PATH "./dynamic/" // path of dynamic texture
#define TILESET_PATH "./tileset/"         // path of tilesets
#define MUSIC_PATH "./music/"             // path of music files
#define SOUND_PATH "./sound/"             // path of sound files

// sound initialization values
#define MAX_MUSIC_FILES 64
#define MAX_SOUND_FILES 17
#define SOUND_CHANNELS 64

// sound files
#define PLAYER_DIE_SOUND                "bech.wav"
#define FIREBALL_HIT_SOUND              "laser.wav"
#define NPC_DIE_SOUND                   "npc_die.wav"
#define NPC_ENEMY_DETECTED_SOUND        "npc_enemy_detected.wav"
#define ROCK_ROLLING_SOUND              "rock.wav"
#define PRESSURE_PLATE_SOUND            "pressure_plate.wav"
#define VAMPIRE_ENEMY_DETECTED_SOUND    "vampire_enemy_detected.wav"
#define VAMPIRE_DIE_SOUND               "vampire_die.wav"
#define ROCK_HIT_SOUND                  "rock_hit.wav"
#define DOOR_OPEN_SOUND                 "door.wav"
#define SPURDO_DIE_SOUND                "fug.wav"
#define OPEN_INVENTORY_SOUND            "inventory_open.wav"
#define LEVER_USE_SOUND                 "pressure_plate.wav"
#define SPIDER_DIE_SOUND                "spider_die.wav"
#define SPIDER_ENEMY_DETECTED_SOUND     "spider_enemy_detected.wav"

#define MESSAGE_TIME 5000

// flags
#define EDIT_MODE       0x01
#define SHOW_FPS        0x02
#define PLAY_MUSIC      0x04
#define PLAY_SOUNDS     0x08
#define VSYNC_ON        0x10
#define ACCELERATION    0x20

static std::string dynamicTitleArray[] =
{
    "dynamic",
    "player",
    "npc",
    "spurdo",
    "lever",
    "door",
    "pressureplate",
    "fireball",
    "sentrygun",
    "rollingstone",
    "vampire",
    "gondola",
    "spider"
};

static std::string editInputStrings[] =
{
    "",
    "Tileset",
    "Size X",
    "Size Y",
    "Map name",
    "Load map",
    "Save map"
};

enum EditInputType: uint16_t
{
    EDIT_INPUT_TYPE_NONE,
    EDIT_INPUT_TYPE_TILESET,
    EDIT_INPUT_TYPE_SIZE_X,
    EDIT_INPUT_TYPE_SIZE_Y,
    EDIT_INPUT_TYPE_MAP_NAME,
    EDIT_INPUT_TYPE_OPEN_MAP,
    EDIT_INPUT_TYPE_SAVE_MAP
};

class HelperEngine
{
    // ============= PROTECTED ==============
    protected:


    //std::map <std::string, uint16_t> dynamicTitleMap;

    std::map <uint16_t, std::string> soundMap;
    std::map <uint16_t, std::string> musicMap;

    std::map <uint16_t, uint16_t> dynamicTypeMap;
    std::map <uint16_t, std::string> dynamicTextureMap;

    SDL_Texture* dynamicTextures[MAX_DYNAMIC_TEXTURES];

    uint32_t configFlags = 0x00000000;

    int screenSizeX = 640;
    int screenSizeY = 480;
    int screenRefresh = 60;

    uint16_t screenTileModuloX = 0;
    uint16_t screenTileModuloY = 0;

    uint16_t screenTileModuloHalfX = 0;
    uint16_t screenTileModuloHalfY = 0;

    uint16_t tilesizeX = 64;
    uint16_t tilesizeY = 64;

    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;

    MapContainer* mapContainer;
    std::list<MapContainer*> mapList;

    SDL_Rect tile[256];
    SDL_Rect imagePosition;
    Mix_Music* music[MAX_MUSIC_FILES];
    Mix_Chunk* sound[MAX_SOUND_FILES];

    SDL_Texture* inventoryTexture = NULL;
    SoundCommand* soundCommand = NULL;
    MessageCommand* messageCommand = NULL;

    Textbox* editInputTextBoxes[7];
    Textbox** editDynamicTextBoxes = NULL;

    Uint32 messageTime = 0;
    Uint32 fpsDisplayNext = 0;
    std::string message = "";

    uint8_t musicPlaying = 0;
    uint8_t musicVolume = 0;

    // text in window
    TTF_Font* font;
    SDL_Colour fontColour;
    SDL_Rect textPosition;


    // camera
    int32_t cameraX = 0; 
    int32_t cameraY = 0;
    int16_t cameraOffsetX = 0; 
    int16_t cameraOffsetY = 0;

    // editor
    uint8_t tileType = 0;
    uint8_t tileValue = 0;
    bool wallAlpha = false;

    Dynamic* dynamicEdit = NULL;
    Trigger* triggerEdit = NULL;
    Area* areaEdit = NULL;

    std::map<uint16_t, std::string> editValues;
    SDL_Rect* editTableDescription = NULL;
    SDL_Rect* editTableValue = NULL;

    int16_t editDynamicInput = -1;
    Dynamic* dynamicMove = NULL;
    Dynamic* dynamicCopy = NULL;

    uint16_t newDynamicType = 1;
    SDL_Texture* textureTrigger = NULL;
    EditInputType editInputType = EDIT_INPUT_TYPE_NONE;

    // mouse
    int mouseX = 0;
    int mouseY = 0;
    uint16_t mouseTileX = 0;
    uint16_t mouseTileY = 0;
    uint32_t mouseScrollTime = 1;
    uint32_t mouseScrollLast = 0;

    // gameplay
    uint16_t inventoryIndex = 0;
    bool showInventory = false;
    bool godMode = false;

    // runtime
    bool run = false;
    bool askQuit = false;
    SDL_Event event;

    // Framerate control
    std::chrono::high_resolution_clock::time_point fpsTicks;
    std::chrono::high_resolution_clock::time_point fpsTicksLast;
    double fps = 0;
    double fpsShown = 0;

    // clear methods
    void ClearTextures(); 
    void ClearMap(MapContainer* mapContainer);
    void ClearAudio();
    void ClearMapTextures(MapContainer* mapContainer);

    // helpful methods
    void FillMap(uint8_t tileType, uint8_t tileValue);
    void DrawEditTable();
    void CreateEditTable(std::map<uint16_t, std::string> map);
    bool PrepareEditDynamic();
    bool PrepareEditTrigger();
    bool PrepareEditArea();
    void FinishEditDynamic();
    void FinishEditTrigger();
    void FinishEditArea();

    static Dynamic* CreateDynamicFromType(uint16_t type, std::map <uint16_t, uint16_t> dynamicTypeMap, MapContainer* mapContainer, SoundCommand* soundCommand);
    static Dynamic* CreateDynamicFromType(uint16_t type, std::map <uint16_t, uint16_t> dynamicTypeMap, std::string name, MapContainer* mapContainer, SoundCommand* soundCommand);

    void EnumerateName(Dynamic* dynamic);
    
    bool SaveMap();
    bool SaveDynamics();
    bool SaveTriggers();
    bool SaveAreas();
    
    // ============= PUBLIC ==============
    public:

    HelperEngine(SDL_Window* window, uint32_t flags);
    ~HelperEngine();

    // initialization functions
    MapContainer* LoadMap(std::string mapName);
    bool LoadMapTextures(MapContainer* mapContainer);
    bool LoadGame(std::string gameName);
    bool LoadAudio();
    void InitSoundMap();
    static std::vector<std::vector<std::string>> LoadConfig(std::string filename);

    static bool SaveDynamics(std::string mapName, std::list<Dynamic*> dynamicList);
    static bool SaveTriggers(std::string mapName, std::list<Trigger*> triggerList);
    static bool SaveAreas(std::string mapName, std::list<Area*> areaList);

    
    // get/add/set functions
    MapContainer* GetMapContainer();

    void AddDynamic(Dynamic* dynamic, MapContainer* mapContainer);
    void AddTrigger(Trigger* trigger, MapContainer* mapContainer);
    void AddArea(Area* area, MapContainer* mapContainer);
    void AddMap(MapContainer* mapContainer);

    void SetPlayer(Player* player, MapContainer* mapContainer);
    void SetMap(std::string name);
    void SetMap(MapContainer* mapContainer);

    void SwitchMap(std::string name);
    void SwitchMap(MapContainer* mapContainer);
    void SetMusicVolume(uint8_t volume);

    void RemoveDynamic(Dynamic* dynamic);
    void RemoveTrigger(Trigger* trigger);
    void RemoveArea(Area* area);
    void RemoveMap(MapContainer* mapContainer);

    void DeleteMap(MapContainer* mapContainer);
    bool DynamicTitleExist(std::string title);

    Dynamic* GetDynamicByCoord(uint32_t x, uint32_t y);
    Dynamic* GetDynamicByName(std::string name);

    // runtime functions
    int Run();
    void MoveCamera();
    void Controls();
    void ControlsEdit();

    void ControlsTextInput();
    void EditMouseScroll();
    void ControlsGameplay();

    bool PlayMusic(uint8_t musicIndex);
    bool PlaySounds();
    void RunDynamic();
    void AskQuit();

    // draw functions
    void DrawMap();
    void DrawDynamic();
    void DrawText(TTF_Font* font, std::string text, SDL_Rect* textPosition, SDL_Colour* fontColour);
    void DrawInventory();

    void DrawInventoryActiveItem();
    void ShowHealth();

    void DrawAreas();
    void DrawTriggers();

    // info functions
    void EditorInfo();
    void ShowFPS();
    void ShowPos();
    void ShowMessage();

    bool SetDynamicConfig(std::vector<std::string> configVector);

    // create methods
    Dynamic* CreateDynamic(std::vector<std::string> dynamicVector);
    Trigger* CreateTrigger(std::vector<std::string> triggerVector, MapContainer* mapContainer);
    Item* CreateItemConfig(std::vector<std::string> itemVector);
    
    Area* CreateArea(std::vector<std::string> areaVector, MapContainer* mapContainer);
    MapContainer* CreateMap(std::string name, std::string tileset);

    static Dynamic* CreateDynamic(std::vector<std::string> dynamicVector, MapContainer* mapContainer, std::map <uint16_t, uint16_t> dynamicTypeMap, SoundCommand* SoundCommand, MessageCommand* messageCommand);
};