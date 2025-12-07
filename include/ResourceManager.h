#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class ResourceManager {
public:
    // Dostêp do Singletona
    static ResourceManager& get();

    // Metody
    void loadTexture(const std::string& name, const std::string& path);
    sf::Texture& getTexture(const std::string& name);

private:
    ResourceManager() = default; // Prywatny konstruktor
    std::map<std::string, sf::Texture> textures;
};