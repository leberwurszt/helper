// DRAW FUNCTIONS

#include "helperengine.h"

// Show frame rate on screen
void HelperEngine::ShowFPS()
{
    textPosition.x = 0;
    textPosition.y = 0;

    if(fpsDisplayNext <= SDL_GetTicks())
    {
        fpsShown = fps;
        fpsDisplayNext = SDL_GetTicks() + 1000;
    }

    DrawText(font, "FPS: " + std::to_string((int)fpsShown), &textPosition, &fontColour);
}

// Shows editor information
void HelperEngine::EditorInfo()
{
    textPosition.x = 0;
    textPosition.y = 0;

    if(editInputType)
        DrawText(font, editInputStrings[editInputType] + ": " + editInputTextBoxes[editInputType]->Show(), &textPosition, &fontColour);
        
    textPosition.y = FONT_SIZE;
    DrawText(font, "Type: " + std::to_string(tileType) + ", Value: " + std::to_string(tileValue), &textPosition, &fontColour);
    
    textPosition.y = FONT_SIZE * 2;
    DrawText(font, std::to_string(mouseTileX) + ", " + std::to_string(mouseTileY), &textPosition, &fontColour);
    
    textPosition.y = FONT_SIZE * 3;
    DrawText(font, "Floor: " + std::to_string(mapContainer->map->GetFloor(mouseTileX, mouseTileY)), &textPosition, &fontColour);

    textPosition.y = FONT_SIZE * 4;
    DrawText(font, "Wall: " + std::to_string(mapContainer->map->GetWall(mouseTileX, mouseTileY)), &textPosition, &fontColour);
    
    textPosition.y = FONT_SIZE * 5;
    DrawText(font, "Item: " + std::to_string(mapContainer->map->GetItem(mouseTileX, mouseTileY)) + " (" + mapContainer->itemConfig[mapContainer->map->GetItem(mouseTileX, mouseTileY)].name + ")", &textPosition, &fontColour);
    
    textPosition.y = FONT_SIZE * 6;
    DrawText(font, std::to_string(mapContainer->map->GetSizeX()) + " * " + std::to_string(mapContainer->map->GetSizeY()), &textPosition, &fontColour);
    
    imagePosition.x = screenSizeX - tilesizeX;
    imagePosition.y = 0;

    switch(tileType)
    {
        case 0: // Floor
            SDL_RenderCopy(render, mapContainer->textureFloor, &tile[tileValue], &imagePosition);
            break;

        case 1: // Wall
            SDL_RenderCopy(render, mapContainer->textureWall, &tile[tileValue], &imagePosition);
            break;

        case 2: // Item
            SDL_RenderCopy(render, mapContainer->textureItem, &tile[tileValue], &imagePosition);
            break;

        case 3: // Dynamic
            SDL_RenderCopy(render, dynamicTextures[tileValue], &tile[1], &imagePosition);
            break;
    }

    imagePosition.x = (mouseTileX - cameraX) * tilesizeX + cameraOffsetX;
    imagePosition.y = (mouseTileY - cameraY) * tilesizeY + cameraOffsetY;
    
    switch(tileType)
    {
        case 0: // Floor
            SDL_SetTextureAlphaMod(mapContainer->textureFloor, 127);
            SDL_RenderCopy(render, mapContainer->textureFloor, &tile[tileValue], &imagePosition);
            SDL_SetTextureAlphaMod(mapContainer->textureFloor, 255);
            break;

        case 1: // Wall
            SDL_SetTextureAlphaMod(mapContainer->textureWall, 127);
            SDL_RenderCopy(render, mapContainer->textureWall, &tile[tileValue], &imagePosition);
            if(!wallAlpha)
                SDL_SetTextureAlphaMod(mapContainer->textureWall, 255);
            break;

        case 2: // Item
            SDL_SetTextureAlphaMod(mapContainer->textureItem, 127);
            SDL_RenderCopy(render, mapContainer->textureItem, &tile[tileValue], &imagePosition);
            SDL_SetTextureAlphaMod(mapContainer->textureItem, 255);
            break;

        case 3: // Dynamic
            SDL_SetTextureAlphaMod(dynamicTextures[tileValue], 127);
            SDL_RenderCopy(render, dynamicTextures[tileValue], &tile[1], &imagePosition);
            SDL_SetTextureAlphaMod(dynamicTextures[tileValue], 255);
    }
    
}

// Shows position of player
void HelperEngine::ShowPos()
{
    textPosition.x = 0;
    textPosition.y = FONT_SIZE;
    DrawText(font, "X: " + std::to_string(mapContainer->player->GetX()) + ", Y: " + std::to_string(mapContainer->player->GetY()), &textPosition, &fontColour);
}

// Draw map depending on camera position
void HelperEngine::DrawMap()
{
    int16_t x;
    int16_t y;

    imagePosition.w = tilesizeX;
    imagePosition.h = tilesizeY;

    for(int16_t i = -2; i < VIEWSIZE_Y + 2; ++i)
        for(int16_t k = -2; k < VIEWSIZE_X + 2; ++k)
        {
            x = k + cameraX;
            y = i + cameraY;

            imagePosition.x = k * tilesizeX + cameraOffsetX;
            imagePosition.y = i * tilesizeY + cameraOffsetY;
            
            SDL_RenderCopy(render, mapContainer->textureFloor, &tile[mapContainer->map->GetFloor(x, y)], &imagePosition);
            SDL_RenderCopy(render, mapContainer->textureWall, &tile[mapContainer->map->GetWall(x, y)], &imagePosition);
            SDL_RenderCopy(render, mapContainer->textureItem, &tile[mapContainer->map->GetItem(x, y)], &imagePosition);
        }
}

