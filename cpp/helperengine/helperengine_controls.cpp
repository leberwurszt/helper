// USER INPUT

#include "helperengine.h"

// Poll SDL events and player control
void HelperEngine::Controls()
{
    SDL_GetMouseState(&mouseX, &mouseY);
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT: // event window quit
            askQuit = true;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE: // Quit game instance
                if(editInputType)
                    editInputType = EDIT_INPUT_TYPE_NONE;
                else if(dynamicEdit)
                    FinishEditDynamic();
                else
                    askQuit = true;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }

        ControlsTextInput();
        if (configFlags & EDIT_MODE)
            ControlsEdit();
        else
            ControlsGameplay();
    }

    if (configFlags & EDIT_MODE)
        EditMouseScroll();
}

void HelperEngine::EditMouseScroll()
{

    if (mouseX <= 0)
    {
        cameraOffsetX += (SDL_GetTicks() - mouseScrollLast) / mouseScrollTime;
        cameraX -= cameraOffsetX / tilesizeX;
        cameraOffsetX %= tilesizeX;
    }
    else if (mouseX >= screenSizeX - 1)
    {
        cameraOffsetX -= (SDL_GetTicks() - mouseScrollLast) / mouseScrollTime;
        cameraX -= cameraOffsetX / tilesizeX;
        cameraOffsetX %= tilesizeX;
    }

    if (mouseY <= 0)
    {
        cameraOffsetY += (SDL_GetTicks() - mouseScrollLast) / mouseScrollTime;
        cameraY -= cameraOffsetY / tilesizeY;
        cameraOffsetY %= tilesizeY;
    }
    else if (mouseY >= screenSizeY - 1)
    {
        cameraOffsetY -= (SDL_GetTicks() - mouseScrollLast) / mouseScrollTime;
        cameraY -= cameraOffsetY / tilesizeY;
        cameraOffsetY %= tilesizeY;
    }

    mouseScrollLast = SDL_GetTicks();
}

