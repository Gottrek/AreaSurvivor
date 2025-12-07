#include "Player.h"
#include <algorithm>

Player::Player() {
    // Pobieramy promieñ z Constants.h
    shape.setRadius(Config::PLAYER_RADIUS);

    // ZMIANA: Kolor fioletowy
    // Mo¿esz u¿yæ gotowego: sf::Color::Magenta 
    // Lub w³asnego RGB (R, G, B): sf::Color(128, 0, 128)
    shape.setFillColor(sf::Color(150, 50, 250));

    // Ustawienie punktu zaczepienia na œrodku (wa¿ne dla kolizji!)
    shape.setOrigin(Config::PLAYER_RADIUS, Config::PLAYER_RADIUS);

    setPosition(Config::LOGICAL_WIDTH / 2.f, Config::LOGICAL_HEIGHT / 2.f);
    this->speed = Config::PLAYER_SPEED;
}

// ... reszta metod (handleInput, update) bez zmian ...

void Player::handleInput(bool blockInput) {
    // Jeœli GUI zjada input (np. piszemy w konsoli), nie ruszamy siê
    // (Opcjonalnie: WASD zazwyczaj dzia³a zawsze, ale strzelanie blokujemy)

    sf::Vector2f movement(0.f, 0.f);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

    // Normalizacja (Twoja logika z main.cpp)
    if (movement.x != 0.f || movement.y != 0.f) {
        float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
        movement /= length;

        // Przesuwamy pozycjê (metoda z Entity)
        move(movement * speed * /*dt przekazane w update*/ 1.0f);
        // UWAGA: W architekturze lepiej trzymaæ wektor ruchu w polu klasy
        // i aplikowaæ go w update(dt). Tutaj uproszczenie.
        this->velocity = movement * speed; // Zak³adaj¹c pole velocity w Entity
    }
    else {
        this->velocity = { 0,0 };
    }
}

void Player::update(float dt) {
    // 1. Ruch
    move(velocity * dt);

    // 2. Kolizja ze œcianami (Logic Bounds)
    sf::Vector2f pos = getPosition();
    float r = Config::PLAYER_RADIUS; // U¿ywamy promienia ze sta³ych

    // Lewa krawêdŸ
    if (pos.x - r < 0.f) pos.x = r;
    // Prawa krawêdŸ
    else if (pos.x + r > Config::LOGICAL_WIDTH) pos.x = Config::LOGICAL_WIDTH - r;

    // Górna krawêdŸ
    if (pos.y - r < 0.f) pos.y = r;
    // Dolna krawêdŸ
    else if (pos.y + r > Config::LOGICAL_HEIGHT) pos.y = Config::LOGICAL_HEIGHT - r;

    setPosition(pos.x, pos.y);
}