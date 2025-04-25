#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <filesystem>

using namespace sf;
using namespace std;

int main()
{
    // 1) Create window
    VideoMode vm(800, 600);
    RenderWindow window(vm, "Chaos Game: Sierpinski Triangle");

    // 2) Load a font and set up instruction text
    Font font;
    if (!font.loadFromFile("x64/Debug/arial.ttf")) {
        cerr << "Error loading font 'arial.ttf'" << endl;
        return -1;
    }
    Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(20);
    instructions.setFillColor(Color::Yellow);
    instructions.setPosition(10.f, 10.f);
    instructions.setString(
        "1) Click THREE times to set triangle vertices.\n"
        "2) Click a FOURTH time to set starting point.\n"
        "3) Watch the Sierpinski Triangle emerge!\n"
        "Press ESC to quit."
    );

    // 3) Containers for the vertices and generated points
    vector<Vector2f> vertices;
    vector<Vector2f> points;

    // 4) Random number generator for picking a vertex
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distVertex(0, 2);

    // 5) Main loop
    while (window.isOpen())
    {
        // --- Handle events ---
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
            {
                window.close();
            }
            else if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left)
            {
                Vector2f clickPos((float)event.mouseButton.x,
                    (float)event.mouseButton.y);

                if (vertices.size() < 3)
                {
                    // First three clicks: collect triangle vertices
                    vertices.push_back(clickPos);
                    cout << "Vertex " << vertices.size()
                        << ": (" << clickPos.x
                        << ", " << clickPos.y << ")\n";
                }
                else if (points.empty())
                {
                    // Fourth click: starting point for chaos game
                    points.push_back(clickPos);
                    cout << "Starting point: ("
                        << clickPos.x << ", "
                        << clickPos.y << ")\n";
                }
                // any clicks after that do nothing
            }
        }

        // --- Update (generate new points) ---
        if (!points.empty())
        {
            // Generate, say, 200 new points per frame for speed
            for (int i = 0; i < 200; ++i)
            {
                int idx = distVertex(gen);     // pick random vertex 0,1 or 2
                Vector2f& V = vertices[idx];   // reference to chosen vertex
                Vector2f& P = points.back();   // last generated point

                // midpoint formula
                Vector2f next;
                next.x = (V.x + P.x) * 0.5f;
                next.y = (V.y + P.y) * 0.5f;

                points.push_back(next);
            }
        }

        // --- Draw everything ---
        window.clear(Color::Black);

        // draw instructions in the top-left
        window.draw(instructions);

        // draw the three vertices as blue squares
        for (auto& v : vertices)
        {
            RectangleShape sq(Vector2f(8.f, 8.f));
            sq.setPosition(v.x - 4.f, v.y - 4.f); // center on v
            sq.setFillColor(Color::Blue);
            window.draw(sq);
        }

        // draw all chaos‐game points as small white pixels
        for (auto& p : points)
        {
            // you can also use CircleShape(1.f) if you prefer
            RectangleShape dot(Vector2f(2.f, 2.f));
            dot.setPosition(p.x, p.y);
            dot.setFillColor(Color::White);
            window.draw(dot);
        }

        window.display();
    }

    return 0;
}