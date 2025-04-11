#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <fstream>
#include <sstream>
#include "graph.h"
#include "entity.h"

const char WALL = '#';
const char SAFE = 'H';
const char FREE = ' ';
const char SLOW = '-';
const char ENEM = 'X';
const char PLAY = 'O';

static void Exit(const std::string& message)
{
	system("cls");
	std::cout << message << std::endl;
	exit(0);
}

class Map
{
	int width = 0, height = 0;
	bool safe = false;
	std::pair<int, int> safe_loc = std::make_pair(-1, -1);

	Entity player, enemy1, enemy2, enemy3;
	Matrix<char> display;
	Matrix<int> reference;
	Graph graph;

	std::pair<int, int>* directions = new std::pair<int, int>[4];

public:
	Map() : player(PLAY), enemy1(ENEM), enemy2(ENEM), enemy3(ENEM) {}

	// Create a corresponding graph from the map
	void CreateGraph();

	// Load coords into characters from file and put 
	// the characters on the map on initialization
	void InitChars(std::istringstream& file_ss);

	// Load the reference map in prep for graph creation
	void LoadReference(char c, int row, int col, int& v);

	// load display from file and create a copy for graph reference and
	// return number of traversable tiles (vertices) for the graph
	int LoadDisplay(std::ifstream& file, std::string& buffer);

	// Put characters on the display map
	void LoadPlayer();
	void LoadEnemy(Entity& enemy);

	// Load characters, display/ref matrices and graph
	void LoadAll(const std::string& file_name);

	// Shortest path functions
	void UpdateShortestPath(Entity& enemy);
	void UpdateAllShortestPath();

	// Update map matrix whenever a character moves
	void Update(Entity& entity, int new_x, int new_y);

	// move an enemy character to the player using shortest path
	void MoveEnemy(Entity& enemy);
	void MoveAllEnemies();

	// Read input from user
	void ReadKeyInput();

	// Display coordinates
	void DisplayCoordinates(Entity& e);

	// Display information such as statuses and coords of characters
	void DisplayStatus();

	// Display the map to user
	void Display();
};