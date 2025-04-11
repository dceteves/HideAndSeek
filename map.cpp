#include "map.h"

void Map::CreateGraph()
{
    for (int i = 1; i < width - 1; i++)
    {
        for (int j = 1; j < height - 1; j++)
        {
            int curr = reference[j][i];

            // A tile is connected if it is one space adjacent 
            // only to its up, right, down, and left directions
            int up = reference[j - 1][i];
            int down = reference[j + 1][i];
            int left = reference[j][i - 1];
            int right = reference[j][i + 1];

            // If the current tile is a slow tile, the edge will 
            // have a weight of 5, otherwise a weight of 1
            char curr_tile = display[j][i];
            int weight = curr_tile == SLOW ? 5 : 1;

            // If you can move up, down, left or right, add an
            // edge to its corresponding vertices in the graph
            if (curr > -1 && up > -1)
                graph.AddEdge(curr, up, weight);
            if (curr > -1 && down > -1)
                graph.AddEdge(curr, down, weight);
            if (curr > -1 && left > -1)
                graph.AddEdge(curr, left, weight);
            if (curr > -1 && right > -1)
                graph.AddEdge(curr, right, weight);
        }
    }
}

void Map::InitChars(std::istringstream& file_ss)
{
    int x_buffer, y_buffer;

    file_ss >> x_buffer >> y_buffer;
    player.set_coord(x_buffer, y_buffer);
    file_ss >> x_buffer >> y_buffer;
    enemy1.set_coord(x_buffer, y_buffer);
    file_ss >> x_buffer >> y_buffer;
    enemy2.set_coord(x_buffer, y_buffer);
    file_ss >> x_buffer >> y_buffer;
    enemy3.set_coord(x_buffer, y_buffer);
}

void Map::LoadReference(char c, int row, int col, int& v)
{
    if (c == FREE || c == SLOW || c == PLAY || c == ENEM) {
        reference.ChangeMatrixValue(row, col, v);
        v++;
    }
    else {
        reference.ChangeMatrixValue(row, col, -1);
    }
}

int Map::LoadDisplay(std::ifstream& file, std::string& buffer)
{
    int r_count = 0, free_spaces = 0;
    while (getline(file, buffer))
    {
        std::istringstream ch_ss(buffer);
        for (int i = 0; i < buffer.size(); i++)
        {
            char c;
            ch_ss >> std::noskipws >> c;

            if (c == SAFE)
                safe_loc = std::make_pair(r_count, i);

            LoadReference(c, r_count, i, free_spaces);
            display.ChangeMatrixValue(r_count, i, c);
        }
        r_count++;
    }
    return free_spaces;
}

void Map::LoadPlayer()
{
    player.set_pos(display[player.get_y()][player.get_x()]);
    player.set_tick(0);
    display.ChangeMatrixValue(player.get_y(), player.get_x(), PLAY);
}

void Map::LoadEnemy(Entity& enemy)
{
    enemy.set_pos(display[enemy.get_y()][enemy.get_x()]);
    enemy.set_tick(0);
    enemy.set_path(graph.ShortestPath(reference[enemy.get_y()][enemy.get_x()]));
    display.ChangeMatrixValue(enemy.get_y(), enemy.get_x(), ENEM);
}

void Map::LoadAll(const std::string& file_name)
{
    int vertices;

    // Initialize file and check if it exists (exit if it does not)
    std::ifstream file(file_name);
    if (!file)
        Exit("File does not exist");

    // Initialize string buffers
    std::string buffer;
    std::getline(file, buffer);
    std::istringstream file_ss(buffer);

    // initialize map and reference matrices
    file_ss >> width >> height;
    display.SetDimensions(height, width);
    reference.SetDimensions(height, width);
    display.InitMatrix();
    reference.InitMatrix();

    // get vertices from LoadMap()
    vertices = LoadDisplay(file, buffer);

    // Create the graph
    graph.InitGraph(vertices);
    CreateGraph();

    // Load player and enemies
    InitChars(file_ss);
    LoadPlayer();
    if (enemy1.on_map())
        LoadEnemy(enemy1);
    if (enemy2.on_map())
        LoadEnemy(enemy2);
    if (enemy3.on_map())
        LoadEnemy(enemy3);

    file.close();
}

void Map::UpdateShortestPath(Entity& enemy)
{
    int x = enemy.get_x(), y = enemy.get_y();
    enemy.set_path(graph.ShortestPath(reference[y][x]));
}

void Map::UpdateAllShortestPath()
{
    if (enemy1.on_map())
        UpdateShortestPath(enemy1);
    if (enemy2.on_map())
        UpdateShortestPath(enemy2);
    if (enemy3.on_map())
        UpdateShortestPath(enemy3);
}

