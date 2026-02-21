
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
| 1 | 19 Jan 2026 | Grid representation & constructor | ✅ Complete |
| 2 | 26 Jan 2026 | A* algorithm implementation | 🔄 In Progress |
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

### What Worked Well
- **Incremental development:** Building MVP first gave a solid foundation
- **Visual output:** ASCII grid made debugging intuitive
- **OOP design:** Separating concerns into classes made code maintainable

### What I'd Do Differently
- Start with file-based grid loading instead of hardcoded grids
- Plan the Position struct earlier in the design phase
- Use more comprehensive testing from the beginning

---

## AI Assistance Disclosure

### AI Tools Used
GitHub Copilot and ChatGPT were used for:
- Explaining constructor syntax and member initializer lists
- Understanding STL container usage (`std::vector`, `std::priority_queue`)
- Debugging compilation errors
- Clarifying A* algorithm pseudocode

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

---

**Repository:** [github.com/jamiemonty/cpp-astar-project](https://github.com/jamiemonty/cpp-astar-project)

**Live Site:** [jamiemonty.github.io/cpp-astar-project](https://jamiemonty.github.io/cpp-astar-project)
