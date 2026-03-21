/*   ____.              .__            _____                 __
    |    |____    _____ |__| ____     /     \   ____   _____/  |_  ____   ____   _____   ___________ ___.__.
    |    \__  \  /     \|  |/ __ \   /  \ /  \ /  _ \ /    \   __\/ ___\ /  _ \ /     \_/ __ \_  __ <   |  |
/\__|    |/ __ \|  Y Y  \  \  ___/  /    Y    (  <_> )   |  \  | / /_/  >  <_> )  Y Y  \  ___/|  | \/\___  |
\________(____  /__|_|  /__|\___  > \____|__  /\____/|___|  /__| \___  / \____/|__|_|  /\___  >__|   / ____|
              \/      \/        \/          \/            \/    /_____/              \/     \/       \/
Jamie Montgomery
19/01/2026
*/
#include <iostream> //  cout 
#include "AStar.h"  //  Print function 
#include <vector>   //  For vectors
#include <map>     //  For maps
#include <cmath>   //  For abs in heuristic
#include <chrono>  //  For timing
#include <queue>   //  For priority queue
#include <algorithm> //  For reverse in path reconstruction

//  A* Algorithm Implementation

//  Node struct(for priority queue)
struct Node {
    Position pos;
	double fScore; //  f(n) = g(n) + h(n)

    bool operator>(const Node& other) const {
        return fScore > other.fScore; //  Higher fScore has lower priority
	}
};

//  Constructor
AStar::AStar(int r, int c): rows(r), cols(c), grid(r, std::vector<char>(c, '.')) {}

//  Set a cell value for walls, start, goal
void AStar::setCell(int r, int c, char value) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        grid[r][c] = value;
    }
}

int AStar::getCols() const {
    return cols;
}

int AStar::getRows() const {
    return rows;
}

void AStar::PrintGrid() {
    std::cout << "--- A * Grid ---\n";
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << ' ';
        }
        std::cout << '\n';
    }
}

//  Check if a cell is passable
bool AStar::isPassable(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        return false;   //  Out of bounds
    }
    return grid[r][c] != '#';   //  Not a wall
}

//  Heuristic function 
double AStar::heuristic(const Position& a, const Position& b) const {
    return abs(a.x - b.x) + abs(a.y - b.y); //  Manhattan distance
}

//  Neighboring positions
vector<Position> AStar::getNeighbors(const Position& pos) const {
    vector<Position> neighbors;
    
	neighbors.push_back(Position(pos.x + 1, pos.y)); //  Right
	neighbors.push_back(Position(pos.x, pos.y + 1)); //  Down
	neighbors.push_back(Position(pos.x - 1, pos.y)); //  Left
	neighbors.push_back(Position(pos.x, pos.y - 1)); //  Up

    return neighbors;
}

vector<Position> AStar::reconstructPath(const map<Position, Position>& cameFrom, Position current) const {
    vector<Position> path;
	path.push_back(current);    //  Start from the goal and work backwards

    while (cameFrom.find(current) != cameFrom.end()) {
		current = cameFrom.at(current); //  Get previous position
		path.push_back(current);    //  Add to path
    }

	reverse(path.begin(), path.end()); //  Reverse to get path from start to goal

    return path;

}

PathResults AStar::Solve(const Position& start, const Position& goal) {
	//  Timing the execution
	auto startTime = std::chrono::high_resolution_clock::now();

	PathResults results;

	if (!isPassable(start.x, start.y) || !isPassable(goal.x, goal.y)) {
		cout << "Start or goal position is not passable.\n";
		return results; //  Return empty results
	}

	//  Check if already at the goal
	if (start == goal) {
		results.pathFound = true;
		results.path = { start }; //  Path is just the start/goal
		results.pathLength = 0;
		results.executionTime = 0.0; //  No time taken
		results.nodesExpanded = 0; //  No nodes expanded
		return results;
	}

	//  Initialise A* data strutures
	priority_queue<Node, vector<Node>, greater<Node>> openSet; //  Min-heap based on fScore

	//  Maps to for tracking costs and paths
	map<Position, Position> cameFrom; //  For path reconstruction
	map<Position, double> gScore; //  Cost from start to current node
	map<Position, double> fScore; //  Estimated total cost from start to goal through current node

	//  Initialise start position
	gScore[start] = 0.0;
	fScore[start] = heuristic(start, goal);
	openSet.push({ start, fScore[start] }); //  Add start to open set

	int nodesExpanded = 0;

	while (!openSet.empty()) {
		Node current = openSet.top();
		openSet.pop();

		nodesExpanded++;

		if (current.pos == goal) {
			results.pathFound = true;
			results.path = reconstructPath(cameFrom, goal);
			results.pathLength = static_cast<int>(results.path.size()) - 1;	//  Don't count the start position
			results.nodesExpanded = nodesExpanded;

			//  Time taken
			auto endTime = std::chrono::high_resolution_clock::now();
			results.executionTime = std::chrono::duration<double>(endTime - startTime).count();

			return results;
		}
		//  Explore neighbors
		vector<Position> neighbors = getNeighbors(current.pos);

		for (const Position& neighbor : neighbors) {
			//  Skip if not passable or out of bounds
			if (!isPassable(neighbor.x, neighbor.y)) {
				continue;
			}

			//  Calculate tentative gScore
			double tentativeGScore = gScore[current.pos] + 1.0;

			if (gScore.find(neighbor) == gScore.end() || tentativeGScore < gScore[neighbor]) {
				//  This path to neighbor is better than any previous one
				cameFrom[neighbor] = current.pos; //  Update path
				gScore[neighbor] = tentativeGScore; //  Update gScore
				fScore[neighbor] = tentativeGScore + heuristic(neighbor, goal); //  Update fScore
				//  Add to open set if not already there
				openSet.push({ neighbor, fScore[neighbor] });
			}
		}
	}

	//  No path found
	results.pathFound = false;
	results.nodesExpanded = nodesExpanded;

	//  Time taken
	auto endTime = std::chrono::high_resolution_clock::now();
	results.executionTime = std::chrono::duration<double>(endTime - startTime).count();

	return results;
}