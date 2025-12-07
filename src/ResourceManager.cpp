#include "ResourceManager.h"
#include <iostream>

ResourceManager& ResourceManager::get() {
    static ResourceManager instance;
    return instance;
}

void ResourceManager::loadTexture(const std::string& name, const std::string& path) {
    sf::Texture tex;
    if (tex.loadFromFile(path)) {
        textures[name] = tex;
    }
    else {
        // Fallback: jeœli nie uda siê za³adowaæ, tworzymy pust¹ teksturê
        std::cerr << "Blad ladowania tekstury: " << path << std::endl;
    }
}

sf::Texture& ResourceManager::getTexture(const std::string& name) {
    return textures[name];
}