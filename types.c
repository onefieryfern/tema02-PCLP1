// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "types.h"
#include <stdio.h>
#include <stdlib.h>

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code)
{
	switch (ship_type_code) {
	case 'A':
		*ship_type = albacore;
		return 1;
	case 'L':
		*ship_type = laffey;
		return 1;
	case 'B':
		*ship_type = belfast;
		return 1;
	case 'Y':
		*ship_type = yamato;
		return 1;
	case 'S':
		*ship_type = shinano;
		return 1;
	default:
		fprintf(stderr, "Unknown ship type %c.\n", ship_type_code);
	}

	return 0;
}

int cast_to_direction_t(direction_t *direction, char direction_code)
{
	switch (direction_code) {
	case 'H':
		*direction = horizontal;
		return 1;
	case 'V':
		*direction = vertical;
		return 1;
	default:
		fprintf(stderr, "Unknown direction %c.\n", direction_code);
	}

	return 0;
}

int read_ship(ship_t *ship)
{
	char ship_type_code;
	char direction_code;
	coords_t start_coords;

	// Read from stdin
	/*
	scanf(" %c %c %llu %llu", &ship_type_code, &direction_code,
		  &start_coords.row, &start_coords.col);
	*/

	scanf(" %c", &ship_type_code);
	scanf(" %c", &direction_code);
	scanf("%llu %llu", &start_coords.row, &start_coords.col);

	// Set ship len
	ship_type_t ship_type;
	if (!cast_to_ship_type_t(&ship_type, ship_type_code)) {
		return 0;
	}

	ship->len = (unsigned short)ship_type;

	// Set ship direction
	direction_t direction;
	if (!cast_to_direction_t(&direction, direction_code)) {
		return 0;
	}

	ship->dir = direction;

	// The grid is indexed from (0, 0), but the read coordinates
	// are indexed from (1, 1)
	start_coords.row--;
	start_coords.col--;

	ship->start = start_coords;

	// Set end coordinates
	if (calc_and_set_ship_end_coords(ship) == -1) {
		return -1;
	}

	return 1;
}

int calc_and_set_ship_end_coords(ship_t *ship)
{
	if (ship->dir == vertical) {
		size_t max_vertical_len = ship->start.row + 1;

		if (ship->len > max_vertical_len) {
			return -1;
		}

		/*
		// TODO: Prettify this comparison
		if (ship->start.col < (unsigned short)(ship->len - 1)) {
			return -1;
		}
		*/

		// Vertical ships' start is at their bottom,
		// thus their end is at their top
		ship->end.row = ship->start.row - (ship->len - 1);
		ship->end.col = ship->start.col;
	} else { // ship->dir == horizontal
		// Horizontal ships' start is at their left,
		// thus their end is at their right
		ship->end.row = ship->start.row;
		ship->end.col = ship->start.col + (ship->len - 1);
	}

	return 1;
}

coords_t get_matrix_coords(coords_t coords)
{
	coords.row--;
	coords.col--;

	return coords;
}

coords_t get_grid_coords(coords_t coords)
{
	coords.row++;
	coords.col++;

	return coords;
}

int are_coords_equal(coords_t a, coords_t b)
{
	return a.row == b.row && a.col == b.col;
}

coords_t *get_ordered_ship_coords(ship_t ship)
{
	coords_t *coords_list = malloc(ship.len * sizeof(coords_t));

	if (!coords_list) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}

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

	return coords_list;
}
