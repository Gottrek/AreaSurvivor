#include "Game.h"
#include <cmath>
#include <cstdlib> // do rand()
#include <ctime>   // do time()

Game::Game() {
    // Inicjalizacja generatora liczb losowych
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    window.create(sf::VideoMode(Config::LOGICAL_WIDTH, Config::LOGICAL_HEIGHT),
        "Arena Survivor", sf::Style::Default);
    window.setFramerateLimit(Config::FRAME_LIMIT);

    gameView.setSize(Config::LOGICAL_WIDTH, Config::LOGICAL_HEIGHT);
    gameView.setCenter(Config::LOGICAL_WIDTH / 2.f, Config::LOGICAL_HEIGHT / 2.f);

    guiManager.init(window);

    restartGame(); // Ustawia zmienne pocz¹tkowe
}

Game::~Game() {
    guiManager.shutdown();
}

void Game::restartGame() {
    player = std::make_unique<Player>();
    enemies.clear();
    spawnTimer = 0.f;
    gameTime = 0.f;
    isGameOver = false;
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();
        if (dt > 0.1f) dt = 0.1f;
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        guiManager.processEvent(window, event);

        if (event.type == sf::Event::Closed)
            window.close();

        // Zmiana rozmiaru okna
        else if (event.type == sf::Event::Resized) {
            resizeView(window, gameView);
        }
    }
}

// Algorytm Letterboxing (Czarne pasy) - zachowuje proporcje gry
void Game::resizeView(const sf::RenderWindow& window, sf::View& view) {
    float windowRatio = window.getSize().x / (float)window.getSize().y;
    float viewRatio = Config::LOGICAL_WIDTH / Config::LOGICAL_HEIGHT;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // Jeœli okno jest za szerokie -> czarne pasy po bokach
    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }
    // Jeœli okno jest za wysokie -> czarne pasy góra/dó³
    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

void Game::update(float dt) {
    ImGui::SFML::Update(window, sf::seconds(dt));
    guiManager.update(dt);

    // Jeœli gra siê skoñczy³a, nie aktualizujemy logiki œwiata
    if (isGameOver) {
        // Tutaj ewentualnie logika menu koñca gry
        return;
    }

    // 1. Liczniki czasu
    gameTime += dt;
    spawnTimer += dt;

    // 2. Spawnowanie wrogów
    if (spawnTimer >= Config::SPAWN_INTERVAL) {
        spawnEnemy();
        spawnTimer = 0.f;
    }

    // 3. Logika Gracza
    player->handleInput(guiManager.wantsCaptureInput());
    player->update(dt);

    // 4. Logika Wrogów
    for (auto& enemy : enemies) {
        enemy->update(dt, player->getPosition());
    }

    // 5. Kolizje
    checkCollisions();
}

void Game::spawnEnemy() {
    float x = 0, y = 0;
    // Losujemy krawêdŸ: 0-góra, 1-prawo, 2-dó³, 3-lewo
    int side = std::rand() % 4;
    float margin = 50.f; // Spawnowanie nieco poza ekranem

    if (side == 0) { // Góra
        x = static_cast<float>(std::rand() % (int)Config::LOGICAL_WIDTH);
        y = -margin;
    }
    else if (side == 1) { // Prawo
        x = Config::LOGICAL_WIDTH + margin;
        y = static_cast<float>(std::rand() % (int)Config::LOGICAL_HEIGHT);
    }
    else if (side == 2) { // Dó³
        x = static_cast<float>(std::rand() % (int)Config::LOGICAL_WIDTH);
        y = Config::LOGICAL_HEIGHT + margin;
    }
    else { // Lewo
        x = -margin;
        y = static_cast<float>(std::rand() % (int)Config::LOGICAL_HEIGHT);
    }

    enemies.push_back(std::make_unique<Enemy>(x, y));
}

void Game::checkCollisions() {
    sf::Vector2f playerPos = player->getPosition();
    float pRadius = Config::PLAYER_RADIUS;
    float eRadius = Config::ENEMY_RADIUS;

    for (const auto& enemy : enemies) {
        sf::Vector2f enemyPos = enemy->getPosition();

        // Obliczamy dystans (dx*dx + dy*dy)
        float dx = playerPos.x - enemyPos.x;
        float dy = playerPos.y - enemyPos.y;
        float distSq = dx * dx + dy * dy;

        // Suma promieni do kwadratu (¿eby unikn¹æ powolnego pierwiastkowania w ka¿dej klatce)
        float minDst = pRadius + eRadius;

        if (distSq < minDst * minDst) {
            isGameOver = true;
            // Opcjonalnie: DŸwiêk œmierci
            break; // Wystarczy dotkn¹æ jednego
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);
    window.setView(gameView);

    // T³o
    sf::RectangleShape arenaBackground(sf::Vector2f(Config::LOGICAL_WIDTH, Config::LOGICAL_HEIGHT));
    arenaBackground.setFillColor(sf::Color(35, 35, 40));
    window.draw(arenaBackground);

    // Wrogowie
    for (const auto& enemy : enemies) {
        enemy->draw(window);
    }

    // Gracz (rysowany tylko jeœli ¿yje, lub martwy w innym kolorze)
    if (!isGameOver) {
        player->draw(window);
    }

    // UI
    float fps = 1.0f / clock.getElapsedTime().asSeconds();
    guiManager.renderDebugPanel(fps, player->getPosition());

    // --- GAME OVER SCREEN (ImGui) ---
    if (isGameOver) {
        // Wyœrodkowanie okna
        ImGui::SetNextWindowPos(ImVec2(window.getSize().x / 2.f, window.getSize().y / 2.f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("NIE ZYJESZ!");
        ImGui::Text("Przetrwales: %.2f sekund", gameTime);

        if (ImGui::Button("Zagraj Ponownie", ImVec2(200, 50))) {
            restartGame();
        }

        ImGui::End();
    }
    else {
        // Licznik czasu w trakcie gry (np. w rogu)
        ImGui::SetNextWindowPos(ImVec2(20, 20));
        ImGui::Begin("Stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoBackground);
        ImGui::Text("Czas: %.1f s", gameTime);
        ImGui::Text("Wrogowie: %zu", enemies.size());
        ImGui::End();
    }

    guiManager.render(window);
    window.display();
}