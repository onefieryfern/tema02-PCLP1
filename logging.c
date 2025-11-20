// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "logging.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void log_hit_successful(int player_num, int ship_type, coords_t coords)
{
	char *ship_type_str = get_ship_type_str(ship_type);

	if (!ship_type_str) {
		fprintf(stderr, "get_ship_type_str() failed\n");
		return;
	}

	printf("Jucatorul %d a lovit o nava %s la coordonatele (%llu, %llu)\n",
		   player_num, ship_type_str, coords.x, coords.y);

	// Cleanup
	free(ship_type_str);
}

char *get_ship_type_str(int ship_type)
{
	char *ship_type_str = NULL;

	switch (ship_type) {
	case 1:
		ship_type_str = dup_str("Albacore");
		break;
	case 2:
		ship_type_str = dup_str("Laffey");
		break;
	case 3:
		ship_type_str = dup_str("Belfast");
		break;
	case 4:
		ship_type_str = dup_str("Yamato");
		break;
	case 5:
		ship_type_str = dup_str("Shinano");
		break;
	default:
		break;
	}

	return ship_type_str;
}
