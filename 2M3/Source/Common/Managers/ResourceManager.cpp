#include "Common/Managers/ResourceManager.h"
#include <cstdlib>

ResourceManager* ResourceManager::instance = nullptr;

ResourceManager::ResourceManager()
{
    std::atexit(&cleanUp);
}

void ResourceManager::cleanUp()
{
    if (instance != nullptr)
        free(instance);
    instance = nullptr;
}