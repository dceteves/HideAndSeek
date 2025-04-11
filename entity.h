#pragma once

#include <random>

class Entity
{
	char type;		// 'O' for player, 'X' for enemies 
	char pos;		// holds the map tile of the current position of character
	int x, y;
	bool slow;		// mainly for enemies
	int move_tick;	// For slow movement
	int* path;		// For enemies' shortest path

public:
	Entity(char c = ' ', char p = ' ', int _x = -1, int _y = -1) 
		: type(c), pos(p), x(_x), y(_y), slow(false), move_tick(-1), path(nullptr) {}

	void InitPath(int v) 
	{
		path = new int[v];
		for (int i = 0; i < v; i++)
			path[v] = -1;
	}

	int  get_x()	const { return x; }
	int  get_y()	const { return y; }
	char get_type() const { return type; }
	char get_pos()	const { return pos; }
	int  get_tick()	const { return move_tick; }
	bool is_slow()	const { return slow; }
	bool on_map()	const { return x > -1 || y > -1; }
	int* get_path() const { return path; }

	void set_coord(int _x, int _y) { x = _x; y = _y; }
	void set_pos(char c) { pos = c; }
	void set_tick(int t) { move_tick = t; }
	void set_slow(bool s) { slow = s; }
	void set_path(int* p) { path = p; }

	void move_amount(int _x, int _y) { x += _x; y += _y; }
	void move_up() { y--; }
	void move_down() { y++; }
	void move_left() { x--; }
	void move_right() { x++; }
	
	void uptick() { move_tick++; }

};

