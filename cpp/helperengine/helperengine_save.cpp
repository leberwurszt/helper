// SAVE FUNCTIONS
#include "helperengine.h"

bool HelperEngine::SaveMap()
{
    bool success = true;

    std::filesystem::create_directory("maps/" + mapContainer->mapDir);
    success *= mapContainer->map->SetStart(mapContainer->player->GetX(), mapContainer->player->GetY());
    success *= mapContainer->map->Save(MAP_PATH + mapContainer->mapDir + "/map.hex");
    success *= SaveDynamics();
    success *= SaveTriggers();
    success *= SaveAreas();
    if (success)
        messageCommand->PushMessage("Map saved");
    else
        messageCommand->PushMessage("Error saving map");

    return success;
}

bool HelperEngine::SaveTriggers()
{
    return SaveTriggers(mapContainer->mapDir, mapContainer->triggerList);
}

bool HelperEngine::SaveAreas()
{
    return SaveAreas(mapContainer->mapDir, mapContainer->areaList);
}

bool HelperEngine::SaveDynamics()
{
    return SaveDynamics(mapContainer->mapDir, mapContainer->dynamicList);
}

bool HelperEngine::SaveDynamics(std::string mapName, std::list<Dynamic*> dynamicList)
{
    bool success = true;
    std::vector<std::string> dynamicVector;
    std::vector<std::string> configVector;

    for(Dynamic* dynamic : dynamicList)
    {
        if(dynamic->GetType() != 1) // not player
        {
            dynamicVector = dynamic->CreateConfig();

            for(std::string line : dynamicVector)
                configVector.push_back(line);

            configVector.push_back("");
        }
    }

    std::ofstream dynamicFile(MAP_PATH + mapName + "/dynamic.config");
    try
    {
        for(std::string line : configVector)
            dynamicFile << line << std::endl;
    }

    catch(const std::exception& e)
    {
        success = false;
        std::cerr << e.what() << std::endl;
    }
    if(dynamicFile.is_open())
        dynamicFile.close();

    return success;
}

bool HelperEngine::SaveTriggers(std::string mapName, std::list<Trigger*> triggerList)
{
    bool success = true;
    std::vector<std::string> triggerVector;
    std::vector<std::string> configVector;

    for(Trigger* trigger : triggerList)
    {
        triggerVector = trigger->CreateConfig();
        for(std::string line : triggerVector)
            configVector.push_back(line);
            
        configVector.push_back("");
    }

    std::ofstream triggerFile(MAP_PATH + mapName + "/trigger.config");
    try
    {
        for(std::string line : configVector)
            triggerFile << line << std::endl;
    }

    catch(const std::exception& e)
    {
        success = false;
        std::cerr << e.what() << std::endl;
    }
    if(triggerFile.is_open())
        triggerFile.close();

    return success;
}

bool HelperEngine::SaveAreas(std::string mapName, std::list<Area*> areaList)
{
    bool success = true;
    std::vector<std::string> areaVector;
    std::vector<std::string> configVector;

    for(Area* area : areaList)
    {
        areaVector = area->CreateConfig();
        for(std::string line : areaVector)
            configVector.push_back(line);
        configVector.push_back("");
    }

    std::ofstream areaFile(MAP_PATH + mapName + "/area.config");
    try
    {
        for(std::string line : configVector)
            areaFile << line << std::endl;
    }

    catch(const std::exception& e)
    {
        success = false;
        std::cerr << e.what() << std::endl;
    }
    if(areaFile.is_open())
        areaFile.close();

    return success;
}
