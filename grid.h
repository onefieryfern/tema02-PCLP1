// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef GRID_H
#define GRID_H

#include "types.h"
#include <stdlib.h>

int is_ship_in_grid(size_t rows, size_t cols, ship_t ship);
int is_ship_placeable(int **grid, size_t rows, size_t cols, ship_t ship);
int add_ship_to_grid(int **grid, size_t rows, size_t cols, ship_t ship);

void print_int_array(int **arr, size_t rows, size_t cols);

void get_new_grids(int **old_player1_grid, int **old_player2_grid,
				   int ***new_player1_grid, int ***new_player2_grid,
				   size_t new_rows, size_t new_cols);

#endif
