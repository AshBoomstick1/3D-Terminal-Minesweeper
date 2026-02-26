#include "3dMS.hpp"

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

  std::cout << "\x1B[0G";
}