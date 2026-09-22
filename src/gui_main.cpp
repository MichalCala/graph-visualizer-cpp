
#include <SFML/Graphics.hpp>

#include <array>
#include <optional>
#include <string>
#include <iostream>

#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <cstddef>
#include <vector>

#include "Graph.hpp"

int main()
{
    // 1. Tworzymy graf nieskierowany
    Graph graph(10, false);

    // Krawedzie z Twojego zadania
    graph.AddEdge(0, 1);
    graph.AddEdge(1, 2);
    graph.AddEdge(2, 3);
    graph.AddEdge(3, 4);
    graph.AddEdge(3, 7);
    graph.AddEdge(4, 5);
    graph.AddEdge(5, 9);
    graph.AddEdge(9, 9);
    graph.AddEdge(6, 8);
    graph.AddEdge(8, 6);
    graph.AddEdge(0, 8);

    // 2. Pozycje wierzcholkow
    
    std::vector<sf::Vector2f> positions = {
        {110.f, 260.f},  // 0
        {245.f, 160.f},  // 1
        {370.f, 160.f},  // 2
        {495.f, 160.f},  // 3
        {610.f, 245.f},  // 4
        {720.f, 350.f},  // 5
        {340.f, 490.f},  // 6
        {530.f, 55.f},   // 7
        {220.f, 410.f},  // 8
        {790.f, 460.f}   // 9
    };

    // 3. Stworzenie okna
    sf::RenderWindow window(
        sf::VideoMode({900, 600}),
        "Graph Visualizer"
    );

    window.setFramerateLimit(60);
    // Czcionka do wyswietlania numerow wierzcholkow
    sf::Font font;

    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf"))
    {
        std::cerr << "Nie udalo sie zaladowac czcionki!\n";
        return 1;
    }

    // Wierzcholek, od ktorego rozpoczniemy DFS
    int startVertex = 0;
    // Pierwszy wierzcholek tworzonej krawedzi
    // -1 oznacza, ze zaden nie zostal wybrany
    int edgeStartVertex = -1;

    // Kolejnosc odwiedzania wierzcholkow
    std::vector<int> dfsOrder;

    // Informacja, ktore wierzcholki zostaly odwiedzone
    std::vector<bool> visited(
        graph.NumberOfVertices(), false
    );

    // Numer kolejnego kroku animacji
    std::size_t nextStep = 0;

    // Czy animacja jest uruchomiona?
    bool animating = false;

    // Zegar odmierzajacy czas miedzy krokami
    sf::Clock stepClock;

    while (window.isOpen())
    {
        // Obsluga zamykania okna
        
        while (const std::optional event = window.pollEvent())
        {
            //Zamykanie okna
            if(event->is<sf::Event::Closed>())
            {
                window.close();
            }

            //Oblsuga klawiatury
            if(const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                //Spacja - rozpoczecie DFS
                if(key->code == sf::Keyboard::Key::Space)
                {
                    dfsOrder = graph.DFS(startVertex);

                    std::fill(
                        visited.begin(),
                        visited.end(),
                        false
                    );

                    nextStep = 0;
                    animating = !dfsOrder.empty();

                    stepClock.restart();
                }
                // R - reset animacji
                if (key->code == sf::Keyboard::Key::R)
                {
                    animating = false;
                    nextStep = 0;

                    dfsOrder.clear();

                    std::fill(
                        visited.begin(),
                        visited.end(),
                        false
                    );
                }
            }

            // Obsluga klikniecia mysza
            
            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
            {
                sf::Vector2f clickPosition{
                static_cast<float>(mouse->position.x),
                static_cast<float>(mouse->position.y)
                };

                // Sprawdzamy, czy kliknieto istniejacy wierzcholek
                int clickedVertex = -1;
                bool tooClose = false;

                for (int i = 0; i < graph.NumberOfVertices(); ++i)
                {
                    float dx = clickPosition.x - positions[i].x;
                    float dy = clickPosition.y - positions[i].y;

                    float distanceSquared = dx * dx + dy * dy;

                    if (distanceSquared <= 22.f * 22.f)
                    {
                        clickedVertex = i;
                        break;
                    }

                    if (distanceSquared < 50.f * 50.f)
                    {
                        tooClose = true;
                    }
                }

                // =========================================
                // LEWY PRZYCISK - wybor lub dodanie wierzcholka
                // =========================================

                if (mouse->button == sf::Mouse::Button::Left)
                {
                    edgeStartVertex = -1;

                    if (clickedVertex != -1)
                    {
                        // Wybrano wierzcholek poczatkowy DFS
                        startVertex = clickedVertex;
                    }
                    else if (!tooClose)
                    {
                        // Sprawdzamy, czy nowe kolo zmiesci sie w oknie
                        if (clickPosition.x >= 22.f &&
                        clickPosition.x <= 878.f &&
                        clickPosition.y >= 22.f &&
                        clickPosition.y <= 578.f)
                        {
                            int newVertex = graph.AddVertex();

                            positions.push_back(clickPosition);

                            startVertex = newVertex;
                        }
                    }

                    // Reset poprzedniej animacji
                    animating = false;
                    nextStep = 0;
                    dfsOrder.clear();

                    visited.assign(graph.NumberOfVertices(), false);
                }

                // =========================================
                // PRAWY PRZYCISK - tworzenie krawedzi
                // =========================================

                if (mouse->button == sf::Mouse::Button::Right)
                {
                    if (clickedVertex != -1)
                    {
                        // Pierwsze klikniecie
                        if (edgeStartVertex == -1)
                        {
                            edgeStartVertex = clickedVertex;
                        }
                        // Drugie klikniecie
                        else
                        {
                            graph.AddEdge(edgeStartVertex, clickedVertex);

                            // Konczymy tworzenie krawedzi
                            edgeStartVertex = -1;

                            // Resetujemy poprzednia animacje DFS
                            animating = false;
                            nextStep = 0;
                            dfsOrder.clear();

                            visited.assign(graph.NumberOfVertices(), false);
                        }
                    }
                    else
                    {
                        // Klikniecie pustego miejsca anuluje tworzenie
                        edgeStartVertex = -1;
                    }
                }
            }

        }

        window.clear(sf::Color(30, 30, 30));
        
        // Wykonujemy jeden krok DFS co pol sekundy
        if (animating && stepClock.getElapsedTime().asSeconds() >= 0.5f)
        {
            // Pobieramy numer kolejnego wierzcholka
            int vertex = dfsOrder[nextStep];

            // Oznaczamy go jako odwiedzony
            visited[vertex] = true;

            // Przechodzimy do nastepnego kroku
            ++nextStep;

            // Uruchamiamy zegar od nowa
            stepClock.restart();

            // Sprawdzamy, czy animacja sie zakonczyla
            if (nextStep >= dfsOrder.size())
            {
                animating = false;
            }
        }

        // 5. Rysowanie krawedzi
        sf::VertexArray edges(sf::PrimitiveType::Lines);

        for (int from = 0;
             from < graph.NumberOfVertices();
             ++from)
        {
            for (int to = from;
                 to < graph.NumberOfVertices();
                 ++to)
            {
                if (!graph.IsEdge(from, to))
                {
                    continue;
                }

                // Petla wlasna, np. 9 -> 9
                if (from == to)
                {
                    sf::CircleShape loop(14.f);

                    loop.setFillColor(sf::Color::Transparent);
                    loop.setOutlineColor(sf::Color(170, 170, 170));
                    loop.setOutlineThickness(2.f);

                    loop.setPosition(
                        positions[from] +
                        sf::Vector2f{10.f, -42.f}
                    );

                    window.draw(loop);

                    continue;
                }

                // Zwykla krawedz
                edges.append(
                    sf::Vertex{
                        positions[from],
                        sf::Color(170, 170, 170)
                    }
                );   

                edges.append(
                    sf::Vertex{
                        positions[to],
                        sf::Color(170, 170, 170)
                    }
                );
            }
        }

        window.draw(edges);

 
    // 6. Rysowanie wierzcholkow i ich numerow
    for (int i = 0; i < graph.NumberOfVertices(); ++i)
    {
        // Rysujemy niebieskie kolo
        sf::CircleShape vertex(22.f);

        vertex.setOrigin({22.f, 22.f});
        vertex.setPosition(positions[i]);        
        // Kolorowanie wierzcholkow
        if (i == edgeStartVertex)
        {
            // Pierwszy wierzcholek nowej krawedzi - fioletowy
            vertex.setFillColor(sf::Color(170, 90, 230));
        }
        else if (visited[i])
        {
            // Odwiedzony przez DFS - zielony
            vertex.setFillColor(sf::Color(50, 200, 100));
        }
        else if (i == startVertex)
        {
            // Wierzcholek startowy DFS - zolty
            vertex.setFillColor(sf::Color(255, 200, 50));
        }
        else
        {
            // Pozostale wierzcholki - niebieskie
            vertex.setFillColor(sf::Color(70, 150, 255));
        }

        window.draw(vertex);

        // Tworzymy napis z numerem wierzcholka
        sf::Text text(font, std::to_string(i), 20);

        text.setFillColor(sf::Color::White);

        // Wyznaczamy srodek napisu
        sf::FloatRect bounds = text.getLocalBounds();

        text.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        // Umieszczamy napis na srodku kola
        text.setPosition(positions[i]);

        window.draw(text);
    }

        // 7. Wyswietlenie gotowej klatki
        window.display();
    }

    return 0;
}