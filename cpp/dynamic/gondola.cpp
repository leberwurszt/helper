// gondola.cpp

#include "gondola.h"

Gondola::Gondola(std::string title, MapContainer* mapContainer, SoundCommand* soundCommand):Dynamic(title, mapContainer, soundCommand)
{
    health =            GONDOLA_HEALTH;
    maxHealth =         GONDOLA_MAX_HEALTH;
    noclip =            GONDOLA_NOCLIP;
    flying =            GONDOLA_FLYING;
    state =             GONDOLA_STATE;
    type =              GONDOLA_TYPE;
    alive =             GONDOLA_ALIVE;
    obstacle =          GONDOLA_OBSTACLE;
    destructable =      GONDOLA_DESTRUCTABLE;
    direction =         GONDOLA_DIRECTION;
    ticksMove =         GONDOLA_TICKS_MOVE;
    ticksAttack =       GONDOLA_TICKS_ATTACK;
    damageAttack =      GONDOLA_DAMAGE;
}

void Gondola::Do()
{
    sprite = 1;
    double playerDist = std::sqrt(std::pow((double)mapContainer->player->GetX() - x, 2) + std::pow((double)mapContainer->player->GetY() - y, 2));
    
    if(playerDist <= range)
    {
        if(!found)
        {
            found = true;
            messageCommand->PushMessage("Gondola found!");
        }
        if(state == 1)
        {
            state = 2;
            soundCommand->PlayMusic(music);
        }
    }
    else if(state == 2)
    {
        soundCommand->PlayMusic(0);
        state = 1;
    }
}

void Gondola::SetMusic(uint8_t music)
{
    this->music = music;
}

uint8_t Gondola::GetMusic()
{
    return music;
}

std::vector<std::string> Gondola::CreateConfig()
{
    std::vector<std::string> vector = Dynamic::CreateConfig();
    vector.push_back("music=" + std::to_string(music));

    return vector;
}

std::map<uint16_t, std::string> Gondola::GetValues()
{
    std::map<uint16_t, std::string> values = Dynamic::GetValues();

    values.insert(std::pair<uint16_t, std::string>(GONDOLA_MAP_MUSIC, std::to_string(music)));

    return values;
}

void Gondola::SetValues(std::map<uint16_t, std::string> values)
{
    Dynamic::SetValues(values);
    
    for(std::map<uint16_t, std::string>::iterator it = values.begin(); it != values.end(); it++)
    {
        switch(it->first)
        {
            case GONDOLA_MAP_MUSIC:
                music = std::stoi(it->second);
                break;

            default:
                break;
        }
    }
}


void Gondola::PrintValues()
{
    Dynamic::PrintValues();
    
    std::cout <<
        "Music: " << (int) music << std::endl;
}