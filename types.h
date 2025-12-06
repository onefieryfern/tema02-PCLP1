// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef TYPES_H
#define TYPES_H

#include "consts.h"
#include <stdlib.h>

typedef struct {
	size_t row;
	size_t col;
} coords_t;

typedef enum {
	vertical,
	horizontal
} direction_t;

typedef enum {
	albacore = 1,
	laffey = 2,
	belfast = 3,
	yamato = 4,
	shinano = 5
} ship_type_t;

typedef struct {
	short len;
	direction_t dir;

	coords_t start;
	coords_t end;
} ship_t;

typedef struct {
	short space;
	ship_t *ship;
} grid_element_t;

typedef struct {
	grid_element_t **grid;
	ship_t *ships;
	unsigned long long num_ships;
} player_data_t;

typedef struct {
	unsigned long long rows;
	unsigned long long cols;

	player_data_t player1;
	player_data_t player2;
} game_state_t;

typedef struct {
	unsigned long long num_successful_hits;
	unsigned long long num_hits;

	unsigned long long num_ships_per_grid;
} stats_t;

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code);
int cast_to_direction_t(direction_t *direction, char direction_code);

int read_ship(ship_t *ship);
int calc_and_set_ship_end_coords(ship_t *ship);

coords_t get_grid_coords(coords_t coords);
coords_t get_output_coords(coords_t coords);

int are_coords_equal(coords_t a, coords_t b);

void get_ordered_ship_coords(coords_t coords_list[MAX_SHIP_LEN], ship_t ship);

char *get_ship_type_str(ship_type_t ship_type);
char *get_direction_str(direction_t direction);

void free_player_data(player_data_t player_data);
void free_game_state(game_state_t game_state);

#endif
