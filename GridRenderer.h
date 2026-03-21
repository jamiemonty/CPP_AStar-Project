#pragma once
#ifndef GRID_RENDERER_H
#define GRID_RENDERER_H

#include <SFML/Graphics.hpp>
#include "AStar.h"

class GridRenderer
{
private:
    int cellSize;
    sf::RenderWindow window;

    // Helper functions for drawing
    void drawGrid(const AStar& astar);
    void drawPath(const PathResults& results);

public:
    // Constructor initializes the window based on grid size
    GridRenderer(int rows, int cols, int cellSize = 40);

    // Main loop that keeps the window open and renders
    void render(const AStar& astar, const PathResults& results);
};

#endif