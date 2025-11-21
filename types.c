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
