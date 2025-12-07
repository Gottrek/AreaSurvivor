#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h" // Zak³adam, ¿e stworzysz te¿ Entity.h wg diagramu
#include "Constants.h"
#include <cmath>

// Player dziedziczy po Entity (wg diagramu)
class Player : public Entity {
public:
    Player();
    void handleInput(bool blockInput); // blockInput = czy ImGui zajmuje myszkê
    void update(float dt) override;
    // draw() jest w klasie bazowej Entity, ale mo¿na nadpisaæ

private:
    float speed;
};