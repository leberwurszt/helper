// LOAD FILE FUNCTIONS

#include "helperengine.h"

//Load music
bool HelperEngine::LoadAudio()
{
    bool success = true;

    Mix_AllocateChannels(SOUND_CHANNELS);

    for(std::map<uint16_t, std::string>::iterator it = soundMap.begin(); it != soundMap.end(); it++)
    {
        sound[it->first] = Mix_LoadWAV((SOUND_PATH + it->second).c_str());

        if(sound[it->first] == NULL)
        {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
            success = false;  
        }
    }

    for(std::map<uint16_t, std::string>::iterator it = musicMap.begin(); it != musicMap.end(); it++)
    {
        music[it->first] = Mix_LoadMUS((MUSIC_PATH + it->second).c_str());

        if(sound[it->first] == NULL)
        {
            printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
            success = false;  
        }
    }
   
    return success;
}

MapContainer* HelperEngine::LoadMap(std::string mapName)
{    
    MapContainer* mapContainer = new MapContainer();

    ClearMap(mapContainer);

    mapContainer->map = new Map();

    if(mapContainer->map->Load(MAP_PATH + mapName + "/map.hex"))
    {
        for(std::vector<std::string> v : LoadConfig(MAP_PATH + mapName + "/dynamic.config"))
            AddDynamic(CreateDynamic(v, mapContainer,dynamicTypeMap, soundCommand, messageCommand), mapContainer);

        for(std::vector<std::string> v : LoadConfig(MAP_PATH + mapName + "/trigger.config"))
            AddTrigger(CreateTrigger(v, mapContainer), mapContainer);

        for(std::vector<std::string> v : LoadConfig(MAP_PATH + mapName + "/area.config"))
            AddArea(CreateArea(v, mapContainer), mapContainer);
        
        mapContainer->mapMusic = mapContainer->map->GetMapMusic();
        mapContainer->mapDir = mapName;
        
        int i = 0;

        for(std::vector<std::string> v : LoadConfig("items.config"))
            mapContainer->itemConfig[i++] = *CreateItemConfig(v);

        return mapContainer;
    }
    else
    {
        if(mapContainer != NULL)
        {
            DeleteMap(mapContainer);
        }
        return NULL;
    }
}

bool HelperEngine::LoadMapTextures(MapContainer* mapContainer)
{
    std::string tileset = mapContainer->map->GetTileset();

    SDL_Surface* surfaceWall = IMG_Load((TILESET_PATH + tileset + "/wall.png").c_str());
    SDL_Surface* surfaceFloor = IMG_Load((TILESET_PATH + tileset + "/floor.png").c_str());
    SDL_Surface* surfaceItem = IMG_Load((TILESET_PATH + tileset + "/item.png").c_str());

    mapContainer->textureWall = SDL_CreateTextureFromSurface(render, surfaceWall);
    mapContainer->textureFloor = SDL_CreateTextureFromSurface(render, surfaceFloor);
    mapContainer->textureItem = SDL_CreateTextureFromSurface(render, surfaceItem);

    SDL_FreeSurface(surfaceWall);
    SDL_FreeSurface(surfaceFloor);
    SDL_FreeSurface(surfaceItem);

    if(mapContainer->textureWall && mapContainer->textureFloor && mapContainer->textureItem)
        return true;
    else
        return false;
}

bool HelperEngine::LoadGame(std::string gameName)
{

    std::string type;
    std::string value;

    bool mapSet = false;

    for(std::vector<std::string> v : LoadConfig(gameName + "/game.config"))
    {
        if (v[0] == "game:")
            for(std::string line : v)
            {
                if(line != "" && line.at(line.length() - 1) != ':')
                {
                    std::istringstream iss(line);
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    transform(type.begin(), type.end(), type.begin(), ::tolower);

                    if(type == "start_map")
                    {
                        AddMap(LoadMap(value));
                        SetMap(value);
                        mapSet = true;
                    }
                }
            }

        if (v[0] == "music:")
        {
            for(std::string line : v)
            {
                if(line != "" && line.at(line.length() - 1) != ':')
                {
                    std::istringstream iss(line);
                    getline(iss, type, '=');
                    getline(iss, value, '=');

                    uint8_t musicValue = std::stoi(type);
                    
                    if(musicValue > 0 && musicValue < MAX_MUSIC_FILES)
                    {
                        musicMap.insert(std::pair<uint16_t, std::string>(musicValue, value));
                    }
                }
            }
            LoadAudio();
        }
    }

    if(!mapSet)
    {
        std::cerr << "No start map defined!" << std::endl;
        return false;
    }

    return true;
}

std::vector<std::vector<std::string>> HelperEngine::LoadConfig(std::string filename)
{
    bool success = true;
	std::ifstream file(filename);
    std::vector<std::vector<std::string>> mainVector;

	try
	{
        std::string line;
        std::vector<std::string> fileVector;
        std::vector<std::string> subVector;

        while(getline(file, line))
            fileVector.push_back(line);


        if(fileVector.size() > 1)
        {
            std::string type;
            std::string value;

            int i = 0;
            char last = '\0';
            line = fileVector.at(i++);
        
            while(i < fileVector.size())
            {
                while(i < fileVector.size() && line.length() == 0)
                    line = fileVector.at(i++);

                if(line.length() > 0)
                    last = line.at(line.length() - 1);

                if(last == ':')
                {
                    subVector.clear();
                    subVector.push_back(line);
                    last = '\0';

                    while(i < fileVector.size() && last != ':')
                    {
                        if(i < fileVector.size())
                        {
                            line = fileVector.at(i++);
                            while(i < fileVector.size() && line.length() == 0)
                                line = fileVector.at(i++);
                            if(line.length() > 0)
                                last = line.at(line.length() - 1);
                        }
                        if(last != ':')
                            subVector.push_back(line);
                    }

                    // Add dynamic to list
                    mainVector.push_back(subVector);
                }
            }
        }
    }
    catch(const std::exception& e)
	{
		std::cerr << "Error loading config file: " << e.what() << std::endl;
		success = false;
	}

	if (file.is_open())
		file.close();

	return mainVector;

    
}