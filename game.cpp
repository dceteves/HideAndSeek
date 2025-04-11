#include "game.h"
#include "matrix.h"
#include "graph.h"
#include "entity.h"

Game::Game()
{
    std::string file_name;
    std::cout << "Input a .txt file name: ";
    std::cin >> file_name;
    file_name.append(".txt");
    map.LoadAll(file_name);
}

Game::Game(const std::string& file_name)
{
    map.LoadAll(file_name);
}

void Game::Run()
{
    while (true)
    {
        map.Display();
        map.DisplayStatus();
        map.ReadKeyInput();

        Sleep(30);
        system("cls");
    }
}