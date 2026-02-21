#include "3dMS.hpp"

const int bomb_count = 20; //percentage of total tiles that are bombs

bool search_list(const std::vector<int> list, const std::vector<int> item)
{
  int item_length = item.size();
  int matching_nums = 0;

  for (int i = 0; i < list.size() - item_length; i += item_length)
  {
    for (int j = 0; j < item_length; j++)
    {
      matching_nums = 0;
      if (list[i + j] == item[i + j])
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
{
  int tile_x = tile_coords[0];
  int tile_y = tile_coords[1];
  int tile_z = tile_coords[2];

  int bomb_count = 0;

  int x_start;
  int y_start;
  int z_start;

  int x_end;
  int y_end;
  int z_end;

  if (tile_x == 0) {x_start = 0;}
  if (tile_x == width - 1) {x_end == tile_x;}

  if (tile_y == 0) {y_start = 0;}
  if (tile_y == width - 1) {y_end == tile_y;}

  if (tile_z == 0) {z_start = 0;}
  if (tile_z == width - 1) {z_end == tile_z;}

  for (; z_start < z_end; z_start++)
  {
    for (; x_start < x_end; x_start++)
    {
      for (; y_start < y_end; y_start++)
      {
        int idx = tile_idx + 3;

        idx += z_start * width * height * 4; //z (depth) translation
        idx += y_start * height * 4; //y translation
        idx += x_start * 4; //x translation

        if (tile_list[idx] == -1)
        {
          bomb_count++;
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
  const int bomb_count = std::floor((width * height* depth) * (bomb_count / 100)); //number of bombs

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
  for (int i = 0; i < bomb_count; i++);
  {
    int rand_idx = rand() % available_tiles.size() / 4;

    bomb_coords.push_back(available_tiles[rand_idx]);
    bomb_coords.push_back(available_tiles[rand_idx + 1]);
    bomb_coords.push_back(available_tiles[rand_idx + 2]);

    available_tiles.erase(available_tiles.begin() + rand_idx, available_tiles.begin() + rand_idx + 2);
  }

  std::vector<int> return_tiles = {};
  for (int x = 0; x < width; x++)
  {
    for (int y = 0; y < height; y++)
    {
      for (int z = 0; z < depth; z++)
      {
        return_tiles.push_back(x);
        return_tiles.push_back(y);
        return_tiles.push_back(z);
        if (search_list(bomb_coords, std::vector<int> {x, y, z}))
        {
          return_tiles.push_back(-1);
        }
        else
        {
          return_tiles.push_back(0);
        }
      }
    }
  }
  
  for (int i = 0; i < return_tiles.size(); i += 4)
  {
    if (return_tiles[i + 3] != -1)
    {
      return_tiles[i + 3] = surrounding_bombs(return_tiles, std::vector<int> {return_tiles.begin() + i, return_tiles.begin() + i + 3}, i);
    }
  }

  return return_tiles;
}

int main()
{
  srand(time(0));

  return 0;
}