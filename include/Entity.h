#pragma once
#include <SFML/Graphics.hpp>

class Entity {
public:
    virtual ~Entity() = default;

    // Metoda czysto wirtualna - ka¿dy obiekt musi j¹ zaimplementowaæ (np. ruch)
    virtual void update(float dt) = 0;

    // Rysowanie obiektu (domyœlna implementacja)
    virtual void draw(sf::RenderWindow& window);

    // Metody pomocnicze (Wrapper na SFML)
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
    void move(const sf::Vector2f& offset);
    sf::FloatRect getGlobalBounds() const;

protected:
    // U¿ywamy CircleShape jako "placeholder" dla grafiki. 
    // W przysz³oœci zmienisz to na sf::Sprite z tekstur¹.
    sf::CircleShape shape;

    // Wektor prêdkoœci (przydatny dla fizyki)
    sf::Vector2f velocity;
};