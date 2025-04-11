#pragma once

#include "map.h"

class Game
{
    Map map;
    int width = -1, height = -1;

public:
    // Instatiate via user input by default cstor
    Game(); 

    // Pass in file name directly by param cstor
    Game(const std::string& file_name); 

    // Actively run the game
    void Run();
};

