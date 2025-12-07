#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(float startX, float startY);

    // ZMIANA: update przyjmuje pozycjê celu (gracza)
    void update(float dt, sf::Vector2f targetPos);

    // update z samej klasy Entity musimy "ukryæ" lub zignorowaæ, 
    // bo tutaj potrzebujemy targetPos. 
    // W C++ override wymaga identycznej sygnatury, wiêc zrobimy to tak:
    void update(float dt) override {}; // Pusta implementacja, nie u¿ywamy jej
};