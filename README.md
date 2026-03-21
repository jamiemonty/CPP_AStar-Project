# A Star Pathfinding Algorithm
## C++ Programming Project

**Author:** Jamie Montgomery  
**Student ID:** G00419525  
**Module:** C++ Programming  
**Lecturer:** Michelle Lynch  
**Academic Year:** 2025-2026

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Design & Implementation](#design--implementation)
3. [Code Examples](#code-examples)
4. [Testing & Validation](#testing--validation)
5. [Project Management](#project-management)
6. [Reflection](#reflection)
7. [References](#references)

---

## Project Overview

### Objectives
This project implements the A* pathfinding algorithm in modern C++ (C++11+) with the following features:
- Create a grid-based environment with obstacles
- Use the Manhattan distance heuristic to estimate remaining cost to the goal
- 4-way movement (up, down, left, right) for pathfinding
- Output the grid and the solved path using ASCII console visualisation
- Provide an SFML-based visualisation of the grid and final path
- Record performance statistics, including nodes expanded and execution time
- Validate using unit-style test cases, including general and edge scenarios

### Motivation
A* is widely used in robotics, game development, and navigation systems because it can find an optimal path while exploring far fewer nodes than Dijkstra's algorithm. This project demonstrates understanding of:
- Heuristic search algorithms using $$f(n) = g(n) + h(n)$$
- Object-oriented design in C++ by using methods and classes (Encapsulation) 
- STL container usage (vectors, maps, priority queues)
- Algorithm efficiency considerations (nodes expanded & execution time)

---

## Design & Implementation

### Architecture

#### UML Class Diagram (MVP)
This is where I began, I found a tool to visualise the minimal viable products UML Class Diagram (src="plantuml.com"), this is subject to change as I hope to add more features along the way. 
It focused on a minimal working grid system:

- A single AStar class stored the grid
- Supported setting cells (`setCell`), validating movement (`isPassable`), and printing the grid output (`PrintGrid`)
- This was to prove the grid representation worked and could display walls, start and goal correctly.

<img width="693" height="281" alt="image" src="https://github.com/user-attachments/assets/98d98a83-a078-4766-a187-883fa0f50ec6" />

#### Final Implementation
The final design expands the MVP into a complete pathfinding system and seperates concerns (abstraction): 

**New structures added:**
- **Position (struct):** represents `(x, y)` coordinates and supports `operator==` (goal check) and `operator<` (use as a key in `std::map`)
- **PathResults (struct):** returns the full results (pathFound, path, pathLength, nodesExpanded, executionTime)
- **Node (struct):** helper type stored in the priority queue so the open set is ordered by lowest `fScore`
- **GridRenderer (class):** SFML-based visualiser that reads the grid and draws the final path

**Key improvement from MVP --> Final Product:**
- The algorithm now runs through `Solve(start, goal)` and returns structured results instead of relying on print-only output.
- A* is modularised into helper methods (`heuristic`, `getNeighbors`, `reconstructPath`) which improves readability, testing, and explanation.
- Visualisation is separated from pathfinding logic (SFML is optional), improving reuse and maintainability.

<img width="1571" height="412" alt="image" src="https://github.com/user-attachments/assets/a81694b2-3ee8-47fc-b252-1b5a2956335f" />


### Class Structure
This project is organised into small and focused types. Each type has a clear responsibility, which improves debugging, navigation, testing and readability.

---

#### **AStar Class**
- **Purpose:** Manages the grid and performs pathfinding
- **Private Members:**
  - `grid`: 2D vector storing cell types
  - `rows`, `cols`: Grid dimensions
- **Private Helper Methods:**
  - `heuristic`: Manhattan distance
  - `getNeighbors`: generates 4 neighbour positions (4-way movement)
  - `reconstructPath`: rebuilds the final path from goal to start then reverses for display
- **Public Interface:**
  - `AStar`: constructs the base grid filled with `'.'`
  - `setCell`: updates a cell with walls, start and goal markers
  - `isPassable`: checks bounds and prevents going through walls (`#`)
  - `PrintGrid`: prints an ASCII version of the grid to the console
  - `Solve`: runs A* and returns a `PathResults` struct when complete
  - `getGrid`: exposes the grid as read only for the SFML renderer (encapsulation)

---
 
#### **Position Struct**
- **Purpose:** Represents a 2D coordinate on the grid.
- **Members:**
    - `x`: row coordinate
    - `y`: column coordinate
- **Operators:**
    - `operator==`: Enables position comparison (used to check if the goal is reached)
    - `operator<`: Enables use in `std::map` for path finding data structures
 
---

#### **PathResults Struct**
- **Purpose:** Bundles the output of the algorithm into one return value
- **Members:**
    - `pathFound`: whether a path exists or not
    - `path`: the coordinates from start to finish
    - `pathLength`: number of steps needed to be taken
    - `nodesExpanded`: number of nodes popped from the open set and processed
    - `executionTime`: runtime measured using `std::chrono`

---

#### **Node Struct (internal helper)**
- **Purpose:** Stores `(Position, fScore)` items inside the open set priority queue.
- **Members:**
  - `pos`: the position being considered
  - `fScore`: priority value `f(n) = g(n) + h(n)`
- **Why it exists:**
  - Allows `std::priority_queue` to always expand the lowest `fScore` node next.

---

#### **GridRenderer Class (SFML Visualiser)**
- **Purpose:** Optional visualisation layer that draws the grid and the final path using SFML.
- **How it interacts with the algorithm:**
  - Reads the grid using `AStar::getGrid()`
  - Draws the path using `PathResults.path`
- **Design benefit:**
  - Keeps rendering separate from pathfinding logic (clean separation of concerns).
 

### Design Decisions

#### Why Use a Constructor?
I chose to use a constructor with a member initializer list for several reasons:
1. **Automatic initialization:** Ensures the grid is always in a valid state
2. **Modern C++ best practice:** Aligns with C++11+ standards
3. **Efficiency:** Member initializer lists are more efficient than assignment in the constructor body
4. **Safety:** Prevents creation of uninitialized objects

```cpp
AStar::AStar(int r, int c) 
    : rows(r), cols(c),
      grid(r, std::vector<char>(c, '.'))
{
}
```

#### Grid Representation
I chose `char` for cell types rather than `int` or enum for clarity:
- `.` = open space
- `#` = wall
- `S` = start
- `G` = goal

This makes visual debugging immediate and intuitive.

---

#### std::map vs std::unordered_map
I decided to use `std::map<Position, ...>` for storing position relationships in the A* rather than `std::unordered_map`. While unordered_map offers 0(1) average lookup time vs maps 0(log n), this required implementing a custom hash function for Position.
**Rationale:**
- Map only requires `operator<`, which demonstrates operator overloading clearly
- For grid sizes used in testing (10x10), the performance difference is unnoticable
- Code clarity and maintainability were prioritised over the performance gains, as performance gains would be more complex than needed
- The implementation is easier to explain and debug

**PathResult Struct**
- **Purpose:** Bundles all pathfinding results together
- **Members:**
    - `pathFound`: Boolean indicating if a path exists
    - `path`: Vector of positions from start to goal
    - `nodesExpanded`: Statistics on algorithm efficiency
    - `pathLength`: Number of steps in the path
    - `executionTime`: Performance metric in seconds

## Algorithm Deep Dive

### The A* Algorithm

A* is an informed search algorithm that uses a heuristic to find the shortest path efficiently. The formula is:

```
f(n) = g(n) + h(n)
```
Where:
- **f(n)**: Total estimated cost (priority score)
- **g(n)**: Actual cost from start to current node
- **h(n)**: Heuristic estimate from current node to goal

## Heuristic Selection: Manhattan Distance

### Why Manhattan Distance?

I decided to go with the Manhattan distance as the heuristic function because:
1. **Admissibility**: Manhattan distance never overestimates the actual path cost for 4-way grid movement
2. **Consistency**: The heuristic is monotonic (satisfies the triangle inequality)
3. **Optimal for Grid Movement**: Good match for grids where diagonal movement is not allowed
4. **Computational Efficiency**: Simple calculation (no square roots like Euclidean distance)

#### Mathematical Definition

```
h(n) = |x1 - x2 | + |y1 - y2|
```
This calculates the minimum number of moves required if there was no obstacles.

#### Example
```
Current position: (2, 3)
Goal position: (8, 9)

h(n)  =  |2 - 8| + |3 - 9|
      =   6 + 6
      = 12 moves minimum
```
#### Admissibility Proof

For 4-way movement, Manhattan distance is admissible because:
- It represents the minimum possible path (straight line in grid terms)
- No valid path can be shorter than the Manhattan distance
- This guarantees A* finds the optimal path

#### Alternative Heuristics Considered
| Heuristic | Formula | Admissible? | Why Not Used? |
|-----------|---------|-------------|---------------|
| **Manhattan** | `|x₁-x₂| + |y₁-y₂|` | ✅ Yes | ✅ **Selected** |
| **Euclidean** | `√((x₁-x₂)² + (y₁-y₂)²)` | ✅ Yes | Weaker guidance than Manhattan for 4-way movement, adds unnecessary computation |

#### Heuristic Impact on Performance

The choice of heuristic affects:
- **Nodes expanded**: Better heuristics explore less unnecessary nodes
- **Optimality**: Admissible heuristics guarantee shortest path
- **Computation time**: Complex heuristics may slow down each iteration
- 
Research shows Manhattan distance is optimal for grid based pathfinding with 4-way movement ([8] Patel, 2019)

## Code Examples

### Grid Initialization

This example creates a 10×10 grid, places walls (`#`), sets a start (`S`) and goal (`G`), then prints the grid before running A*.

```cpp
int main() {
    // Create a 10x10 grid
    AStar astar(10, 10);
    
    // Add walls
    for (int r = 2; r < 8; ++r) {
        astar.setCell(r, 5, '#');
    }
    
    // Set start and goal
    astar.setCell(1, 1, 'S');
    astar.setCell(8, 8, 'G');
    
    astar.PrintGrid();
}
```

**Explanation:**
- `AStar grid(10, 10)` calls the constructor which initialises the grid with `'.'` open spaces.
- `setCell()` is used to place walls and special markers.
- The grid is printed using `PrintGrid()` for quick visual debugging.

### Output

```
=== A* Grid (MVP) ===
. . . . . . . . . . 
. S . . . . . . . . 
. . . . . # . . . . 
. # # # # # . . . . 
. . . . . # . . . . 
. . . . . # . . . . 
. . . . . # . . . . 
. . . . . # . . . . 
. . . . . . . . G . 
. . . . . . . . . . 
```

---

### Position Struct Usage

`Position` represents a coordinate in the grid and supports operators that are needed by the algorithm and STL containers.

```cpp
// Create positions
Position start(1, 1);
Position goal(8, 3);

// Check equality (used to determine if the goal has been reached)
if (start == goal) {
    std::cout << "Already at goal!\n";
}

// Ordered comparison (required for std::map keys)
if (start < goal) {
    std::cout << "Start comes before goal in ordering\n";
}
```

**Explanation:**
- `operator==` is used for comparisons such as `current == goal`.
- `operator<` defines an ordering so `Position` can be used as a key in `std::map` (used for `gScore`, `fScore`, and `cameFrom`).

---

### Helper Methods

#### Heuristic Function (Manhattan Distance)

```cpp
double AStar::heuristic(const Position& a, const Position& b) const {
    return abs(a.x - b.x) + abs(a.y - b.y);
}
```

**Explanation:**
This returns the Manhattan distance between two positions, which is suitable for 4-way movement. A* uses this value as `h(n)` when calculating `f(n) = g(n) + h(n)`.

---

#### Get Neighbours (4-way movement)

```cpp
vector<Position> AStar::getNeighbors(const Position& pos) const {
    vector<Position> neighbors;

    neighbors.push_back(Position(pos.x + 1, pos.y)); // Right
    neighbors.push_back(Position(pos.x, pos.y + 1)); // Down
    neighbors.push_back(Position(pos.x - 1, pos.y)); // Left
    neighbors.push_back(Position(pos.x, pos.y - 1)); // Up

    return neighbors;
}
```

**Explanation:**
This generates the four adjacent positions. The algorithm then checks each neighbour using `isPassable()` to ensure it is within bounds and not a wall (`#`).

---

#### Reconstruct Path (backtracking using `cameFrom`)

```cpp
vector<Position> AStar::reconstructPath(
    const map<Position, Position>& cameFrom,
    Position current
) const {
    vector<Position> path;
    path.push_back(current);

    while (cameFrom.find(current) != cameFrom.end()) {
        current = cameFrom.at(current);
        path.push_back(current);
    }

    reverse(path.begin(), path.end());
    return path;
}
```

**Explanation:**
- During A*, the algorithm stores parent relationships in `cameFrom`.
- Once the goal is reached, this method traces backwards from the goal to the start, then reverses the list to produce the final path in start to the goal order.
- This avoids storing full paths for every explored node (which would waste memory and add complexity).

---

### Example Output: `testComplexMaze()` (A* in action)

This test demonstrates A* solving a larger 15×15 maze. The first grid shows the maze layout before solving. After calling `Solve(start, goal)`, the program prints performance statistics and then prints the grid again with the final path marked using `*`.

#### Initial Maze Grid

```
--- A * Grid ---
S . . . . . . . . . # . . . .
. . . . . . . . . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . . . . . # . . . .
# # # # # # # # . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . . . . . # . . . .
. . . . # . # # # # # # # # #
. . . . # . . . . . . . . . .
. . . . # . . . . . . . . . .
. . . . . . . . . . . . . . .
. . . . . . . . . . . . . . G

```

**Explanation of the statistics:**
- **Path length**: number of moves from `S` to `G` (calculated as `results.path.size() - 1`), minus 1 to avoid counting the goal position.
- **Nodes expanded**: number of nodes removed from the open set (priority queue) and processed.
- **Execution time**: time taken for `Solve()` measured using `std::chrono`.

#### Maze With Solution Path

After a valid path is found, the program iterates over `results.path` and marks path cells with `*`. The start and goal cells are not overwritten.

```
Maze with Solution Path:
--- A * Grid ---
S . . . . . . . . . # . . . .
* * * * * * * * . . # . . . .
. . . . # . . * . . # . . . .
. . . . # . . * . . # . . . .
. . . . # . . * * . # . . . .
. . . . # . . . * . # . . . .
. . . . # . . . * . # . . . .
# # # # # # # # * . # . . . .
. . . . # . . . * . # . . . .
. . . . # * * * * . # . . . .
. . . . # * # # # # # # # # #
. . . . # * . . . . . . . . .
. . . . # * . . . . . . . . .
. . . . . * * * . . . . . . .
. . . . . . . * * * * * * * G
Path length: 34 steps
Nodes expanded: 129
Execution time: 0.0010723 seconds

Path: (0,0) -> (1,0) -> (1,1) -> (1,2) -> (1,3) -> (1,4) -> (1,5) -> (1,6) ->
      (1,7) -> (2,7) -> (3,7) -> (4,7) -> (4,8) -> (5,8) -> (6,8) -> (7,8) ->
      (8,8) -> (9,8) -> (9,7) -> (9,6) -> (9,5) -> (10,5) -> (11,5) -> (12,5) ->
      (13,5) -> (13,6) -> (13,7) -> (14,7) -> (14,8) -> (14,9) -> (14,10) -> (14,11) ->
      (14,12) -> (14,13) -> (14,14)
```

**What the path list represents:**
- This is the ordered list of positions returned by A* in `results.path`.
- It is produced by storing parent relationships in `cameFrom` and reconstructing the path once the goal is found.


## Testing & Validation

### Test Cases

All test cases are implemented in `TestCases.cpp`. Each test creates a new grid, places walls/start/goal markers, runs `AStar::Solve(start, goal)`, and validates behaviour using the returned `PathResults` and printed output.

> Note: `main.cpp` currently runs `testComplexMaze()`, `testNoPath()`, and `testStartEqualsGoal()`. The `testBasicPathfinding()` function exists but is not currently called from `main.cpp`.

---

#### Test 1: Basic Pathfinding (`testBasicPathfinding()`)
**Purpose:** Confirms that A* can find a valid route around obstacles and that the returned path can be visualised on the grid.

- **Setup:**
  - Grid size: 10×10
  - Obstacles:
    - Vertical wall at column 5 for rows 2 to 7
    - Horizontal wall at row 3 for columns 1 to 4
  - Start: `(1, 1)` marked as `S`
  - Goal: `(8, 3)` marked as `G`
- **Expected:**
  - `results.pathFound == true`
  - A path is returned and marked on the grid with `'*'`
- **Validation performed in code:**
  - If `results.pathFound` is true:
    - Prints: *Path Found!*
    - Prints: path length, nodes expanded, execution time (seconds)
    - Prints the coordinate path list
    - Marks the path on the grid with `'*'` (without overwriting `S` or `G`) and prints the updated grid
    - Prints: `TEST 1 PASSED`
  - Otherwise prints: `TEST 1 FAILED`

---

#### Test 2: No Path Available (`testNoPath()`)
**Purpose:** Ensures the algorithm correctly detects when a goal is unreachable.

- **Setup:**
  - Grid size: **10×10**
  - Start: `(1, 1)` marked as `S`
  - Goal: `(5, 5)` marked as `G`
  - Obstacles: goal is surrounded on all four sides by walls (`#`)
- **Expected:**
  - `results.pathFound == false`
- **Validation performed in code:**
  - Prints the initial grid
  - Runs A*
  - If `results.pathFound` is false:
    - Prints: `TEST 2 PASSED: Correctly detected no path`
    - Prints nodes expanded + execution time (seconds)

---

#### Test 3: Start Equals Goal (`testStartEqualsGoal()`)
**Purpose:** Validates the edge case where the start node is already the goal.

- **Setup:**
  - Grid size: 10×10
  - Start: `(5, 5)`
  - Goal: `(5, 5)`
- **Expected:**
  - `results.pathFound == true`
  - `results.pathLength == 0`
  - `results.nodesExpanded == 0`
- **Validation performed in code:**
  - Prints the initial grid
  - Runs A*
  - If `results.pathFound` is true and `results.pathLength == 0`:
    - Prints: `TEST 3 PASSED`
    - Prints nodes expanded (should be 0) and execution time (seconds)

---

#### Test 4: Edge Cases (`testEdgeCases()`)
**Purpose:** Tests input validation and small boundary scenarios.

This function contains three sub-tests:

**Sub-test A: Start on Wall**
- **Setup:** start cell is set to `'#'` and goal is set to `G`
- **Expected:** `results.pathFound == false`

**Sub-test B: Goal on Wall**
- **Setup:** goal cell is set to `'#'` (not passable)
- **Expected:** `results.pathFound == false`

**Sub-test C: Adjacent Positions**
- **Setup:** start and goal are next to each other 
- **Expected:** `results.pathFound == true` and `results.pathLength == 1`

At the end it prints: `TEST 4 COMPLETE`.

---

#### Test 5: Complex Maze (`testComplexMaze()`)
**Purpose:** Stress-tests the algorithm on a larger grid, outputs performance statistics, prints the final marked path, and launches SFML visualisation.

- **Setup:**
  - Grid size: **15×15**
  - Start: `(0, 0)` marked as `S`
  - Goal: `(14, 14)` marked as `G`
- **Expected:**
  - `results.pathFound == true`
  - A path is marked using `'*'` and the SFML renderer displays the result
- **Validation performed in code:**
  - If path found:
    - Prints path length, nodes expanded, execution time (seconds)
    - Marks the path with `'*'` and prints the solved maze
    - Prints the coordinate path list
    - Opens SFML window via `GridRenderer::render(grid, results)`
  - If no path found:
    - Prints failure message, nodes expanded
    - Still opens SFML window (useful for debugging why it failed)

---

## Project Management

### Development Timeline (MVP)

| Week | Date | Milestone | Status |
|------|------|-----------|--------|
| 4 | 09th Feb 2026 | Grid representation & constructor | Complete |
| 6 | 23rd Feb 2026 | Position struct, PathResult, helper methods | Complete |
| 7 | 02nd Mar 2026 | Weighted terrain | Planned (idea scrapped) |
| 9 | 16th Mar 2026 | Testing & documentation | Complete |

### Actual Development Timeline

| Week | Date | Milestone | Status |
|------|------|-----------|--------|
| 4 | 09th Feb 2026 | Grid representation & constructor | Complete |
| 5 | 16th Feb 2026 | Position struct, PathResult, helper methods | Complete |
| 6 | 23rd Feb 2026 | A* algorithm (`Solve`), path reconstruction, metrics (nodesExpanded + executionTime) | Complete |
| 7 | 02nd Mar 2026 | Testing (No Path, Start=Goal, Edge Cases) + documentation updates | Complete |
| 8 | 09th Mar 2026 | Complex maze scenario + path marking (`*`) | Complete |
| 9 | 16th Mar 2026 | SFML GridRenderer visualisation integration | Complete |

### Tools & Environment
- **IDE:** Visual Studio 2022
- **Compiler:** MSVC with C++14 standard
- **Version Control:** Git/GitHub & OneDrive
- **Documentation:** GitHub Pages

#### Summary
1. **Week 4:** Demonstrated grid printing, constructor-based initialisation, and `setCell()` wall placement
2. **Week 5:** Demonstrated `Position` + `PathResults` and early A* structure (open set + score maps)
3. **Week 6:** Demonstrated working `Solve()` including heuristic, neighbours, and reconstructing/marking the final path
4. **Week 7:** Demonstrated testing and validation for general + edge scenarios
5. **Week 8:** Demonstrated complex maze output and performance statistics
6. **Week 9:** Demonstrated SFML visualisation showing the same path results in a graphical format
   
---

## Reflection

### Challenges Encountered

#### Challenge 1: Understanding Constructors
**Problem:** Initially unfamiliar with constructor syntax and member initializer lists  
**Solution:** Researched C++ references and asked for clarification. Learned that initializer lists are more efficient than assignment in the constructor body.  
**Learning:** Constructors are essential for object-oriented design and ensure objects are always in a valid state.

#### Challenge 2: Grid Representation
**Problem:** Deciding between `int`, `char`, or `enum` for cell types  
**Solution:** Chose `char` for immediate visual feedback during debugging  
**Learning:** Simple representations often work best during development; can refactor later if needed.

#### Challenge 3: Understanding Operator Overloading
**Problem:** Initially not clear to me why Position needed `operator<` and when to use it vs `operator==`
**Solution:** Learned that `operator==` checks equality (if I am at the goal), while `operator<` defines ordering for containers like `std::map`. The map needs to organise positions, which requires a comparision function.
**Learning:** Operator overloading makes custom types work good with S|TL containers. The `operator<` creates lexicographic ordering (compare x first, then y), similar to dictonary sorting.

### What Worked Well
- **Incremental development:** Building MVP first gave a solid foundation
- **Visual output:** ASCII grid made debugging intuitive
- **OOP design:** Separating concerns into classes made code maintainable

### What I'd Do Differently
- Start with file-based grid loading instead of hardcoded grids
- Plan the Position struct earlier in the design phase
- Use more comprehensive testing from the beginning

### Key Learnings 

1. **Algorithm Selection Matters**: Choosing Manhattan distance over Euclidean made the implementation simpler and guaranteed optimality for 4-way movement.

2. **Memory vs. Speed Trade-offs**: Using `std::map` instead of `std::unordered_map` showed that optimization isn't always necessary, correctness come first.

3. **Operator Overloading Power**: Implementing `operator<` and `operator==` made Position objects work seamlessly with STL containers, demonstrating how C++ enables custom types to feel "built-in".

4. **Separation of methods**: Breaking A* into helper methods (heuristic, getNeighbors, reconstructPath) made the code easier to understand, test, and debug.

---

## AI Assistance Disclosure

### AI Tools Used
GitHub Copilot and ChatGPT were used for:
- Explaining constructor syntax and member initializer lists
- Understanding STL container usage (`std::vector`, `std::priority_queue`)
- Debugging compilation errors
- Clarifying A* algorithm pseudocode
- Explaining operator overloading
- Understanding std::map requirements and hash functions
- Clarifying Path Result struct design patterns

### How AI Was Used Appropriately
- All code suggestions were **adapted to my design**
- I **fully understand** every line of code in my project
- AI was used for **learning and clarification**, not copy-pasting solutions
- Code snippets were **modified and integrated** into my unique implementation

### Independent Work
- Grid design decisions were mine
- Class structure and method naming were my choices
- Testing scenarios were designed by me
- All demonstrations showed my understanding

---

## References

1. Hart, P. E., Nilsson, N. J., & Raphael, B. (1968). A Formal Basis for the Heuristic Determination of Minimum Cost Paths. *IEEE Transactions on Systems Science and Cybernetics*.

2. [A* Search Algorithm - Wikipedia](https://en.wikipedia.org/wiki/A*_search_algorithm)

3. [C++ Reference - std::vector](https://en.cppreference.com/w/cpp/container/vector)

4. [C++ Reference - std::priority_queue](https://en.cppreference.com/w/cpp/container/priority_queue)

5. Lecture notes and materials provided by Michelle Lynch, Atlantic TU.
  
6. [Setup std::vector in class constructor](https://stackoverflow.com/questions/11415469/setup-stdvector-in-class-constructor?newreg=3ea56c42483440c1947370acf4821c9d)
   
7. [W3Schools - C++ Constructors](https://www.w3schools.com/cpp/cpp_constructors.asp)
   
8. Patel, A. (2019) - [Introduction to the A* Algorithm*. Red Blob Games.](https://www.redblobgames.com/pathfinding/a-star/introduction.html)
   
9. [C++ Reference - std::map](https://en.cppreference.com/w/cpp/container/map)

---

**Repository:** [github.com/jamiemonty/cpp-astar-project](https://github.com/jamiemonty/cpp-astar-project)

**Live Site:** [jamiemonty.github.io/cpp-astar-project](https://jamiemonty.github.io/cpp-astar-project)
