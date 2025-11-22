// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "types.h"
#include <stdio.h>

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
	scanf("%c %c %llu %llu", &ship_type_code, &direction_code,
		  &start_coords.x, &start_coords.y);

	// The grid is indexed from (0, 0), but the read coordinates
	// are indexed from (1, 1)
	start_coords.x--;
	start_coords.y--;

	ship->start_coords = start_coords;

	// Set ship type
	ship_type_t ship_type;
	if (!cast_to_ship_type_t(&ship_type, ship_type_code)) {
		return 0;
	}

	ship->type = ship_type;

	// Calculate end coordinates
	direction_t direction;
	if (!cast_to_direction_t(&direction, direction_code)) {
		return 0;
	}

	coords_t end_coords;
	if (get_ship_end_coords(&end_coords, start_coords, direction, ship_type)
		== -1) {
		return -1;
	}

	ship->end_coords = end_coords;

	return 1;
}

int get_ship_end_coords(coords_t *end_coords, coords_t start_coords,
						direction_t direction, ship_type_t ship_type)
{
	unsigned short ship_len = (unsigned short)ship_type;

	if (direction == vertical) {
		// TODO: Prettify this comparison
		if (start_coords.y < (unsigned short)(ship_len - 1)) {
			return -1;
		}

		// Vertical ships' start is at their bottom,
		// thus their end is at their top
		end_coords->y = start_coords.y - (ship_len - 1);
	} else {
		// Horizontal ships' start is at their left,
		// thus their end is at their right
		end_coords->x = start_coords.x + (ship_len - 1);
	}

	return 1;
}
