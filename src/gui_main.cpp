
#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    // Tworzymy okno o rozmiarze 900x600 pikseli
    sf::RenderWindow window(
        sf::VideoMode({900, 600}),
        "Graph Visualizer"
    );

    window.setFramerateLimit(60);

    // Tworzymy pierwszy wierzcholek grafu
    sf::CircleShape vertex(40.f);

    vertex.setFillColor(sf::Color::Green);
    vertex.setOrigin({40.f, 40.f});
    vertex.setPosition({450.f, 300.f});

    // Glowna petla programu
    while (window.isOpen())
    {
        // Obsluga zdarzen, np. zamkniecia okna
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // Czyscimy ekran
        window.clear(sf::Color(30, 30, 30));

        // Rysujemy wierzcholek
        window.draw(vertex);

        // Wyswietlamy narysowana klatke
        window.display();
    }

    return 0;
}