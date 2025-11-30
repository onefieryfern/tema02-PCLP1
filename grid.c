// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "grid.h"
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

	if (ship.dir == vertical) {
		for (size_t i = ship.start.row; i >= ship.end.row; --i) {
			if (grid[i][ship.start.col] != 0) {
				return 0;
			}

			if (i == 0) {
				break;
			}
		}
	} else { // ship.dir == horizontal
		for (size_t i = ship.start.col; i <= ship.end.col; ++i) {
			if (grid[ship.start.row][i] != 0) {
				return 0;
			}
		}
	}

	return 1;
}

int add_ship_to_grid(int **grid, size_t rows, size_t cols, ship_t ship)
{
	if (!is_ship_placeable(grid, rows, cols, ship)) {
		return 0;
	}

	if (ship.dir == vertical) {
		for (size_t i = ship.start.row; i >= ship.end.row; --i) {
			grid[i][ship.start.col] = ship.len;

			if (i == 0) {
				break;
			}
		}
	} else { // ship.dir == horizontal
		for (size_t i = ship.start.col; i <= ship.end.col; ++i) {
			grid[ship.start.row][i] = ship.len;
		}
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
