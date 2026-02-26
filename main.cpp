#include "3dMS.hpp"

const float bomb_percent = 0.2; //percentage of total tiles that are bombs

//dimensions of the board
const int width =  3;
const int height = 3;
const int depth =  3;

std::vector<int> tile_list = {}; //list of all the tiles

int format_input(const std::string question, std::string input, const std::vector<int> acceptable_inputs, const bool first_test)
//takes the players input an ensures it is acceptable for the program
{
  if (input == "exit" || input == "e")
  {
    abort();
  }

  if (first_test == false)
  {
    std::cout << question;
    std::cin >> input;
    std::cout << "\x1B[1A";
  }
  std::cout << "\x1B[1A";

  try 
  {
    std::stoi(input);
  }
  catch (std::invalid_argument &e)
  {
    std::cout << "Input contained a non-integer charactar.\n";
    format_input(question, input, acceptable_inputs, false);
  }

  const int int_input = stoi(input);
  if (int_input < 1 || int_input > width)
  {
    std::cout << "Input falls outside the accepted range of integers.\n";
    format_input(question, input, acceptable_inputs, false);
  }

  return int_input;
}

char format_input(const std::string question, std::string input, const std::vector<char> acceptable_inputs, const bool first_test)
{

}

bool won()
//return true or false depending on wether the player has won or not
{
  for (int i = 4; i < tile_list.size(); i += 5)
  {
    if (tile_list[i] == 0)
    {
      return false;
    }
    else if (tile_list[i] == 2)
    {
      if (tile_list[i - 1] > -1)
      {
        return false;
      }
    }
  }

  return true;
}

bool search_list(const std::vector<int> list, const std::vector<int> item)
//checks if a sub-list matches a section in the larger list
{
  int item_length = item.size();
  int matching_nums = 0;

  for (int i = 0; i < list.size() - item_length; i += item_length)
  {
    matching_nums = 0;
    for (int j = 0; j < item_length; j++)
    {
      if (list[i + j] == item[j])
      {
        matching_nums++;
      }
    }

    if (matching_nums == item_length)
    {
      return true;
    }
  }

  return false;
}

std::vector<int> nearby_tile_idxes(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx)
{
  std::vector<int> idx_list = {};

  //the coords of the tile selected
  int tile_x = tile_coords[0];
  int tile_y = tile_coords[1];
  int tile_z = tile_coords[2];
  
  //where to start the search, cuts off out-of-bounds tiles (lower x, y, or z than possible)
  int x_start_benchmark = -1;
  int y_start_benchmark = -1;
  int z_start_benchmark = -1;

  int x_start;
  int y_start;
  int z_start;

  //where to end the search, cuts off out-of-bounds tiles (higher x, y, or z than possible)
  int x_end = 1;
  int y_end = 1;
  int z_end = 1;

  //finds if the tile is on an edge and cuts off the search by setting the end and start variables accordingly
  if (tile_x == 0) {x_start_benchmark = 0;}             //if the tile is on the left wall, dont search past the left wall
  if (tile_x == width - 1) {x_end = 0;} //if the tile is on the right wall, dont search past the right wall

  if (tile_y == 0) {y_start_benchmark = 0;}
  if (tile_y == height - 1) {y_end = 0;}

  if (tile_z == 0) {z_start_benchmark = 0;}
  if (tile_z == depth - 1) {z_end = 0;}

  //searches each tile in 3x3x3 radius, cutting off out-of-bounds tiles using start and end variables
  for (z_start = z_start_benchmark; z_start <= z_end; z_start++)
  {
    for (x_start = x_start_benchmark; x_start <= x_end; x_start++)
    {
      for (y_start = y_start_benchmark; y_start <= y_end; y_start++)
      {
        if (x_start != 0 || y_start != 0 || z_start != 0) //prevents the check from checking its own tile, not neccisary
        {
          int idx = tile_idx;

          //moves the index to the info of the tile being searched right now
          idx += z_start * width * height * 5; //z (depth) translation
          idx += y_start * height * 5; //y translation
          idx += x_start * 5; //x translation

          //if the tile is a bomb add it to the bomb counter
          idx_list.push_back(idx);
        }
      }
    }
  }

  return idx_list;
}