void Map::Update(Entity& entity, int new_x, int new_y)
{
    char entity_type = entity.get_type();
    char new_loc = display[new_y][new_x];

    // Get all exception cases out of the way
        
    // Case 1: An enemy character is about to move into a safe space (do nothing)
    int safe_x = safe_loc.second, safe_y = safe_loc.first;
    if (entity_type == ENEM && (new_x == safe_x && new_y == safe_y))
        return;
    
    // Case 2: player and enemy bump into each other (send player to safe tile)
    if ((entity_type == PLAY && new_loc == ENEM) || (entity_type == ENEM && new_loc == PLAY)) {
        Update(player, safe_x, safe_y);
        return;
    }
     
    // Case 3: The new location is a wall (do nothing)
    if (new_loc == WALL)
        return;

    // Case 4: An enemy character is about to bump into another enemy character (do nothing)
    if (entity_type == ENEM && new_loc == ENEM)
        return;

    // Case 5: The entity is in grass (only increment entity tick)
    if (entity.is_slow() && entity.get_tick() < 3) {
        entity.uptick();
        return;
    }
    entity.set_tick(0);

    // Once the exception cases are passed, execute the function
    int x = entity.get_x(), y = entity.get_y();
    char pos = entity.get_pos();
    entity.set_pos(new_loc);
    entity.move_amount(new_x - x, new_y - y);
    display.ChangeMatrixValue(y, x, pos);
    display.ChangeMatrixValue(new_y, new_x, entity_type);

    // If the player moves, update the enemies' shortest path to the player 
    if (entity_type == PLAY)
        UpdateAllShortestPath();

    // if a character enters a slow (grass) tile
    if (new_loc == SLOW)
        entity.set_slow(true);
    else
        entity.set_slow(false);

    // If player enters/exits a safe tile 
    if (new_loc == SAFE && entity_type == PLAY)
        safe = true;
    if (new_loc != SAFE && entity_type == PLAY)
        safe = false;
}

void Map::MoveEnemy(Entity& enemy)
{
    // If the enemy is not on the map, don't do anything
    if (!enemy.on_map())
        return;

    // if user is in safe tile, randomly move a space
    if (safe)
    {
        directions[0] = std::make_pair(0, -1); // up
        directions[1] = std::make_pair(0, 1);  // down 
        directions[2] = std::make_pair(-1, 0); // left
        directions[3] = std::make_pair(1, 0);  // right
        auto rand_dir = directions[rand() % 4];
        int x = rand_dir.first, y = rand_dir.second;
        Update(enemy, enemy.get_x() + x, enemy.get_y() + y);
        return;
    }

    // Once excception cases pass, execute code
    int target = reference[player.get_y()][player.get_x()];
    int source = reference[enemy.get_y()][enemy.get_x()];
    int* path = enemy.get_path();
    int previous = target;
    while (path[previous] != source)
        previous = path[previous];

    // find where the vertex is on the map
    int x = enemy.get_x(), y = enemy.get_y();
    if (reference[y - 1][x] == previous)
        y--;
    else if (reference[y + 1][x] == previous)
        y++;
    else if (reference[y][x - 1] == previous)
        x--;
    else if (reference[y][x + 1] == previous)
        x++;

    // Update map
    Update(enemy, x, y);
}

void Map::MoveAllEnemies()
{
    if (enemy1.on_map())
        MoveEnemy(enemy1);
    if (enemy2.on_map())
        MoveEnemy(enemy2);
    if (enemy3.on_map())
        MoveEnemy(enemy3);
}

void Map::ReadKeyInput()
{
    // quit the program
    if (GetKeyState('Q') & 0x8000)
        Exit("Exited program");

    // movement controls
    if ((GetKeyState('W') & 0x8000) || (GetKeyState(VK_UP) & 0x8000)) {
        Update(player, player.get_x(), player.get_y() - 1);
        MoveAllEnemies();
    }
    if ((GetKeyState('S') & 0x8000) || (GetKeyState(VK_DOWN) & 0x8000)) {
        Update(player, player.get_x(), player.get_y() + 1);
        MoveAllEnemies();
    }
    if ((GetKeyState('A') & 0x8000) || (GetKeyState(VK_LEFT) & 0x8000)) {
        Update(player, player.get_x() - 1, player.get_y());
        MoveAllEnemies();
    }
    if ((GetKeyState('D') & 0x8000) || (GetKeyState(VK_RIGHT) & 0x8000)) {
        Update(player, player.get_x() + 1, player.get_y());
        MoveAllEnemies();
    }
    if (GetKeyState(VK_SPACE) & 0x8000) {
        MoveAllEnemies();
    }
}

void Map::DisplayCoordinates(Entity& e)
{
    std::cout << "Coords: (";
    std::cout << e.get_x() << ", ";
    std::cout << e.get_y() << ") ";
}

void Map::DisplayStatus()
{
    std::cout << "Player ";
    DisplayCoordinates(player);

    if (safe) {
        std::cout << "[safe!]";
    }
    std::cout << std::endl;

    std::cout << "Enemy 1 ";
    DisplayCoordinates(enemy1);
    std::cout << std::endl;

    std::cout << "Enemy 2 ";
    DisplayCoordinates(enemy2);
    std::cout << std::endl;

    std::cout << "Enemy 3 ";
    DisplayCoordinates(enemy3);
    std::cout << std::endl;
}

void Map::Display()
{
    display.DisplayMatrix();
}