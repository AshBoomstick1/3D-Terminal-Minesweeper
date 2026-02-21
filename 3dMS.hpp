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

extern std::vector<int> tile_list;

//function declarations
bool search_list(const std::vector<int> list, const std::vector<int> item);
int surrounding_bombs(const std::vector<int> tile_list, const std::vector<int> tile_coords, const int tile_idx);

#endif