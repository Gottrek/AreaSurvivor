#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imguiThemes.h"

// Sta³e konfiguracyjne
const float WINDOW_WIDTH = 1000.f;
const float WINDOW_HEIGHT = 500.f;
const float PLAYER_RADIUS = 50.f;
const float PLAYER_SPEED = 400.f;

int main()
{
    // --- 1. Inicjalizacja Okna ---
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Arena Survivor");
    window.setFramerateLimit(144);

    // --- 2. Inicjalizacja ImGui ---
    ImGui::SFML::Init(window);
    imguiThemes::embraceTheDarkness();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.FontGlobalScale = 2.f; // Zmniejszy³em skalê czcionki, bo 3.0 to bardzo du¿o

    // Stylizacja ImGui
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg].w = 0.9f; // Trochê bardziej nieprzezroczyste t³o

    // --- 3. Inicjalizacja Gracza ---
    sf::CircleShape player(PLAYER_RADIUS);
    player.setFillColor(sf::Color::Green);
    player.setOrigin(PLAYER_RADIUS, PLAYER_RADIUS); // Punkt zaczepienia w œrodku ko³a
    player.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT / 2.f);

    sf::Clock clock;

    // --- 4. Pêtla G³ówna Gry ---
    while (window.isOpen())
    {
        // A. Obs³uga Zdarzeñ
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
        }

        // B. Obliczanie DeltaTime (czasu klatki)
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();
        // Zabezpieczenie przed "skokiem" czasu przy zawieszeniu okna
        if (dt > 0.1f) dt = 0.1f;

        // C. Aktualizacja ImGui
        ImGui::SFML::Update(window, deltaTime);

        // Konfiguracja DockSpace (t³o dla okienek ImGui)
        ImGui::PushStyleColor(ImGuiCol_WindowBg, sf::Color(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, sf::Color(0, 0, 0, 0));
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        ImGui::PopStyleColor(2);

        // Przyk³adowe okno ImGui
        ImGui::Begin("Debug Panel");
        ImGui::Text("FPS: %.1f", 1.0f / dt);
        ImGui::Text("Position: (%.1f, %.1f)", player.getPosition().x, player.getPosition().y);
        ImGui::End();

        // D. Logika Gry (Ruch Gracza)
        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) movement.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) movement.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) movement.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) movement.x += 1.f;

        // Normalizacja wektora (¿eby ruch po skosie nie by³ szybszy)
        if (movement.x != 0.f || movement.y != 0.f)
        {
            // Prosta normalizacja
            float length = std::sqrt(movement.x * movement.x + movement.y * movement.y);
            movement /= length;

            player.move(movement * PLAYER_SPEED * dt);
        }

        // E. Renderowanie
        window.clear(sf::Color(20, 20, 20)); // Ciemnoszare t³o zamiast czarnego
        window.draw(player);
        ImGui::SFML::Render(window);
        window.display();
    }

    // --- 5. Czyszczenie ---
    ImGui::SFML::Shutdown();

    return 0;
}