int surrounding_bombs(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx)
//counts the number of bombs surroudning a selected tile
{
  //counter of the bombs detected
  int bomb_count = 0;
  std::vector<int> tile_idxes = nearby_tile_idxes(tile_list, tile_coords, tile_idx);

  for (int i = 0; i < tile_idxes.size(); i++)
  {
    if (tile_list[tile_idxes[i] + 3] == -1)
    {
      bomb_count++;
    }
  }

  return bomb_count;
}

std::vector<int> fill_board()
{
  std::vector<int> available_tiles; //a list of all the tiles that the bombs are pulled from
  std::vector<int> bomb_coords; //a flattened 3d array of the coords of all the bombs
  const int bomb_count = std::floor((width * height* depth) * bomb_percent); //number of bombs

  //filling the availables tiles list with a flattened 3d array of the coordinates of every tile
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      for (int z = 0; z < depth; z++)
      {
        available_tiles.push_back(x);
        available_tiles.push_back(y);
        available_tiles.push_back(z);
      }
    }
  }

  //picks the bombs from the list at random
  for (int i = 0; i < bomb_count; i++)
  {
    //random idx to draw bomb from
    int rand_idx = (rand() % ((available_tiles.size() - 3) / 3)) * 3 - 1;
    
    //adds bomb to bomb_list
    bomb_coords.push_back(available_tiles[rand_idx]);
    bomb_coords.push_back(available_tiles[rand_idx + 1]);
    bomb_coords.push_back(available_tiles[rand_idx + 2]);

    //removes bomb coordinate from list to avoid repeating indexes
    available_tiles.erase(available_tiles.begin() + rand_idx, available_tiles.begin() + rand_idx + 2);
  }

  //adds the bombs from bomb list to a final list of tiles to return
  std::vector<int> return_tiles = {};
  for (int z = 0; z < depth; z++)
  {
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        //adds coordinates to list
        return_tiles.push_back(x);
        return_tiles.push_back(y);
        return_tiles.push_back(z);
        //checks if tile is a bomb or not and pushes appropriate tile info
        if (search_list(bomb_coords, std::vector<int> {x, y, z}))
        {
          return_tiles.push_back(-1); //if bomb
        }
        else
        {
          return_tiles.push_back(0); //if not a bomb
        }
        
        return_tiles.push_back(0); //ifn uncovered or covered. 0 for covered, 1 for uncovered, 2 for flagged
      }
    }
  }


  //fills in the number of surroudning bombs for each non-bomb tile
  for (int i = 0; i < return_tiles.size(); i += 5)
  {
    //if tile is not a bomb, find the number of surrounding bombs
    if (return_tiles[i + 3] != -1)
    {
      return_tiles[i + 3] = surrounding_bombs(return_tiles, std::vector<int> {return_tiles.begin() + i, return_tiles.begin() + i + 3}, i);
    }
  }

  return return_tiles;
}

