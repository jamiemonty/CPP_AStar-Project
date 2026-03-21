#include "GridRenderer.h"
#include <optional>

GridRenderer::GridRenderer(int rows, int cols, int size) : cellSize(size)
{
    // Initialize the SFML window based on the grid dimensions
    window.create(sf::VideoMode({ (unsigned int)(cols * cellSize), (unsigned int)(rows * cellSize) }), "A* Pathfinding Visualization");
}

void GridRenderer::render(const AStar& astar, const PathResults& results)
{
    while (window.isOpen())
    {
        // 1. Process Window Events
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // 2. Clear Screen
        window.clear(sf::Color::White);

        // 3. Draw Things
        drawGrid(astar);

        if (results.pathFound && results.path.size() > 1)
        {
            drawPath(results);
        }

        // 4. Display to Screen
        window.display();
    }
}

void GridRenderer::drawGrid(const AStar& astar)
{
    const auto& grid = astar.getGrid();
    int rows = astar.getRows();
    int cols = astar.getCols();

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            sf::RectangleShape cell(sf::Vector2f(cellSize - 1.f, cellSize - 1.f));
            cell.setPosition(sf::Vector2f(c * cellSize, r * cellSize)); // x is cols(c), y is rows(r)

            char cellValue = grid[r][c];

            if (cellValue == '#') {
                cell.setFillColor(sf::Color::Black);
            }
            else if (cellValue == 'S') {
                cell.setFillColor(sf::Color::Green);
            }
            else if (cellValue == 'G') {
                cell.setFillColor(sf::Color::Red);
            }
            else {
                cell.setFillColor(sf::Color(220, 220, 220)); // Light grey for empty
            }

            window.draw(cell);
        }
    }
}

void GridRenderer::drawPath(const PathResults& results)
{
    // Skip the first and last elements since they are already colored 'S' and 'G'
    for (size_t i = 1; i < results.path.size() - 1; ++i)
    {
        sf::RectangleShape pathCell(sf::Vector2f(cellSize - 1.f, cellSize - 1.f));

        // Results path holds Position(x, y). Following Cartesian grid logic where x is row, y is col
        pathCell.setPosition(sf::Vector2f(results.path[i].y * cellSize, results.path[i].x * cellSize));

        pathCell.setFillColor(sf::Color::Blue);
        window.draw(pathCell);
    }
}