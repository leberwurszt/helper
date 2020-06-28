// EDITOR FUNCTIONS

#include "helperengine.h"

// Prepare edit mode for dynamic
bool HelperEngine::PrepareEditDynamic()
{
    dynamicEdit = Dynamic::GetDynamicByCoord(mouseTileX, mouseTileY, mapContainer);

    if(editTableDescription)
    {
        delete[] editTableDescription;
        editTableDescription = NULL;
    }

    if(editTableValue)
    {
        delete[] editTableValue;
        editTableValue = NULL;
    }

    if(dynamicEdit)
    {
        editValues = dynamicEdit->GetValues();
        editTableDescription = new SDL_Rect[editValues.size()];
        editTableValue = new SDL_Rect[editValues.size()];

        editDynamicTextBoxes = new Textbox*[256];

        for(int i = 0; i < 256; i++)
            editDynamicTextBoxes[i] = NULL;

        CreateEditTable(editValues);

        return true;
    }
    else
    {
        FinishEditDynamic();
        return false;
    }
}

bool HelperEngine::PrepareEditTrigger()
{
    triggerEdit = Trigger::GetTriggerByCoord(mouseTileX, mouseTileY, mapContainer);

    if(editTableDescription)
    {
        delete[] editTableDescription;
        editTableDescription = NULL;
    }

    if(editTableValue)
    {
        delete[] editTableValue;
        editTableValue = NULL;
    }

    if(triggerEdit)
    {
        editValues = triggerEdit->GetValues();
        editTableDescription = new SDL_Rect[editValues.size()];
        editTableValue = new SDL_Rect[editValues.size()];

        editDynamicTextBoxes = new Textbox*[256];

        for(int i = 0; i < 256; i++)
            editDynamicTextBoxes[i] = NULL;

        CreateEditTable(editValues);

        return true;
    }
    else
    {
        FinishEditTrigger();
        return false;
    }
}

bool HelperEngine::PrepareEditArea()
{
    return false;
}

void HelperEngine::CreateEditTable(std::map<uint16_t, std::string> map)
{
    int i = 0;
    for(std::map<uint16_t, std::string>::iterator it = editValues.begin(); it != editValues.end(); ++it, ++i)
    {
        editTableDescription[i].h = FONT_SIZE;
        editTableDescription[i].w = FONT_SIZE * 14;
        editTableDescription[i].x = 0;
        editTableDescription[i].y = i * FONT_SIZE;

        editTableValue[i].h = FONT_SIZE;
        editTableValue[i].w = FONT_SIZE * 16;
        editTableValue[i].x = editTableDescription[i].w;
        editTableValue[i].y = i * FONT_SIZE;

        editDynamicTextBoxes[it->first] = new Textbox();
        editDynamicTextBoxes[it->first]->SetText(it->second);
    }
}

// edit menu for dynamic
void HelperEngine::DrawEditTable()
{

    std::map<uint16_t, std::string>::iterator it = editValues.begin();

    SDL_Rect rectText;
    int i = 0;
    for(std::map<uint16_t, std::string>::iterator it = editValues.begin(); it != editValues.end(); ++it, ++i)
    {
        SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(render, &editTableDescription[i]);

        SDL_SetRenderDrawColor(render, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(render, &editTableDescription[i]);

        SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);
        rectText = editTableDescription[i];

        std::string description = "";
        
        if(dynamicEdit)
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
        
        if(triggerEdit)
            switch(it->first)
        {
            case TRIGGER_MAP_TITLE:
                description = "title";
                break;
                
            case TRIGGER_MAP_X:
                description = "x";
                break; 

            case TRIGGER_MAP_Y:
                description = "y";
                break;

            case TRIGGER_MAP_TYPE:
                description = "type";
                break;

            case TRIGGER_MAP_NEW_MAP:
                description = "new map";
                break;

            case TRIGGER_MAP_NEW_X:
                description = "new x";
                break;

            case TRIGGER_MAP_NEW_Y:
                description = "new y";
                break;

            case TRIGGER_MAP_SWITCH_MAP:
                description = "switch map";
                break;

            case TRIGGER_MAP_CHANGE_STATE:
                description = "change state";
                break;

            case TRIGGER_MAP_TELEPORT:
                description = "teleport";
                break;

            case TRIGGER_MAP_ON_CONTACT:
                description = "on contact";
                break;

            case TRIGGER_MAP_ON_USE:
                description = "on use";
                break;

            case TRIGGER_MAP_SWITCH:
                description = "switch";
                break;

            case TRIGGER_MAP_STATE_1:
                description = "state 1";
                break;

            case TRIGGER_MAP_STATE_2:
                description = "state 2";
                break;

            case TRIGGER_MAP_TARGET_DYNAMIC:
                description = "target";
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
        
        SDL_RenderFillRect(render, &editTableValue[i]);

        SDL_SetRenderDrawColor(render, 0, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(render, &editTableValue[i]);

        SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);

        rectText = editTableValue[i];
        DrawText(font, value, &rectText, &fontColour);
        
    }
    SDL_SetRenderDrawColor(render, 0, 0, 0, SDL_ALPHA_OPAQUE);
}

void HelperEngine::FinishEditDynamic()
{
    editDynamicInput = -1;
    
    for(std::map<uint16_t, std::string>::iterator it = editValues.begin(); it != editValues.end(); ++it)
        it->second = editDynamicTextBoxes[it->first]->GetText();

    if(dynamicEdit)
        dynamicEdit->SetValues(editValues);

    editValues.clear();
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

void HelperEngine::FinishEditTrigger()
{
    editDynamicInput = -1;
    
    for(std::map<uint16_t, std::string>::iterator it = editValues.begin(); it != editValues.end(); ++it)
        it->second = editDynamicTextBoxes[it->first]->GetText();

    if(triggerEdit)
        triggerEdit->SetValues(editValues);

    editValues.clear();
    triggerEdit = NULL;

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

void HelperEngine::FinishEditArea()
{

}