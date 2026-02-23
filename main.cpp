#include "3dMS.hpp"

const float bomb_percent = 0.2; //percentage of total tiles that are bombs

//dimensions of the board
const int width =  3;
const int height = 3;
const int depth =  3;

std::vector<int> tile_list = {}; //list of all the tiles

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

int surrounding_bombs(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx)
//counts the number of bombs surroudning a selected tile
{
  //the coords of the tile selected
  int tile_x = tile_coords[0];
  int tile_y = tile_coords[1];
  int tile_z = tile_coords[2];

  std::cout << tile_x << ", " << tile_y << ", " << tile_z << "," << tile_idx << "\n";

  //counter of the bombs detected
  int bomb_count = 0;
  
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
  if (tile_x == width - 1) {x_end == 0;} //if the tile is on the right wall, dont search past the right wall

  if (tile_y == 0) {y_start_benchmark = 0;}
  if (tile_y == height - 1) {y_end == 0;}

  if (tile_z == 0) {z_start_benchmark = 0;}
  if (tile_z == depth - 1) {z_end == 0;}

  std::cout << "\t" << x_start_benchmark << ", " << x_end << " : " << y_start_benchmark << ", " << y_end << " : " << z_start_benchmark << ", " << z_end << " \n"; 

  //searches each tile in 3x3x3 radius, cutting off out-of-bounds tiles using start and end variables
  for (z_start = z_start_benchmark; z_start <= z_end; z_start++)
  {
    std::cout <<  "\tZ START: " << z_start << "\n";
    for (x_start = x_start_benchmark; x_start <= x_end; x_start++)
    {
      std::cout << "\t X START: " << x_start << "\n";
      for (y_start = y_start_benchmark; y_start <= y_end; y_start++)
      {
        std::cout << "\t  Y START: " << y_start << "\n";
        if (x_start != 0 || y_start != 0 || z_start != 0) //prevents the check from checking its own tile, not neccisary
        {
          std::cout <<  "\t" << "      TRANSLATIONS: " << x_start << ", " << y_start << ", " << z_start << "\n";
          //idx is the idx of what the tile is (either bomb which is -1 or the number of bombs nearby (the number that is dicolonistsplayed when uncovered))
          int idx = tile_idx + 3;

          //moves the index to the info of the tile being searched right now
          idx += z_start * width * height * 5; //z (depth) translation
          idx += y_start * height * 5; //y translation
          idx += x_start * 5; //x translation

          std::cout << "\t" << "      IDX: " << idx << "\n";

          //if the tile is a bomb add it to the bomb counter
          if (tile_list[idx] == -1)
          {
            std::cout << "\t" << "      IS BOMB\n";
            bomb_count++;
          }
        }
      }
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

  for (int i = 0; i < return_tiles.size(); i += 5)
  {
    std::cout << "[" << return_tiles[i] << ", " << return_tiles[i + 1] << ", " << return_tiles[i + 2] << ", " << return_tiles[i + 3] << ", " << return_tiles[i + 4] << "]" << i/4 << "\n";
  }

  return return_tiles;
}

void print_board()
{
  for (int z = 0; z < depth; z++)
  {
    for (int y = 0; y < height; y++)
    {
      for (int x = 0; x < width; x++)
      {
        int idx = z * width * height * 5 + y * height * 5 + x * 5 + 3;
        if (tile_list[idx] == -1)
        {
          std::cout << "XX";
        }
        else
        {
          std::cout << tile_list[idx] << " ";
        }
      }
      std::cout << "\n";
    }
    std::cout << "\n\n";
  }
}

int main()
{
  srand(time(0));

  tile_list = fill_board();

  print_board();

  return 0;
}