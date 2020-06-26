// RUNTIME FUNCTIONS

#include "helperengine.h"

// Camera movement depending on player's position
void HelperEngine::MoveCamera()
{

    if(mapContainer->map->GetSizeX() > VIEWSIZE_X)
    {
        int16_t playerMovementX = mapContainer->player->GetMovementX();
        cameraOffsetX = (playerMovementX * mapContainer->player->GetMovementPercentage() * tilesizeX + screenTileModuloHalfX) + tilesizeX;

        cameraX = mapContainer->player->GetX() - VIEWSIZE_X / 2 - (cameraOffsetX / tilesizeX - 1);
        cameraOffsetX %= tilesizeX;

        if(cameraX == 0 && cameraOffsetX > 0)
            cameraOffsetX = 0;

        if(cameraX < 0)
        {
            cameraX = 0;
            cameraOffsetX = 0;
        }

        if((cameraX + VIEWSIZE_X) >= mapContainer->map->GetSizeX() && cameraOffsetX < screenTileModuloX)
            cameraOffsetX = screenTileModuloX;

        if((cameraX + VIEWSIZE_X) > mapContainer->map->GetSizeX())
        {
            cameraX = mapContainer->map->GetSizeX() - VIEWSIZE_X;
            cameraOffsetX = screenTileModuloX;
        }

    }
    else
    {
        cameraX = 0;
        cameraOffsetX = 0;
    }

    if(mapContainer->map->GetSizeY() > VIEWSIZE_Y)
    {
        int16_t playerMovementY = mapContainer->player->GetMovementY();
        cameraOffsetY = (playerMovementY * mapContainer->player->GetMovementPercentage() * tilesizeY + screenTileModuloHalfY) + tilesizeY;

        cameraY = mapContainer->player->GetY() - VIEWSIZE_Y / 2 - (cameraOffsetY / tilesizeY - 1);
        cameraOffsetY %= tilesizeY;


        if(cameraY == 0 && cameraOffsetY > 0)
            cameraOffsetY = 0;

        if(cameraY < 0)
        {
            cameraY = 0;
            cameraOffsetY = 0;
        }

        if((cameraY + VIEWSIZE_Y) >= mapContainer->map->GetSizeY() && cameraOffsetY < screenTileModuloY)
            cameraOffsetY = screenTileModuloY;

        if((cameraY + VIEWSIZE_Y) > mapContainer->map->GetSizeY())
        {
            cameraY = mapContainer->map->GetSizeY() - VIEWSIZE_Y;
            cameraOffsetY = screenTileModuloY;
        }
    }
    else
    {
        cameraY = 0;
        cameraOffsetY = 0;
    }

}

// run dynamics
void HelperEngine::RunDynamic()
{
    for(Dynamic* dynamic : mapContainer->dynamicList)
    {
        if(dynamic->GetState())
            dynamic->Do();
        else if (dynamic->GetTitle() != "player")
            RemoveDynamic(dynamic);
    }
}

// main loop
int HelperEngine::Run()
{
    // Check if render, font and map are loaded
    if (!render)
    {
        std::cerr << SDL_GetError() << std::endl;
        return -1;
    }

    if(!font)
    {
        std::cerr << TTF_GetError() << std::endl;
        return -1;
    }

    if(!mapContainer)
    {
        std::cerr << "No map loaded!" << std::endl;
        return -1;
    }

    // if edit mode is active, set initial camera position to player's position
    if(configFlags & EDIT_MODE)
    {
        cameraX = mapContainer->player->GetX() - VIEWSIZE_X / 2;
        cameraY = mapContainer->player->GetY() - VIEWSIZE_Y / 2;
    }

    run = true;
    
    while(run)
    {
        SDL_RenderClear(render); // Clear Screen
        soundCommand->SetDest(mapContainer->player->GetX(), mapContainer->player->GetY());

        if(!(configFlags & EDIT_MODE))
        {
            RunDynamic();
            
            for(Trigger* trigger : mapContainer->triggerList)
            {
                trigger->Do();
                if(trigger->GetType() | TRIGGER_SWITCH_MAP && trigger->IsActive())
                {
                    SwitchMap(trigger->GetNewMap());
                    mapContainer->player->SetPos(trigger->GetNewX(), trigger->GetNewY());

                    if(musicPlaying != mapContainer->mapMusic)
                        musicPlaying = 0;
                }
            }
            for(Area* area : mapContainer->areaList)
                area->Do();
            MoveCamera();
        }

        Controls();
        DrawMap();
        DrawDynamic();

        if(showInventory)
            DrawInventory();

        DrawInventoryActiveItem();

        ShowHealth();

        // Additional info on screen

        if(configFlags & SHOW_FPS)
            ShowFPS();
        
        if(configFlags & EDIT_MODE)
        {
            DrawTriggers();
            DrawAreas();
            EditorInfo();

            if(editDynamic)
                EditDynamic(dynamicEdit);
        }

        ShowMessage();

        if(askQuit)
            AskQuit();

        SDL_RenderPresent(render); // Confirm drawing on screen

        // play ingame music and sounds
        if(configFlags & PLAY_SOUNDS)
            PlaySounds();

        if(configFlags & PLAY_MUSIC)
        {
            if(musicPlaying == 0)
                PlayMusic(mapContainer->mapMusic);
            uint8_t popMusic = soundCommand->PopMusic();
            if(popMusic)
            {
                PlayMusic(popMusic);
            }
        }


        // calculate framerate
        fpsTicksLast = fpsTicks;

        fpsTicks = std::chrono::high_resolution_clock::now();
        fps = 1. / std::chrono::duration_cast<std::chrono::duration<double>>(fpsTicks - fpsTicksLast).count();
    }
    return 0;
}

// Ask user whether to quit
void HelperEngine::AskQuit()
{
    run = false;
}