#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector> // NOWE
#include "Constants.h"
#include "GUIManager.h"
#include "Player.h"
#include "Enemy.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void resizeView(const sf::RenderWindow& window, sf::View& view);

    // NOWE: Metody logiki gry
    void spawnEnemy();
    void checkCollisions();
    void restartGame();

    sf::RenderWindow window;
    sf::View gameView;
    GUIManager guiManager;
    std::unique_ptr<Player> player;
    sf::Clock clock;

    // NOWE: Zmienne stanu gry
    std::vector<std::unique_ptr<Enemy>> enemies; // Kontener na wrogów
    float spawnTimer;    // Odlicza do kolejnego spawnu
    float gameTime;      // Ca³kowity czas gry
    bool isGameOver;     // Czy gra siê skoñczy³a
};