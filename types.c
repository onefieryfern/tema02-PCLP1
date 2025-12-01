// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "types.h"
#include "consts.h"
#include <stdio.h>

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code)
{
	switch (ship_type_code) {
	case 'A':
		*ship_type = albacore;
		return SUCCESS;
	case 'L':
		*ship_type = laffey;
		return SUCCESS;
	case 'B':
		*ship_type = belfast;
		return SUCCESS;
	case 'Y':
		*ship_type = yamato;
		return SUCCESS;
	case 'S':
		*ship_type = shinano;
		return SUCCESS;
	default:
		fprintf(stderr, "Unknown ship type %c.\n", ship_type_code);
	}

	return FAILURE;
}

int cast_to_direction_t(direction_t *direction, char direction_code)
{
	switch (direction_code) {
	case 'H':
		*direction = horizontal;
		return SUCCESS;
	case 'V':
		*direction = vertical;
		return SUCCESS;
	default:
		fprintf(stderr, "Unknown direction %c.\n", direction_code);
	}

	return FAILURE;
}

/*
 * Temporary comment
 *
 * ship is filled with garbage data on exit with FAILURE
 * ship is filled correctly, except for `end_coords`, on exit with ERR_SHIP_OUT_OF_BOUNDS
 */
int read_ship(ship_t *ship)
{
	// Read from stdin
	char ship_type_code;
	char direction_code;
	coords_t start_coords;

	scanf(" %c", &ship_type_code);
	scanf(" %c", &direction_code);
	scanf("%llu %llu", &start_coords.row, &start_coords.col);

	// Set ship length (type)
	ship_type_t ship_type;
	if (cast_to_ship_type_t(&ship_type, ship_type_code) == FAILURE) {
		return FAILURE;
	}

	ship->len = (unsigned short)ship_type;

	// Set ship direction
	direction_t direction;
	if (cast_to_direction_t(&direction, direction_code) == FAILURE) {
		return FAILURE;
	}

	ship->dir = direction;

	// The grid starts at (0, 0), but the read coordinates
	// are indexed from (1, 1)
	ship->start = get_grid_coords(start_coords);

	// Set end coordinates
	if (calc_and_set_ship_end_coords(ship) == ERR_SHIP_OUT_OF_BOUNDS) {
		return ERR_SHIP_OUT_OF_BOUNDS;
	}

	return 1;
}

int calc_and_set_ship_end_coords(ship_t *ship)
{
	if (ship->dir == vertical) {
		coords_t grid_start = { 0, 0 };
		size_t max_vertical_len = (ship->start.row - grid_start.row) + 1;

		if (ship->len > max_vertical_len) {
			return ERR_SHIP_OUT_OF_BOUNDS;
		}

		// Vertical ships' start is at their bottom,
		// thus their end is at their top
		ship->end.row = ship->start.row - (ship->len - 1);

		ship->end.col = ship->start.col;
	} else { // ship->dir == horizontal
		ship->end.row = ship->start.row;

		// Horizontal ships' start is at their left,
		// thus their end is at their right
		ship->end.col = ship->start.col + (ship->len - 1);
	}

	return SUCCESS;
}

coords_t get_grid_coords(coords_t coords)
{
	coords.row--;
	coords.col--;

	return coords;
}

coords_t get_output_coords(coords_t coords)
{
	coords.row++;
	coords.col++;

	return coords;
}

int are_coords_equal(coords_t a, coords_t b)
{
	return a.row == b.row && a.col == b.col;
}

void get_ordered_ship_coords(coords_t coords_list[MAX_SHIP_LEN], ship_t ship)
{
	size_t list_len = 0;

	if (ship.dir == vertical) {
		for (size_t i = ship.start.row; i >= ship.end.row; --i) {
			coords_list[list_len].row = i;
			coords_list[list_len].col = ship.start.col;

			list_len++;

			if (i == 0) {
				break;
			}
		}
	} else { // ship.dir == horizontal
		for (size_t i = ship.start.col; i <= ship.end.col; ++i) {
			coords_list[list_len].row = ship.start.row;
			coords_list[list_len].col = i;

			list_len++;
		}
	}
}
