#pragma once

namespace Config {
    const float LOGICAL_WIDTH = 1280.f;
    const float LOGICAL_HEIGHT = 720.f;

    const float PLAYER_RADIUS = 25.f;
    const float PLAYER_SPEED = 3.f;
    const unsigned int FRAME_LIMIT = 144;

    // NOWE: Konfiguracja wrogów
    const float ENEMY_RADIUS = 20.f;
    const float ENEMY_SPEED = 50.f;     // Wolniejsi od gracza
    const float SPAWN_INTERVAL = 1.0f;   // Co ile sekund nowy wróg
}