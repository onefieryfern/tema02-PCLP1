// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "logging.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void log_hit_successful(int player_num, ship_type_t ship_type, coords_t coords)
{
	char *ship_type_str = get_ship_type_str(ship_type);

	if (!ship_type_str) {
		fprintf(stderr, "get_ship_type_str() failed\n");
		return;
	}

	printf("Jucatorul %d a lovit o nava %s la coordonatele (%llu, %llu)\n\n",
		   player_num, ship_type_str, coords.row, coords.col);

	// Cleanup
	free(ship_type_str);
}

void log_hit_miss(int player_num, coords_t coords)
{
	printf("Jucatorul %d a ratat o lovitura la coordonatele (%llu, %llu).\n",
		   player_num, coords.row, coords.col);
}

void log_already_hit(coords_t coords, int player_num)
{
	printf("Coordonatele (%llu, %llu) au fost deja atacate de jucatorul %d\n",
		   coords.row, coords.col, player_num);
}

void log_destroy(int player_num, ship_type_t ship_type,
				 coords_t start, coords_t end)
{
	char *ship_type_str = get_ship_type_str(ship_type);

	if (!ship_type_str) {
		fprintf(stderr, "get_ship_type_str() failed\n");
		return;
	}

	printf("Jucatorul %d a distrus o nava %s plasata intre coordonatele "
		   "(%llu, %llu) si (%llu, %llu).\n", player_num, ship_type_str,
		   start.row, start.col, end.row, end.col);

	// Cleanup
	free(ship_type_str);
}

void log_cannot_place(ship_type_t ship_type, direction_t direction,
					  coords_t coords)
{
	char *ship_type_str = get_ship_type_str(ship_type);

	if (!ship_type_str) {
		fprintf(stderr, "get_ship_type_str() failed\n");
		return;
	}

	char *direction_str = NULL;

	switch (direction) {
	case vertical:
		direction_str = dup_str("vertical");
		break;
	case horizontal:
		direction_str = dup_str("orizontal");
		break;
	}

	if (!direction_str) {
		fprintf(stderr, "dup_str() failed\n");
		return;
	}

	printf("Nava %s nu poate fi amplasata %s la coordonatele (%llu, %llu).\n",
		   ship_type_str, direction_str, coords.row, coords.col);

	// Cleanup
	free(ship_type_str);
	free(direction_str);
}

void log_player_won(int player_num)
{
	printf("Jucatorul %d a castigat.\n", player_num);
}

char *get_ship_type_str(ship_type_t ship_type)
{
	char *ship_type_str = NULL;

	switch (ship_type) {
	case albacore:
		ship_type_str = dup_str("Albacore");
		break;
	case laffey:
		ship_type_str = dup_str("Laffey");
		break;
	case belfast:
		ship_type_str = dup_str("Belfast");
		break;
	case yamato:
		ship_type_str = dup_str("Yamato");
		break;
	case shinano:
		ship_type_str = dup_str("Shinano");
		break;
	}

	if (!ship_type_str) {
		fprintf(stderr, "dup_str() failed\n");
		return NULL;
	}

	return ship_type_str;
}
