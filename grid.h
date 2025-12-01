// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef GRID_H
#define GRID_H

#include "types.h"
#include <stdlib.h>

int is_ship_in_grid(size_t rows, size_t cols, ship_t ship);
int is_ship_placeable(Complex_grid **grid, size_t rows, size_t cols, ship_t ship);
int add_ship_to_grid(Complex_grid **grid, size_t rows, size_t cols, ship_t *ship);

void print_int_array(Complex_grid **arr, size_t rows, size_t cols);

Complex_grid **get_new_grid(Complex_grid **old_grid, size_t new_rows, size_t new_cols);
void get_new_grids(Complex_grid **old_player1_grid, Complex_grid **old_player2_grid,
				   Complex_grid ***new_player1_grid, Complex_grid ***new_player2_grid,
				   size_t new_rows, size_t new_cols);

#endif
