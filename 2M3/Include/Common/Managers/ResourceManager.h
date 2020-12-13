#pragma once
#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"

class ResourceManager
{
public:
    /**
     * Singleton management
     */
    static ResourceManager* getInstance()
    {
        if (instance == 0)
        {
            instance = new ResourceManager();
        }

        return instance;
    }

    /**
     * Methods
     */
    TextureHolder* textures() { return textureHolder; }
    void setTextures(TextureHolder* textures) { textureHolder = textures; }

private:
    /**
     * Singleton management
     */
    ResourceManager();
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&);
    ResourceManager& operator=(const ResourceManager&);

    static void cleanUp();

    static ResourceManager* instance;

    /**
     * Attributes
     */
    TextureHolder* 		textureHolder = nullptr;
};