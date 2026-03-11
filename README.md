
**# A* Pathfinding Algorithm
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
- Grid-based representation with obstacles
- Manhattan heuristic for distance estimation
- 4-way movement (extensible to 8-way)
- Weighted terrain support
- Visual ASCII output

### Motivation
A* is widely used in robotics, game development, and navigation systems. This project demonstrates understanding of:
- Heuristic search algorithms
- Object-oriented design in C++
- STL container usage
- Algorithm efficiency considerations

---

## Design & Implementation

### Architecture

#### UML Class Diagram (MVP)
This is where I began, I found a tool to visualise the minimal viable products UML Class Diagram, this is subject to change as I hope to add more features along the way. 
**<img width="693" height="281" alt="image" src="https://github.com/user-attachments/assets/98d98a83-a078-4766-a187-883fa0f50ec6" />

### Class Structure

**AStar Class**
- **Purpose:** Manages the grid and performs pathfinding
- **Private Members:**
  - `grid`: 2D vector storing cell types
  - `rows`, `cols`: Grid dimensions
- **Public Interface:**
  - Constructor for initialization
  - `setCell()`: Modify grid cells
  - `PrintGrid()`: Visualize the grid
  - `Solve()`: Perform A* pathfinding (to be implemented)
 
**Position Struct**
- **Purpose:** Represents a 2D  coordinate on the grid
- **Members:**
    - `x`: Column coordinate
    - `y`: Row coordinate
- **Operators:**
    - `operator==`: Enables position comparison (used to check if the goal is reached)
    - `operator<`: Enables use in `std::map` for path finding data structures

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
- Future: `r`, `g`, `w` for terrain types

This makes visual debugging immediate and intuitive.

---

#### std::map vs std::unordered_map
I decided to use `std::map<Position>` for storing position relationships in the A* rather than `std::unordered_map`. While unordered_map offers 0(1) average lookup time vs maps 0(log n), this required implementing a custom hash function for Position.
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
    - `timeTaken`: Performance metric in milliseconds

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
2. **Consistency**: The heuristic is montonic (satisfies the triangle inequality)
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
| **Euclidean** | `√((x₁-x₂)² + (y₁-y₂)²)` | ❌ No* | Underestimates for 4-way movement |
*Euclidean is admissible for 8-way diagonal movement but underestimates for 4-way movement, potentially finding suboptimal paths.

#### Heuristic Impact on Performance

The choice of heuristic affects:
- **Nodes expanded**: Better heuristics explore less unnecessary nodes
- **Optimality**: Admissible heuristics guarantee shortest path
- **Computation time**: Complex heuristics may slow down each iteration
Research shows Manhattan distance is optimal for grid based pathfinding with 4-way movement (Patel, 2019)

## Code Examples

### Grid Initialization

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

```cpp
//  Create positions
Position start(1, 1)
Position goal(8, 8)

// Place on grid
astar.setCell(start.x, start.y, 'S');
astar.setCell(goal.x, goal.y, 'G');

// Check equality
if (start == goal) {
    cout << "Already at goal!\n";
}

// Compare positions (for std::map)
if (start < goal) {
    cout << "Start comes before goal in ordering\n";
}
```
---

### Helper Methods

**Heuristic Function (Manhattan Distance)**
```cpp
double AStar::heuristic(const Position& a, const Position& b) const {
    return abs(a.x - b.x) + abs(a.y - b.y);
}
```
Estimates the minimum distance between two positions. Used by A* to prioritise which position to check first.

**Get Neighbours**
```cpp
vector<Position> AStar::getNeighbors(const Position& pos) const {
    // Returns positions: right, down, left, up
    // 4-way movement (no diagonals)
}
```
Returns all adjacent positions for the Manhattan 4-way grid movement.

**Reconstruct Path**
```cpp
vector<Position> AStar::reconstructPath(
    const map<Position, Position>& cameFrom, 
    Position current) const {
    // Traces backwards from goal to start
    // Returns path from start to goal
}
```
After A* finds the goal, this method traces backwards through the `cameFrom` map to build the complete path, instead of recording results as the algorithm finds the path, as it might explore many wrong paths before finding the goal. Also storing the entire path for every dead end is wasted memory, slow, and complex.

## Testing & Validation

### Test Cases

#### Test 1: Basic Pathfinding
**Setup:** 10×10 grid with vertical wall  
**Expected:** Path found around obstacle  
**Result:** ✅ Pass

#### Test 2: No Path Available
**Setup:** Goal surrounded by walls  
**Expected:** No path found  
**Result:** ✅ Pass

#### Test 3: Start Equals Goal
**Setup:** Same position for start and goal  
**Expected:** Path length = 0  
**Result:** ✅ Pass

---

## Project Management

### Development Timeline

| Week | Date | Milestone | Status |
|------|------|-----------|--------|
| 1 | 19 Jan 2026 | Grid representation & constructor | Complete |
| 2 | 26 Jan 2026 | Position struct, PathResult, helper methods | Complete |
| 3 | 02 Feb 2026 | Weighted terrain | 📅 Planned |
| 4 | 09 Feb 2026 | Testing & documentation | 📅 Planned |

### Tools & Environment
- **IDE:** Visual Studio 2022
- **Compiler:** MSVC with C++14 standard
- **Version Control:** Git & GitHub
- **Documentation:** GitHub Pages (Markdown)

### Progress Tracking
I used lab sessions as checkpoints:
1. **Week 1:** Demonstrated grid printing and class structure
2. **Week 2:** Will demonstrate pathfinding algorithm
3. **Week 3:** Will demonstrate enhanced features

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