// Draw dynamic objects, e.g. player, enemies... 
void HelperEngine::DrawDynamic()
{
    imagePosition.w = tilesizeX;
    imagePosition.h = tilesizeY;

    for(Dynamic* dynamic : mapContainer->dynamicList) 
    {
            imagePosition.x = ((int) dynamic->GetX() - cameraX) * tilesizeX + cameraOffsetX - ((double) dynamic->GetMovementX() * dynamic->GetMovementPercentage() * tilesizeX);
            imagePosition.y = ((int) dynamic->GetY() - cameraY) * tilesizeY + cameraOffsetY - ((double) dynamic->GetMovementY() * dynamic->GetMovementPercentage() * tilesizeY);   

            SDL_RenderCopy(render, dynamicTextures[dynamic->GetType()], &tile[dynamic->GetSprite()], &imagePosition);
    }
}

void HelperEngine::DrawAreas()
{
    SDL_Rect areaRect;
    SDL_Surface* areaSurface = NULL;
    SDL_Texture* areaTexture = NULL;

    for(Area* area : mapContainer->areaList) 
    {
        areaRect.x = ((int) area->GetLowerX() - cameraX) * tilesizeX + cameraOffsetX;
        areaRect.y = ((int) area->GetLowerY() - cameraY) * tilesizeY + cameraOffsetY;

        areaRect.w = (area->GetUpperX() - area->GetLowerX() + 1) * tilesizeX;
        areaRect.h = (area->GetUpperY()- area->GetLowerY() + 1) * tilesizeY ;

        areaSurface = SDL_CreateRGBSurface(0, areaRect.w, areaRect.h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
        SDL_FillRect(areaSurface, NULL, 0x400000ff);
        areaTexture = SDL_CreateTextureFromSurface(render, areaSurface);

        SDL_RenderCopy(render, areaTexture, NULL, &areaRect);
        SDL_FreeSurface(areaSurface);
        SDL_DestroyTexture(areaTexture);

    }
}

void HelperEngine::DrawTriggers()
{
    for(Trigger* trigger : mapContainer->triggerList)
    {
        imagePosition.x = ((int) trigger->GetX() - cameraX) * tilesizeX + cameraOffsetX;
        imagePosition.y = ((int) trigger->GetY() - cameraY) * tilesizeY + cameraOffsetY;   

        SDL_RenderCopy(render, textureTrigger, NULL, &imagePosition);
    }
}

// Draw text on window
void HelperEngine::DrawText(TTF_Font* font, std::string text, SDL_Rect* textPosition, SDL_Colour* fontColour)
{
    if(text.length() > 0)
    {
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), *fontColour);
        textPosition->h = textSurface->h;
        textPosition->w = textSurface->w;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);
        SDL_RenderCopy(render, textTexture, NULL, textPosition);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

// Active Item in inventory
void HelperEngine::DrawInventoryActiveItem()
{
    Item* activeItem = mapContainer->player->GetFromInventory(inventoryIndex);
    if(activeItem != NULL)
    {
        imagePosition.x = 0;
        imagePosition.y = screenSizeY - tilesizeY;

        SDL_RenderCopy(render, mapContainer->textureItem, &tile[activeItem->index], &imagePosition);
        textPosition.x = tilesizeX * 1.25;

        textPosition.y = screenSizeY - (tilesizeY / 2 + FONT_SIZE / 2);
        DrawText(font, activeItem->name.c_str(), &textPosition, &fontColour);
    }
}

void HelperEngine::DrawInventory()
{
    for(int i = 0; i < PLAYER_INVENTORY_MAX_SIZE_Y; i++)
         for(int k = 0; k < PLAYER_INVENTORY_MAX_SIZE_X; k++)
        {
            Item* activeItem = mapContainer->player->GetFromInventory(k * PLAYER_INVENTORY_MAX_SIZE_Y + i);
            imagePosition.x = i * tilesizeX;
            imagePosition.y = k * tilesizeY;

            SDL_RenderCopy(render, inventoryTexture, NULL, &imagePosition);

            if(activeItem != NULL)
            {
                if(inventoryIndex == k * PLAYER_INVENTORY_MAX_SIZE_Y + i)
                    SDL_SetTextureAlphaMod(mapContainer->textureItem, 128);
                else
                    SDL_SetTextureAlphaMod(mapContainer->textureItem, SDL_ALPHA_OPAQUE);
                
                SDL_RenderCopy(render, mapContainer->textureItem, &tile[activeItem->index], &imagePosition);
            }
        }
    SDL_SetTextureAlphaMod(mapContainer->textureItem, SDL_ALPHA_OPAQUE);
}

// Show player's health
void HelperEngine::ShowHealth()
{
    std::string text = std::to_string(mapContainer->player->GetHealth());
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), fontColour);

    textPosition.h = textSurface->h;
    textPosition.w = textSurface->w;

    textPosition.x = screenSizeX - textPosition.w;
    textPosition.y = 0;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);

    SDL_RenderCopy(render, textTexture, NULL, &textPosition);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

// Ingame messages
void HelperEngine::ShowMessage()
{
    std::string messageTemp = messageCommand->PopMessage();

    if(messageTemp != "")
    {
        message = messageTemp;
        messageTime = SDL_GetTicks() + MESSAGE_TIME;
    }

    if(messageTime > SDL_GetTicks())
    {
        SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, message.c_str(), fontColour);
        textPosition.h = textSurface->h;
        textPosition.w = textSurface->w;

        textPosition.x = MESSAGE_POS_X;
        textPosition.y = MESSAGE_POS_Y;

        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);
        Uint8 alpha = (double)(messageTime - SDL_GetTicks()) / MESSAGE_TIME * 255;
        
        SDL_SetTextureAlphaMod(textTexture, alpha);
        SDL_RenderCopy(render, textTexture, NULL, &textPosition);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}