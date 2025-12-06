// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "types.h"
#include "consts.h"
#include "memory.h"
#include <stdio.h>

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

short get_ship_len_from_type(char ship_type_code)
{
	switch (ship_type_code) {
	case 'A':
		return ALBACORE_LEN;
	case 'L':
		return LAFFEY_LEN;
	case 'B':
		return BELFAST_LEN;
	case 'Y':
		return YAMATO_LEN;
	case 'S':
		return SHINANO_LEN;
	}

	return 0;
}

char *get_ship_type_str(int len)
{
	char *ship_type_str = NULL;

	switch (len) {
	case ALBACORE_LEN:
		ship_type_str = dup_str("Albacore");
		break;
	case LAFFEY_LEN:
		ship_type_str = dup_str("Laffey");
		break;
	case BELFAST_LEN:
		ship_type_str = dup_str("Belfast");
		break;
	case YAMATO_LEN:
		ship_type_str = dup_str("Yamato");
		break;
	case SHINANO_LEN:
		ship_type_str = dup_str("Shinano");
		break;
	default:
		ship_type_str = dup_str("");
	}

	if (!ship_type_str) {
		fprintf(stderr, "dup_str() failed\n");
		return NULL;
	}

	return ship_type_str;
}

char *get_direction_str(char direction)
{
	char *direction_str = NULL;

	switch (direction) {
	case 'V':
		direction_str = dup_str("vertical");
		break;
	case 'H':
		direction_str = dup_str("orizontal");
		break;
	}

	if (!direction_str) {
		fprintf(stderr, "dup_str() failed\n");
		return NULL;
	}

	return direction_str;
}

/*
 * ship is incorrectly filled on exit with FAILURE
 * ship is filled correctly, except for `end_coords`, on exit with
 * ERR_SHIP_OUT_OF_BOUNDS
 */
int read_ship(ship_t *ship)
{
	// Read from stdin
	char ship_type_code;
	char direction_code;
	coords_t start_coords;

	scanf(" %c", &ship_type_code);
	scanf(" %c", &direction_code);
	scanf("%zu %zu", &start_coords.row, &start_coords.col);

	// Set ship length (type)
	ship->len = get_ship_len_from_type(ship_type_code);

	// Set ship direction
	ship->dir = direction_code;

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
	if (ship->dir == 'V') {
		coords_t grid_start = { 0, 0 };
		size_t max_vertical_len = (ship->start.row - grid_start.row) + 1;

		if ((unsigned short)ship->len > max_vertical_len) {
			return ERR_SHIP_OUT_OF_BOUNDS;
		}

		// Vertical ships' start is at their bottom,
		// thus their end is at their top
		ship->end.row = ship->start.row - (ship->len - 1);

		ship->end.col = ship->start.col;
	} else { // ship->dir == 'H'
		ship->end.row = ship->start.row;

		// Horizontal ships' start is at their left,
		// thus their end is at their right
		ship->end.col = ship->start.col + (ship->len - 1);
	}

	return SUCCESS;
}

void get_ordered_ship_coords(coords_t coords_list[MAX_SHIP_LEN], ship_t ship)
{
	size_t list_len = 0;

	if (ship.dir == 'V') {
		for (size_t i = ship.start.row; i >= ship.end.row; --i) {
			coords_list[list_len].row = i;
			coords_list[list_len].col = ship.start.col;

			list_len++;

			if (i == 0) {
				break;
			}
		}
	} else { // ship.dir == 'H'
		for (size_t i = ship.start.col; i <= ship.end.col; ++i) {
			coords_list[list_len].row = ship.start.row;
			coords_list[list_len].col = i;

			list_len++;
		}
	}
}
