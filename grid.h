// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef GRID_H
#define GRID_H

#include "types.h"
#include <stdlib.h>

int is_ship_in_grid(size_t rows, size_t cols, ship_t ship);
int is_ship_placeable(grid_element_t **grid, size_t rows, size_t cols, ship_t ship);
int add_ship_to_grid(grid_element_t **grid, size_t rows, size_t cols, ship_t *ship);

void print_grid(grid_element_t **grid, size_t rows, size_t cols);

grid_element_t **get_new_grid(grid_element_t **old_grid, size_t new_rows, size_t new_cols);
void get_new_grids(grid_element_t **old_player1_grid, grid_element_t **old_player2_grid,
				   grid_element_t ***new_player1_grid, grid_element_t ***new_player2_grid,
				   size_t new_rows, size_t new_cols);

#endif