void HelperEngine::ControlsEdit()
{
    int input = 0;
    int inputX = 0;
    int inputY = 0;
    mouseTileX = cameraX + (mouseX - cameraOffsetX) / tilesizeX;
    mouseTileY = cameraY + (mouseY - cameraOffsetY) / tilesizeY;
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    switch (event.type)
    {

    case SDL_KEYDOWN: // if key is pressed
        switch (event.key.keysym.sym)
        {
            // camera movement
            case SDLK_LEFT:
                cameraX--;
                break;

            case SDLK_RIGHT:
                cameraX++;
                break;

            case SDLK_UP:
                cameraY--;
                break;

            case SDLK_DOWN:
                cameraY++;
                break;

            case SDLK_MINUS:
                tilesizeX--;
                tilesizeY--;
                break;

            case SDLK_PLUS:
                tilesizeX++;
                tilesizeY++;
                break;

            case SDLK_F1:
                editInputType = EDIT_INPUT_TYPE_MAP_NAME;
                editInputTextBoxes[EDIT_INPUT_TYPE_MAP_NAME]->SetText(mapContainer->map->GetTitle());
            break;

            case SDLK_F2: // Turn on/off transparent walls
                if (!wallAlpha)
                {
                    wallAlpha = true;
                    SDL_SetTextureAlphaMod(mapContainer->textureWall, 127);
                }
                else
                {
                    wallAlpha = false;
                    SDL_SetTextureAlphaMod(mapContainer->textureWall, 255);
                }

                break;

            case SDLK_F3: // Change map size
                editInputType = EDIT_INPUT_TYPE_SIZE_X;
                editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_X]->SetText(std::to_string(mapContainer->map->GetSizeX()));
            break;

            case SDLK_F4: // fill the whole map
                FillMap(tileType, tileValue);
            break;

            case SDLK_F6:
                editInputType = EDIT_INPUT_TYPE_TILESET;
                editInputTextBoxes[EDIT_INPUT_TYPE_TILESET]->SetText(mapContainer->map->GetTileset());
            break;

            case SDLK_F7: // Decrease tile code value
                tileValue--;
                std::cout << "Tile code set to " << tileValue << std::endl;
                break;

            case SDLK_F8: // Increase tile code value
                tileValue++;
                std::cout << "Tile code set to " << tileValue << std::endl;
                break;

            case SDLK_F9: // Set tile type to 0 (floor)
                tileType = 0;
                std::cout << "Tile type set to floor" << std::endl;
                break;

            case SDLK_F10: // Set tile type to 1 (wall)
                tileType = 1;
                std::cout << "Tile type set to wall" << std::endl;
                break;

            case SDLK_F11: // Set tile type to 2 (item)
                tileType = 2;
                std::cout << "Tile type set to item" << std::endl;
                break;

            default:
                break;
        }
        break;

    case SDL_MOUSEWHEEL:
        if (event.wheel.y > 0)
            tileValue++;
        else if (event.wheel.y < 0)
            tileValue--;
        break;

    case SDL_MOUSEBUTTONDOWN:
        switch (event.button.button)
        {
        case SDL_BUTTON_RIGHT:
            dynamicMove = GetDynamicByCoord(mouseTileX, mouseTileY);
            break;

        case SDL_BUTTON_LEFT:
            if (askQuit)
            {
            }

            if (editDynamic)
            {
                editDynamicInput = -1;
                for (uint16_t i = 0; i < dynamicValues.size(); i++)
                {
                    if (
                        mouseX > dynamicEditTableValue[i].x && mouseX < (dynamicEditTableValue[i].x + dynamicEditTableValue[i].w) && mouseY > dynamicEditTableValue[i].y && mouseY < (dynamicEditTableValue[i].y + dynamicEditTableValue[i].h))
                    {
                        uint16_t k = 0;
                        for (std::map<uint16_t, std::string>::iterator it = dynamicValues.begin(); it != dynamicValues.end(); ++it, ++k)
                            if (k == i)
                                editDynamicInput = it->first;
                    }
                }
            }
            break;

        case SDL_BUTTON_X1:
            tileType++;
            break;

        case SDL_BUTTON_X2:
            tileType--;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    if(!editDynamic && !editInputType)
    {
        switch (event.type)
        {

        case SDL_KEYDOWN: // if key is pressed
            switch (event.key.keysym.sym)
            {
                case SDLK_s: // Save map
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL])
                {
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT])
                    {
                        editInputTextBoxes[EDIT_INPUT_TYPE_SAVE_MAP]->SetText(mapContainer->mapDir);
                        editInputType = EDIT_INPUT_TYPE_SAVE_MAP;
                    }
                    else
                        SaveMap();
                }
                break;

            case SDLK_o: // Open map
                if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL])
                {
                    editInputType = EDIT_INPUT_TYPE_OPEN_MAP;
                    editInputTextBoxes[EDIT_INPUT_TYPE_OPEN_MAP]->Clear();
                }
                break;

            case SDLK_e: // Edit dynamic
                FinishEditDynamic();
                editDynamic = PrepareEditDynamic();
                break;

            case SDLK_r: // Remove dynamic
                RemoveDynamic(GetDynamicByCoord(mouseTileX, mouseTileY));
                break;

            case SDLK_b: // Copy dynamic
                dynamicCopy = GetDynamicByCoord(mouseTileX, mouseTileY);
                break;

            case SDLK_m: // Paste dynamic
                if (dynamicCopy != NULL)
                {
                    Dynamic *newDynamic = new Dynamic(*dynamicCopy);
                    EnumerateName(newDynamic);
                    newDynamic->SetPos(mouseTileX, mouseTileY);
                    AddDynamic(newDynamic, mapContainer);
                }
                break;
            }
        }
        // left mouse: place tile on mouse position
        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            switch (tileType)
            {
            case 0: // Floor
                mapContainer->map->SetFloor(mouseTileX, mouseTileY, tileValue);
                break;

            case 1: // Wall
                mapContainer->map->SetWall(mouseTileX, mouseTileY, tileValue);
                break;

            case 2: // Item
                mapContainer->map->SetItem(mouseTileX, mouseTileY, tileValue);
                break;

            case 3: // Dynamic
                Dynamic *newDynamic = CreateDynamicFromType(tileValue, dynamicTypeMap, mapContainer, soundCommand);
                EnumerateName(newDynamic);
                newDynamic->SetPos(mouseTileX, mouseTileY);
                newDynamic->SetType((DynamicType)tileValue);
                AddDynamic(newDynamic, mapContainer);
                break;
            }
        }

        // right mouse: remove tile on mouse position
        if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            if (dynamicMove != NULL)
                dynamicMove->SetPos(mouseTileX, mouseTileY);
            else
                switch (tileType)
                {
                case 0: // Floor
                    mapContainer->map->SetFloor(mouseTileX, mouseTileY, 0);
                    break;

                case 1: // Wall
                    mapContainer->map->SetWall(mouseTileX, mouseTileY, 0);
                    break;

                case 2: // Item
                    mapContainer->map->SetItem(mouseTileX, mouseTileY, 0);
                    break;
                }
        }
        else
            dynamicMove = NULL;
    }
}

