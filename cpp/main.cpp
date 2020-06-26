// HELPER QUEST
// Apu Apustaja

// main.cpp

#include "helperengine/helperengine.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

int Run(HelperEngine* engine);
int Menu(SDL_Window* window);
void HelpText();

int main(int argc, char *argv[])
{

    int screenSizeX = 640;
    int screenSizeY = 480;

    int screenRefresh = 60;
    int screenPixelFormat = 32;

    int musicVolume = 128;

    bool screenFull = false;
    bool editMode = false;
    bool showFPS = false;
    bool playMusic = true;
    bool playSounds = true;
    bool startGame = true;
    bool accelerated = true;
    bool vsync = true;

    uint32_t flags = 0x00000000;
    MapContainer* mapContainer = NULL;

    std::string mapName = "";
    std::string gameName = "basehelper";
    HelperEngine* helper = NULL;

    // search arguments
    for(int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];

        if(arg == "-m" || arg == "--map")                                       // map name
            if(i < argc - 1)
            {
                startGame = false;
                mapName = argv[++i];
            }
            else
            {
                std::cerr << arg << " requires one argument." << std::endl;
                return -1;
            }

        else if(arg == "-e" || arg == "--edit")                                 // edit mode
            editMode = true;

        else if(arg == "-r" || arg == "--framerate")                            // show framerate
            showFPS = true;

        else if(arg == "-h" || arg == "--help")                                 // help
        {                             
            HelpText();
            return 0;
        }

        else if(arg == "-g" || arg == "--game")                                 // help
        {                             
            startGame = true;
            if(i < argc - 1)
                gameName = argv[++i];
            else
                gameName = "basehelper";
            
        }

        else if(arg == "-n" || arg == "--nomusic")
        {
            playMusic = false;
        }

        else                                                                    // invalid argument
        {
            std::cerr << arg << " is not a valid argument." << std::endl;
            return -1;
        }           
    }

    // enter map name
    if(mapName == "" && !startGame)
    {
        std::cout << "Map: ";
        std::cin >> mapName;
    }

    std::ifstream configFile("./settings.config");

	try
	{
        std::string line;
        std::vector<std::string> fileVector;

		while(getline(configFile,line))
            fileVector.push_back(line);
        
        std::string type;
        std::string value;

        for (std::string line : fileVector)
        {
            std::istringstream iss(line);

            getline(iss, type, '=');
            getline(iss, value, '=');

            transform(type.begin(), type.end(), type.begin(), ::tolower);
            transform(value.begin(), value.end(), value.begin(), ::tolower);

            if(type == "height")
                screenSizeY = std::stoi(value);

            if(type == "width")
                screenSizeX = std::stoi(value);

            if(type == "refresh_rate")
                screenRefresh = std::stoi(value);

            if(type == "colours")
                screenPixelFormat = std::stoi(value);

            if(type == "music_volume")
                musicVolume = std::stoi(value);

            if(type == "fullscreen")
                screenFull = (value == "yes") ? true : false;

            if(type == "vsync")
                vsync = (value == "yes") ? true : false;

            if(type == "opengl")
                accelerated = (value == "yes") ? true : false;

            if(type == "music")
                playMusic = (value == "yes") ? true : false;

            if(type == "sounds")
                playSounds = (value == "yes") ? true : false; 
        }
    }

	catch(const std::exception& e)
	{
		std::cerr << "Unable to load config file" << std::endl << e.what() << std::endl;
    }

    if (configFile.is_open())
	    configFile.close();

    // set flags
    flags += showFPS ? SHOW_FPS : 0;
    flags += playMusic ? PLAY_MUSIC : 0;
    flags += playSounds ? PLAY_SOUNDS : 0;
    flags += editMode ? EDIT_MODE : 0;
    flags += accelerated ? ACCELERATION : 0;
    flags += vsync ? VSYNC_ON : 0;

    // initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    //Set texture filtering to linear
    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0" ))
        std::cerr << "Warning: Linear texture filtering not enabled!" << std::endl;

    //Initialize SDL_mixer
    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    // initialize window
    SDL_Window* window = SDL_CreateWindow("Helper Quest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenSizeX, screenSizeY, SDL_WINDOW_OPENGL * accelerated);
    if (!window)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    if(screenFull)
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // initialize Engine
    helper = new HelperEngine(window, flags);

    if(helper != NULL)
    {
        if(!startGame)
        {
            // load map
            mapContainer = helper->LoadMap(mapName);

            if (mapContainer == NULL && editMode)
            {
                std::cout << "Map " << mapName << " does not exist, creating..." << std::endl;
                std::string tileset;

                std::cout << "Tileset: ";
                std::cin >> tileset;

                mapContainer = helper->CreateMap(mapName, tileset);
                std::cout << "Map created." << std::endl;
            }

            if (mapContainer == NULL)
            {
                std::cerr << "error loading map" << std::endl;
                return -1;
            }
            else
            {
                std::cout << "Hanlo" << std::endl;
                helper->AddMap(mapContainer);

                std::cout << "Hanloo" << std::endl;
                helper->SetMap(mapContainer);

                std::cout << "Hanlooo" << std::endl;
                helper->SetMusicVolume(musicVolume);
                std::cout << "Hanloooo" << std::endl;
                
                Run(helper);
            }
        }
        else
        {
            helper->LoadGame("basehelper");
            Run(helper);
        }     
    }

    // Exit
    SDL_DestroyWindow(window);
    SDL_Quit();

    delete helper;
    std::cout << "FUG :DDDDDDDD" << std::endl;

    return 0;
}

int Run(HelperEngine* engine)
{
    return engine->Run();
}

int Menu(SDL_Window* window)
{
    return 0;
}

void HelpText()
{
    std::cout <<
        "Usage: helper [options]" << std::endl <<
        "Options:" << std::endl <<
        "  -h, --help\t\tshow this help text" << std::endl <<
        "  -e, --edit\t\tmap editor" << std::endl <<
        "  -m MAP, --map MAP\tselect map" << std::endl <<
        "  -g GAME, --game GAME\tstart a game (basehelper is default)" << std::endl <<
        std::endl
    ;
}