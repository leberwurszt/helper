// EDITOR FUNCTIONS

#include "helperengine.h"

void HelperEngine::FinishEditDynamic()
{
    editDynamicInput = -1;
    editDynamic = false;
    
    for(std::map<uint16_t, std::string>::iterator it = dynamicValues.begin(); it != dynamicValues.end(); ++it)
        it->second = editDynamicTextBoxes[it->first]->GetText();
    if(dynamicEdit != NULL)
        dynamicEdit->SetValues(dynamicValues);
    dynamicValues.clear();
    dynamicEdit = NULL;

    if(editDynamicTextBoxes != NULL)
    {
        for(int i = 0; i < 256; i++)
            if(editDynamicTextBoxes[i] != NULL)
            {
                delete editDynamicTextBoxes[i];
                editDynamicTextBoxes[i] = NULL;
            }
        delete editDynamicTextBoxes;
        editDynamicTextBoxes = NULL;
    }
}

// Prepare edit mode for dynamic
bool HelperEngine::PrepareEditDynamic()
{
    dynamicEdit = GetDynamicByCoord(mouseTileX, mouseTileY);

    if(dynamicEditTableDescription != NULL)
    {
        delete[] dynamicEditTableDescription;
        dynamicEditTableDescription = NULL;
    }

    if(dynamicEditTableValue != NULL)
    {
        delete[] dynamicEditTableValue;
        dynamicEditTableValue = NULL;
    }

    if(dynamicEdit != NULL)
    {
        dynamicValues = dynamicEdit->GetValues();
        dynamicEditTableDescription = new SDL_Rect[dynamicValues.size()];
        dynamicEditTableValue = new SDL_Rect[dynamicValues.size()];

        
        int i = 0;
        editDynamicTextBoxes = new Textbox*[256];
        for(int i = 0; i < 256; i++)
            editDynamicTextBoxes[i] = NULL;

        for(std::map<uint16_t, std::string>::iterator it = dynamicValues.begin(); it != dynamicValues.end(); ++it, ++i)
        {
            dynamicEditTableDescription[i].h = FONT_SIZE;
            dynamicEditTableDescription[i].w = FONT_SIZE * 14;
            dynamicEditTableDescription[i].x = 0;
            dynamicEditTableDescription[i].y = i * FONT_SIZE;

            dynamicEditTableValue[i].h = FONT_SIZE;
            dynamicEditTableValue[i].w = FONT_SIZE * 16;
            dynamicEditTableValue[i].x = dynamicEditTableDescription[i].w;
            dynamicEditTableValue[i].y = i * FONT_SIZE;

            editDynamicTextBoxes[it->first] = new Textbox();
            editDynamicTextBoxes[it->first]->SetText(it->second);
        }

        return true;
    }
    else
    {
        FinishEditDynamic();
        return false;
    }
}

// edit menu for dynamic
void HelperEngine::EditDynamic(Dynamic* dynamic)
{
    if(!dynamicEdit)
    {
        editDynamic = false;
        return;
    }
    std::map<uint16_t, std::string>::iterator it = dynamicValues.begin();

    SDL_Rect rectText;
    int i = 0;
    for(std::map<uint16_t, std::string>::iterator it = dynamicValues.begin(); it != dynamicValues.end(); ++it, ++i)
    {
        SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(render, &dynamicEditTableDescription[i]);

        SDL_SetRenderDrawColor(render, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(render, &dynamicEditTableDescription[i]);

        SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);
        rectText = dynamicEditTableDescription[i];
        std::string description = "";
        switch(it->first)
        {
            case DYNAMIC_MAP_TITLE:
                description = "title";
                break;
            case DYNAMIC_MAP_X:
                description = "x";
                break; 
            case DYNAMIC_MAP_Y:
                description = "y";
                break;
            case DYNAMIC_MAP_TYPE:
                description = "type";
                break;
            case DYNAMIC_MAP_STATE:
                description = "state";
                break;
            case DYNAMIC_MAP_HEALTH:
                description = "health";
                break;
            case DYNAMIC_MAP_MAXHEALTH:
                description = "max_health";
                break;
            case DYNAMIC_MAP_FLYING:
                description = "flying";
                break;
            case DYNAMIC_MAP_OBSTACLE:
                description = "obstacle";
                break;
            case DYNAMIC_MAP_DESTRUCTABLE:
                description = "destructable";
                break;
            case DYNAMIC_MAP_DIRECTION:
                description = "direction";
                break;
            case DYNAMIC_MAP_DAMAGE:
                description = "damage";
                break;
            case DYNAMIC_MAP_SPEED:
                description = "speed";
                break;
            case DYNAMIC_MAP_ATTACKSPEED:
                description = "attack_speed";
                break;
            case DOOR_MAP_OPEN_TYPE:
                description = "open_type";
                break;
            case DOOR_MAP_KEY:
                description = "key";
                break;
            case GONDOLA_MAP_MUSIC:
                description = "music";
                break;

            default:
                description = "invalid";
                break;
        }
        

        DrawText(font, description, &rectText, &fontColour);

        
        std::string value;
        if(editDynamicInput == it->first)
        {
            value = editDynamicTextBoxes[it->first]->Show();
            SDL_SetRenderDrawColor(render, 0, 0, 128, SDL_ALPHA_OPAQUE);
        }
        else
        {
            value = editDynamicTextBoxes[it->first]->GetText();
            SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
        }
        
        SDL_RenderFillRect(render, &dynamicEditTableValue[i]);

        SDL_SetRenderDrawColor(render, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(render, &dynamicEditTableValue[i]);


        SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);

        rectText = dynamicEditTableValue[i];
        DrawText(font, value, &rectText, &fontColour);
        
    }
    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
}
