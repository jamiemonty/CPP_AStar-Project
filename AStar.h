#pragma once
#ifndef ASTAR_H
#define ASTAR_H

#include <vector>	//	For vectors
#include <map>		//	For maps

using namespace std;

//	Position struct, represents a 2D position on the grid
struct Position
{
	int x;
	int y;

	//	Starts the member initialiser list, sets x and y to 0 by default
	Position(int x = 0, int y = 0) : x(x), y(y) {}
	
	bool operator==(const Position& other) const {
		return x == other.x && y == other.y;
	}

	//	Overload the less than operator for sorting positions
	bool operator<(const Position& other) const {
		if (x != other.x) {
			return x < other.x;
		}
		return y < other.y;
	}
};

//	Struct to hold the results of the pathfinding algorithm
struct PathResults
{
	bool pathFound;	
	vector<Position> path;	//	The path from start to goal
	int nodesExpanded;	//	Number of nodes expanded during the search
	int pathLength;
	double executionTime;	

	PathResults() : pathFound(false), nodesExpanded(0), pathLength(0), executionTime(0.0) {}

};


//	Manages the grid 
class AStar
{
private:
	std::vector<std::vector<char>> grid;	//	2D grid for the characters
	int rows, cols;	//	Grid dimensions

	double heuristic(const Position& a, const Position& b) const;	//	Heuristic function for A* (Manhattan distance)
	vector<Position> getNeighbors(const Position& pos) const;	//	Get valid neighboring positions
	vector<Position> reconstructPath(const map<Position, Position>& cameFrom, Position current) const;	//	Reconstruct the path from start to goal

public:
	//	Function that runs when a AStar object is created
	AStar(int r, int c);

	void setCell(int r, int c, char value);

	int getRows() const;
	int getCols() const;
	bool isPassable(int r, int c) const;

	//	Display 
	void PrintGrid();

	PathResults Solve(const Position& start, const Position& goal);

	// For the visualiser to read cells 
	const std::vector<std::vector<char>>& getGrid() const { return grid; }
};

#endif