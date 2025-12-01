// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "grid.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>

int is_ship_in_grid(size_t rows, size_t cols, ship_t ship)
{
	coords_t grid_end = { rows - 1, cols - 1 };

	if (ship.start.row > grid_end.row || ship.end.row > grid_end.row) {
		return 0;
	}

	if (ship.start.col > grid_end.col || ship.end.col > grid_end.col) {
		return 0;
	}

	return 1;
}

int is_ship_placeable(grid_element_t **grid, size_t rows, size_t cols, ship_t ship)
{
	if (!is_ship_in_grid(rows, cols, ship)) {
		return 0;
	}

	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, ship);

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		if (grid[current_coords.row][current_coords.col].space != 0) {
			return 0;
		}
	}

	return 1;
}

int add_ship_to_grid(grid_element_t **grid, size_t rows, size_t cols, ship_t *ship)
{
	if (!is_ship_placeable(grid, rows, cols, *ship)) {
		return 0;
	}

	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, *ship);

	for (size_t i = 0; i < ship->len; ++i) {
		coords_t current_coords = ship_coords[i];

		grid[current_coords.row][current_coords.col].space = ship->len;
		grid[current_coords.row][current_coords.col].ship = ship;
	}

	return 1;
}

void print_grid(grid_element_t **grid, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			printf("%d", grid[i][j].space);

			// Print spaces after each element, but not after the last one
			if (j < cols - 1) {
				printf(" ");
			}
		}

		// Print newlines between rows (and after the last row)
		printf("\n");
	}
}

grid_element_t **get_new_grid(grid_element_t **old_grid, size_t new_rows, size_t new_cols)
{
	free_grid(old_grid);
	old_grid = NULL;

	grid_element_t **new_grid = alloc_grid(new_rows, new_cols);

	if (!new_grid) {
		fprintf(stderr, "alloc_grid() failed\n");

		return NULL;
	}

	clear_grid(new_grid, new_rows, new_cols);

	return new_grid;
}

void get_new_grids(grid_element_t **old_player1_grid, grid_element_t **old_player2_grid,
				   grid_element_t ***new_player1_grid, grid_element_t ***new_player2_grid,
				   size_t new_rows, size_t new_cols)
{
	*new_player1_grid = get_new_grid(old_player1_grid, new_rows, new_cols);
	*new_player2_grid = get_new_grid(old_player2_grid, new_rows, new_cols);

	if (!*new_player1_grid || !*new_player2_grid) {
		fprintf(stderr, "get_new_grid() failed\n");

		free_grid(*new_player1_grid);
		free_grid(*new_player2_grid);

		*new_player1_grid = NULL;
		*new_player2_grid = NULL;
	}
}

unsigned long long get_num_ships_for_size(size_t rows, size_t cols)
{
	unsigned long long num_ships = 0;

	// Albacore
	num_ships += (rows * cols) / 20;

	// Laffey
	num_ships += (rows * cols) / 30;

	// Belfast
	num_ships += (rows * cols) / 40;

	// Yamato
	num_ships += (rows * cols) / 55;

	// Shinano
	num_ships += (rows * cols) / 70;

	return num_ships;
}
