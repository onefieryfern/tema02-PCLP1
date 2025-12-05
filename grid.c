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

int is_ship_placeable(grid_element_t **grid, size_t rows, size_t cols,
					  ship_t ship)
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

void add_ship_to_grid(grid_element_t **grid, ship_t *ship)
{
	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, *ship);

	for (size_t i = 0; i < ship->len; ++i) {
		coords_t current_coords = ship_coords[i];

		grid[current_coords.row][current_coords.col].space = (short)ship->len;
		grid[current_coords.row][current_coords.col].ship = ship;
	}
}

void remove_ship_from_grid(grid_element_t **grid, ship_t ship)
{
	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, ship);

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		grid[current_coords.row][current_coords.col].ship = NULL;

		if (grid[current_coords.row][current_coords.col].space != SPACE_HIT) {
			grid[current_coords.row][current_coords.col].space = SPACE_EMPTY;
		}
	}
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
