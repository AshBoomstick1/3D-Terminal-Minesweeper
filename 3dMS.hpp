#ifndef MS_H
#define MS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cmath>

//variable declarations
extern const int width;
extern const int height;
extern const int depth;


/*
TILE LIST EXPLANATION
  tile list is a flattened 3d array with each flattened array storing:
    -the corrdinates of the tiles (x, y, z), 
    -if the tile is a bomb or the number of surroding bombs, 
    -and then if the tile is covered (0), uncovered (1), or flagged (2)

  eg: 
    [0, 0, 0, -1, 0] is a sub array that represents one tile
    [x, y, z, is_bomb/bomb_count, covered/uncovered/flagged]
*/
extern std::vector<int> tile_list;


//function declarations
bool search_list(const std::vector<int> list, const std::vector<int> item);
int surrounding_bombs(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx);
std::vector<int> nearby_tile_idxes(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx);
int format_input(const std::string question, std::string input, const std::vector<int> acceptable_inputs, const bool first_test);
char format_input(const std::string question, std::string input, const std::vector<char> acceptable_inputs, const bool first_test);
bool won();

void print_board();
void print_single_pixel(const int x, const int y, const int z, std::string sprite);

#endif