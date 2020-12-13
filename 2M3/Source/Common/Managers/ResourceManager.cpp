#include "Common/Managers/ResourceManager.h"
#include <cstdlib>

ResourceManager* ResourceManager::instance = 0;

ResourceManager::ResourceManager()
{
    std::atexit(&cleanUp);
}

void ResourceManager::cleanUp()
{
    if (instance != 0)
        delete instance;
    instance = 0;
}