void HelperEngine::ControlsGameplay()
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    switch (event.type)
    {
    case SDL_KEYDOWN: // if key is pressed
        switch (event.key.keysym.sym)
        {

        case SDLK_n: // noclip on/off
            if (mapContainer->player->GetNoclip())
                mapContainer->player->SetNoclip(false);
            else
                mapContainer->player->SetNoclip(true);
            break;

        case SDLK_m: // godmode (indestructable player)
            if (!godMode)
            {
                mapContainer->player->SetDestructable(false);
                godMode = true;
            }
            else
            {
                mapContainer->player->SetDestructable(true);
                godMode = false;
            }
            break;

        // Player movement
        case SDLK_a:
            if (configFlags & EDIT_MODE && state[SDL_SCANCODE_LSHIFT])
                mapContainer->player->Move(-10, 0);
            else
                mapContainer->player->Move(-1, 0);
            break;

        case SDLK_d:
            if (configFlags & EDIT_MODE && state[SDL_SCANCODE_LSHIFT])
                mapContainer->player->Move(10, 0);
            else
                mapContainer->player->Move(1, 0);
            break;

        case SDLK_w:
            if (configFlags & EDIT_MODE && state[SDL_SCANCODE_LSHIFT])
                mapContainer->player->Move(0, -10);
            else
                mapContainer->player->Move(0, -1);
            break;

        case SDLK_s:
            if (configFlags & EDIT_MODE && state[SDL_SCANCODE_LSHIFT])
                mapContainer->player->Move(0, 10);
            else
                mapContainer->player->Move(0, 1);
            break;

        // Player attack
        case SDLK_LEFT:
            if (!mapContainer->player->Use(mapContainer->player->GetX() - 1, mapContainer->player->GetY()))
                ;
            mapContainer->player->Attack(mapContainer->player->GetX() - 1, mapContainer->player->GetY(), PLAYER_DAMAGE);
            break;

        case SDLK_RIGHT:
            if (!mapContainer->player->Use(mapContainer->player->GetX() + 1, mapContainer->player->GetY()))
                ;
            mapContainer->player->Attack(mapContainer->player->GetX() + 1, mapContainer->player->GetY(), PLAYER_DAMAGE);

            break;

        case SDLK_UP:
            if (!mapContainer->player->Use(mapContainer->player->GetX(), mapContainer->player->GetY() - 1))
                ;
            mapContainer->player->Attack(mapContainer->player->GetX(), mapContainer->player->GetY() - 1, PLAYER_DAMAGE);

            break;

        case SDLK_DOWN:
            if (!mapContainer->player->Use(mapContainer->player->GetX(), mapContainer->player->GetY() + 1))
                ;
            mapContainer->player->Attack(mapContainer->player->GetX(), mapContainer->player->GetY() + 1, PLAYER_DAMAGE);

            break;

        // Inventory control
        case SDLK_1:
            if (inventoryIndex > 0)
                inventoryIndex--;
            break;

        case SDLK_2:
            if (inventoryIndex < mapContainer->player->GetInventoryCount() - 1)
                inventoryIndex++;
            break;

        case SDLK_3:
            mapContainer->player->UseFromInventory(inventoryIndex);
            if (inventoryIndex > mapContainer->player->GetInventoryCount() - 1 && inventoryIndex > 0)
                inventoryIndex--;
            break;

        case SDLK_4:
            mapContainer->player->TakeItem();
            break;

        case SDLK_i:
            if (!showInventory)
            {
                soundCommand->PlaySound(OPEN_INVENTORY);
                showInventory = true;
            }
            else
                showInventory = false;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}

void HelperEngine::ControlsTextInput()
{
    switch (event.type)
    {
    case SDL_TEXTINPUT:
        
        if (editDynamicInput >= 0)
            editDynamicTextBoxes[editDynamicInput]->Insert(event.text.text);

        if(editInputType)
                editInputTextBoxes[editInputType]->Insert(event.text.text);
    break;

    case SDL_KEYDOWN: // if key is pressed
        switch (event.key.keysym.sym)
        {
        case SDLK_BACKSPACE:
            if (editDynamicInput >= 0)
            {
                editDynamicTextBoxes[editDynamicInput]->Delete(1);
            }
            if(editInputType)
            {
                editInputTextBoxes[editInputType]->Delete(1);
            }

        break;

        case SDLK_LEFT:
            if (editDynamicInput >= 0)
            {
                editDynamicTextBoxes[editDynamicInput]->MovePointer(-1);
            }
            if(editInputType)
            {
                editInputTextBoxes[editInputType]->MovePointer(-1);
            }
        break;

        case SDLK_RIGHT:
            if (editDynamicInput >= 0)
            {
                editDynamicTextBoxes[editDynamicInput]->MovePointer(1);
            }
            if(editInputType)
            {
                editInputTextBoxes[editInputType]->MovePointer(1);
            }
        break;


        case SDLK_RETURN:
            if (editDynamic)
                FinishEditDynamic();

            if(editInputType)
            {
                switch(editInputType)
                {
                    case EDIT_INPUT_TYPE_TILESET:
                        mapContainer->map->SetTileset(editInputTextBoxes[EDIT_INPUT_TYPE_TILESET]->GetText());
                        ClearMapTextures(mapContainer);
                        LoadMapTextures(mapContainer);
                        editInputType = EDIT_INPUT_TYPE_NONE;
                    break;

                    case EDIT_INPUT_TYPE_MAP_NAME:
                        mapContainer->map->SetTitle(editInputTextBoxes[EDIT_INPUT_TYPE_MAP_NAME]->GetText());
                        editInputType = EDIT_INPUT_TYPE_NONE;
                    break;

                    case EDIT_INPUT_TYPE_SIZE_X:
                        editInputType = EDIT_INPUT_TYPE_SIZE_Y;
                        editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_Y]->SetText(std::to_string(mapContainer->map->GetSizeY()));
                    break;

                    case EDIT_INPUT_TYPE_SIZE_Y:
                        mapContainer->map->Resize(std::stoi(editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_X]->GetText()), std::stoi(editInputTextBoxes[EDIT_INPUT_TYPE_SIZE_Y]->GetText()));
                        std::cout << "New map size is " << mapContainer->map->GetSizeX() << " * " << mapContainer->map->GetSizeY() << std::endl;
                        editInputType = EDIT_INPUT_TYPE_NONE;
                    break;

                    case EDIT_INPUT_TYPE_OPEN_MAP:
                        MapContainer* newMap;
                        newMap = LoadMap(editInputTextBoxes[EDIT_INPUT_TYPE_OPEN_MAP]->GetText());
                        if(newMap)
                        {
                            DeleteMap(mapContainer);
                            SetMap(newMap);
                        }
                        else
                        {
                            messageCommand->PushMessage("Map " + editInputTextBoxes[EDIT_INPUT_TYPE_OPEN_MAP]->GetText() + " not found!");
                        }
                        editInputType = EDIT_INPUT_TYPE_NONE;
                    break;

                    case EDIT_INPUT_TYPE_SAVE_MAP:
                        mapContainer->mapDir = editInputTextBoxes[EDIT_INPUT_TYPE_SAVE_MAP]->GetText();
                        SaveMap();
                        editInputType = EDIT_INPUT_TYPE_NONE;
                    break;

                }
                
            }



            break;
        }
        break;
    }
}