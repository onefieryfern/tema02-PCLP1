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

int is_ship_placeable(int **grid, size_t rows, size_t cols, ship_t ship)
{
	if (!is_ship_in_grid(rows, cols, ship)) {
		return 0;
	}

	coords_t *ship_coords = get_ordered_ship_coords(ship);

	if (!ship_coords) {
		fprintf(stderr, "get_ordered_ship_coords() failed\n");
		return -1;
	}

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		if (grid[current_coords.row][current_coords.col] != 0) {
			return 0;
		}
	}

	return 1;
}

int add_ship_to_grid(int **grid, size_t rows, size_t cols, ship_t ship)
{
	if (!is_ship_placeable(grid, rows, cols, ship)) {
		return 0;
	}

	coords_t *ship_coords = get_ordered_ship_coords(ship);

	if (!ship_coords) {
		fprintf(stderr, "get_ordered_ship_coords() failed\n");
		return -1;
	}

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		grid[current_coords.row][current_coords.col] = ship.len;
	}

	return 1;
}

void print_int_array(int **arr, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			printf("%d", arr[i][j]);

			// Print spaces after each element, but not after the last one
			if (j < cols - 1) {
				printf(" ");
			}
		}

		// Print newlines between rows (and after the last row)
		printf("\n");
	}
}

int **get_new_grid(int **old_grid, size_t new_rows, size_t new_cols)
{
	free_int_matrix(old_grid);
	old_grid = NULL;

	int **new_grid = alloc_int_matrix(new_rows, new_cols);

	if (!new_grid) {
		fprintf(stderr, "alloc_int_matrix() failed\n");

		return NULL;
	}

	zero_out_matrix(new_grid, new_rows, new_cols);

	return new_grid;
}

void get_new_grids(int **old_player1_grid, int **old_player2_grid,
				   int ***new_player1_grid, int ***new_player2_grid,
				   size_t new_rows, size_t new_cols)
{
	*new_player1_grid = get_new_grid(old_player1_grid, new_rows, new_cols);
	*new_player2_grid = get_new_grid(old_player2_grid, new_rows, new_cols);

	if (!*new_player1_grid || !*new_player2_grid) {
		fprintf(stderr, "alloc_int_matrix() failed\n");

		free_int_matrix(*new_player1_grid);
		free_int_matrix(*new_player2_grid);

		*new_player1_grid = NULL;
		*new_player2_grid = NULL;
	}
}
