#include "AssetManager.h"
#include "TextureManager.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager()
{
    if (instance == nullptr) 
    {
        instance = this;
    }
}

AssetManager::~AssetManager()
{
}

//Working with a MAP DOESNT WORK
void AssetManager::AddTexture(std::string id, const char* path)
{
    //Guard Clause
    for (int i = 0; i < textures.size(); i++)
    {
        if (textureIds[i] == id)
        {
            return;
        }
    }

    SDL_Texture* texture = TextureManager::LoadTexture(path);
    textures.emplace_back(texture);
    textureIds.emplace_back(id);
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
    for (int i = 0; i < textures.size(); i++) 
    {
        if (textureIds[i] == id) 
        {
            return textures[i];
        }
    }

    std::cout << "NO TEXTURE FOUND FOR " << id << std::endl;
    return nullptr;
}

void AssetManager::AddFont(std::string id, const char* path, int fontSize)
{
    //Guard Clause
    for (int i = 0; i < fonts.size(); i++)
    {
        if (fontIds[i] == id)
        {
            return;
        }
    }

    fonts.emplace_back(TTF_OpenFont(path, fontSize));
    fontIds.emplace_back(id);
}

TTF_Font* AssetManager::GetFont(std::string id)
{
    for (int i = 0; i < fonts.size(); i++)
    {
        if (fontIds[i] == id)
        {
            return fonts[i];
        }
    }

    std::cout << "NO FONT FOUND FOR " << id << std::endl;
    return nullptr;
}
