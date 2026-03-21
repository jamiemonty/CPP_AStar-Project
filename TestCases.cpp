/*   ____.              .__            _____                 __
    |    |____    _____ |__| ____     /     \   ____   _____/  |_  ____   ____   _____   ___________ ___.__.
    |    \__  \  /     \|  |/ __ \   /  \ /  \ /  _ \ /    \   __\/ ___\ /  _ \ /     \_/ __ \_  __ <   |  |
/\__|    |/ __ \|  Y Y  \  \  ___/  /    Y    (  <_> )   |  \  | / /_/  >  <_> )  Y Y  \  ___/|  | \/\___  |
\________(____  /__|_|  /__|\___  > \____|__  /\____/|___|  /__| \___  / \____/|__|_|  /\___  >__|   / ____|
              \/      \/        \/          \/            \/    /_____/              \/     \/       \/
Jamie Montgomery
28/02/2026
*/
#include <iostream>
#include "AStar.h"
#include "GridRenderer.h"

void testBasicPathfinding() {

    //  Create object to automatically call the constructor which initialises the grid
    AStar grid(10, 10);

    //  Add vertical wall
    for (int r = 2; r < 8; ++r) {
        grid.setCell(r, 5, '#');
    }

    //  Add horizontal wall
    for (int c = 1; c < 5; ++c) {
        grid.setCell(3, c, '#');
    }

    //  Set start and goal
    grid.setCell(1, 1, 'S');
    grid.setCell(8, 3, 'G');
    Position start(1, 1);
    Position goal(8, 3);

    //  Print the grid
    grid.PrintGrid();

    PathResults results = grid.Solve(start, goal);

    if (results.pathFound) {
        std::cout << "Path Found!\n";
        std::cout << "Path Length: " << results.pathLength << "\n";
        std::cout << "Nodes Expanded: " << results.nodesExpanded << "\n";
        std::cout << "Execution Time: " << results.executionTime << " seconds\n";

        std::cout << "Path: \n";

        for (int i = 0; i < results.path.size(); ++i) {
            std::cout << "(" << results.path[i].x << ", " << results.path[i].y << ")";
            if (i < results.path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n\n";

        //  Mark the path on the grid for visualization
        for (const Position& pos : results.path) {
            if (!(pos == start) && !(pos == goal)) { //  Don't overwrite start and goal
                grid.setCell(pos.x, pos.y, '*'); //  Mark path with '*'
            }
        }
        std::cout << "Grid with Path:\n";
        grid.PrintGrid();

        std::cout << "\n TEST 1 PASSED\n";
    }
    else {
        std::cout << "No Path Found.\n";
        std::cout << " TEST 1 FAILED\n";
    }
}

void testNoPath() {
    std::cout << "\n========================================\n";
    std::cout << "  TEST 2: No Path Available\n";
    std::cout << "========================================\n\n";

    //  Create a new grid
    AStar grid(10, 10);

    //  Define start and goal
    Position start(1, 1);
    Position goal(5, 5);

    //  Place start
    grid.setCell(start.x, start.y, 'S');

    //  Surround goal with walls (make it unreachable)
    grid.setCell(goal.x, goal.y, 'G');
    grid.setCell(4, 5, '#');  // Left of goal
    grid.setCell(6, 5, '#');  // Right of goal
    grid.setCell(5, 4, '#');  // Above goal
    grid.setCell(5, 6, '#');  // Below goal

    std::cout << "Initial Grid (goal is surrounded by walls):\n";
    grid.PrintGrid();

    //  Run A* algorithm
    std::cout << "\n--- Running A* ---\n";
    PathResults results = grid.Solve(start, goal);

    if (!results.pathFound) {
        std::cout << "\n? TEST 2 PASSED: Correctly detected no path\n";
        std::cout << "  Nodes expanded: " << results.nodesExpanded << "\n";
        std::cout << "  Execution time: " << results.executionTime << " seconds\n";
    }
    else {
        std::cout << "\n? TEST 2 FAILED: Found path when none exists!\n";
        std::cout << "  This shouldn't happen!\n";
    }
}

void testStartEqualsGoal() {
    std::cout << "\n========================================\n";
    std::cout << "  TEST 3: Start Equals Goal\n";
    std::cout << "========================================\n\n";

    //  Create a new grid
    AStar grid(10, 10);

    //  Start and goal are the same position
    Position start(5, 5);
    Position goal(5, 5);

    //  Place on grid
    grid.setCell(start.x, start.y, 'S');

    std::cout << "Initial Grid (start = goal):\n";
    grid.PrintGrid();

    //  Run A* algorithm
    std::cout << "\n--- Running A* ---\n";
    PathResults results = grid.Solve(start, goal);

    if (results.pathFound && results.pathLength == 0) {
        std::cout << "\n? TEST 3 PASSED: Start equals goal handled correctly\n";
        std::cout << "  Path length: " << results.pathLength << " (should be 0)\n";
        std::cout << "  Nodes expanded: " << results.nodesExpanded << " (should be 0)\n";
        std::cout << "  Execution time: " << results.executionTime << " seconds\n";
    }
    else if (results.pathFound && results.pathLength != 0) {
        std::cout << "\n? TEST 3 PARTIAL: Path found but length is wrong\n";
        std::cout << "  Expected path length: 0\n";
        std::cout << "  Actual path length: " << results.pathLength << "\n";
    }
    else {
        std::cout << "\n? TEST 3 FAILED: Should find 'path' when start = goal\n";
    }
}

void testEdgeCases() {
    std::cout << "\n========================================\n";
    std::cout << "  TEST 4: Edge Cases\n";
    std::cout << "========================================\n\n";

    // ====================================
    // Sub-test A: Start on Wall
    // ====================================
    std::cout << "--- Sub-test A: Start on Wall ---\n";
    AStar grid1(10, 10);

    Position wallStart(5, 5);
    Position validGoal(8, 8);

    grid1.setCell(wallStart.x, wallStart.y, '#');  // Make start a wall
    grid1.setCell(validGoal.x, validGoal.y, 'G');

    PathResults results1 = grid1.Solve(wallStart, validGoal);

    if (!results1.pathFound) {
        std::cout << "? Sub-test A PASSED: Start on wall rejected\n";
    }
    else {
        std::cout << "? Sub-test A FAILED: Should reject start on wall\n";
    }

    // ====================================
    // Sub-test B: Goal on Wall
    // ====================================
    std::cout << "\n--- Sub-test B: Goal on Wall ---\n";
    AStar grid2(10, 10);

    Position validStart(1, 1);
    Position wallGoal(5, 5);

    grid2.setCell(validStart.x, validStart.y, 'S');
    grid2.setCell(wallGoal.x, wallGoal.y, '#');  // Make goal a wall

    PathResults results2 = grid2.Solve(validStart, wallGoal);

    if (!results2.pathFound) {
        std::cout << "? Sub-test B PASSED: Goal on wall rejected\n";
    }
    else {
        std::cout << "? Sub-test B FAILED: Should reject goal on wall\n";
    }

    // ====================================
    // Sub-test C: Adjacent Positions
    // ====================================
    std::cout << "\n--- Sub-test C: Adjacent Positions ---\n";
    AStar grid3(10, 10);

    Position start(5, 5);
    Position goal(6, 5);  // One step to the right

    grid3.setCell(start.x, start.y, 'S');
    grid3.setCell(goal.x, goal.y, 'G');

    PathResults results3 = grid3.Solve(start, goal);

    if (results3.pathFound && results3.pathLength == 1) {
        std::cout << "? Sub-test C PASSED: Adjacent positions (path length = " << results3.pathLength << ")\n";
    }
    else if (results3.pathFound) {
        std::cout << "? Sub-test C PARTIAL: Path found but length = " << results3.pathLength << " (expected 1)\n";
    }
    else {
        std::cout << "? Sub-test C FAILED: Should find path between adjacent positions\n";
    }

    std::cout << "\n? TEST 4 COMPLETE\n";
}

void testComplexMaze() {

    AStar grid(15, 15);

    //  Vertical walls
    for (int r = 2; r < 13; r++) {
        grid.setCell(r, 4, '#');
    }

    for (int r = 0; r < 10; r++) {
        grid.setCell(r, 10, '#');
    }

    //  Horizontal walls
    for (int c = 0; c < 8; c++) {
        grid.setCell(7, c, '#');
    }

    for (int c = 6; c < 15; c++) {
        grid.setCell(10, c, '#');
    }

    //  Set start in top-left and goal in bottom-right
    Position start(0, 0);
    Position goal(14, 14);

    grid.setCell(start.x, start.y, 'S');
    grid.setCell(goal.x, goal.y, 'G');

    grid.PrintGrid();

    //  Run A* algorithm
    PathResults results = grid.Solve(start, goal);

    if (results.pathFound) {
        std::cout << "  Path length: " << results.pathLength << " steps\n";
        std::cout << "  Nodes expanded: " << results.nodesExpanded << "\n";
        std::cout << "  Execution time: " << results.executionTime << " seconds\n";

        //  Mark the path
        for (const Position& pos : results.path) {
            if (!(pos == start) && !(pos == goal)) {
                grid.setCell(pos.x, pos.y, '*');
            }
        }

        std::cout << "\nMaze with Solution Path:\n";
        grid.PrintGrid();

        std::cout << "Path: ";
        for (size_t i = 0; i < results.path.size(); ++i) {
            if (i > 0 && i % 8 == 0) {
                std::cout << "\n      ";
            }
            std::cout << "(" << results.path[i].x << "," << results.path[i].y << ")";
            if (i < results.path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n";

        // VISUALIZE:
        GridRenderer renderer(grid.getRows(), grid.getCols());
        renderer.render(grid, results);

    }
    else {
        std::cout << "\nFAILED: Should find path through maze\n";
        std::cout << "  Nodes expanded: " << results.nodesExpanded << "\n";

        // You can pop open the window when it fails so you can see why
        GridRenderer renderer(grid.getRows(), grid.getCols());
        renderer.render(grid, results);
    }
}