void print_board()
{
  const std::string covered_color = "\x1B[48:5:8m";
  const std::string uncovered_color = "\x1B[48:5:7m";
  const std::string flag_color = "\x1B[48:5:12m";
  const std::string bomb_color = "\x1B[48:5:9m";
  const std::string border_color = "\x1B[48:5:20m";
  const std::string end_color = "\x1B[0m";


  std::cout << "\n\n\t3D MINESWEEPER\n\n";

  for (int z = 0; z < depth; z++)
  {
    std::cout << "\tLayer: " << z + 1 << "\n\t";
    std::cout << border_color << "  " << end_color;
    for (int i = 0; i < width; i++)
    {
      if (i < 9)
      {
        std::cout << border_color << i + 1 << " " << end_color;
      }
      else
      {
        std::cout << border_color << i + 1 << end_color;
      }
    }

    std::cout << "\n";
    for (int y = 0; y < height; y++)
    {
      std::cout << "\t";
      if (y < 9)
      {
        std::cout << border_color <<  y + 1 << " " << end_color;
      }
      else
      {
        std::cout << border_color << y + 1 << end_color;
      }

      for (int x = 0; x < width; x++)
      {
        int idx = z * width * height * 5 + y * height * 5 + x * 5 + 3;
        if (tile_list[idx] == -1)
        {
          std::cout << "XX";
        }
        else
        {
          //std::cout << tile_list[idx] << " ";
          std::cout << "  ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n\n";
  }
  std::cout << "\n";
}

void print_single_pixel(const int x, const int y, const int z, std::string sprite)
{

  const int tab_spaces = 8;
  int x_bottom_offset = (tab_spaces / 2) + x;
  int y_bottom_offset = (height - y) + ((depth - z)) * 2 + (depth - z - 1) * (height + 2);

  std::cout << "\x1B[0G";

  for (int i = 0; i < y_bottom_offset + 1; i++)
  {
    std::cout << "\x1B[1A"; //moves the cursor to the top of the frame
  }

  for (int i = 0; i < x_bottom_offset + 1; i++)
  {
    std::cout << "\x1B[2C"; //moves the cursor to the top of the frame
  }

  std::cout << sprite << " ";

  for (int i = 0; i < y_bottom_offset + 1; i++)
  {
    std::cout << "\x1B[1B"; //moves the cursor to the top of the frame
  }

  for (int i = 0; i < x_bottom_offset + 2; i++)
  {
    std::cout << "\x1B[2D"; //moves the cursor to the top of the frame
  }
}

void uncover(const int x, const int y, const int z, const bool is_flagging)
{
  const int tile_idx = z * width * height * 5 + y * height * 5 + x * 5;

  if (tile_list[tile_idx + 3] > -1) //if the tile's not a bomb
  {
    tile_list[tile_idx + 4] = 1;
    if (tile_list[tile_idx + 3] == 0) //if it has 0 bombs nearby, used to start clearing 0s automatically
    {
      print_single_pixel(x, y, z, std::string("OO"));
      std::vector<int> tile_coords = {tile_list[tile_idx], tile_list[tile_idx + 1], tile_list[tile_idx + 2]};
      std::vector<int> idx_list = nearby_tile_idxes(tile_list, tile_coords, tile_idx);
      for (int i = 0; i < idx_list.size(); i++)
      {
        if (tile_list[idx_list[i] + 3] == 0)
        {
          uncover(tile_list[idx_list[i]], tile_list[idx_list[i] + 1], tile_list[idx_list[i] + 2], false);
        }
      }
    }
    else
    {
      print_single_pixel(x, y, z, std::to_string(tile_list[tile_idx + 3]));
    }
  }
  else
  {
    std::cout << "\n\nYOU LOSE";
  }
}

int main()
{
  srand(time(0));

  tile_list = fill_board();

  print_board();

  int rounds = 0;

  while (won() == false || rounds < 3)
  {
    int x;
    int y;
    int z;

    std::string question;
    std::vector<int> acceptable_answers;
    std::string input;
    int int_input;

    
    question = "Enter the layer: ";
    acceptable_answers = {1, 2, 3};
    std::cout << question;
    std::cin >> input;
    int_input = format_input(question, input, acceptable_answers, true);
    z = int_input - 1;

    question = "Enter the X: ";
    acceptable_answers = {1, 2, 3};
    std::cout << question;
    std::cin >> input;
    int_input = format_input(question, input, acceptable_answers, true);
    x = int_input - 1;

    question = "Enter the Y: ";
    acceptable_answers = {1, 2, 3};
    std::cout << question;
    std::cin >> input;
    int_input = format_input(question, input, acceptable_answers, true);
    y = int_input - 1;

    uncover(x, y, z, false);

    rounds++;
  }

  return 0;
}