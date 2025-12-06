// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef TYPES_H
#define TYPES_H

#include <stdlib.h>

// Ship properties
#define NUM_SHIP_TYPES 5
#define MAX_SHIP_LEN 5

// Ship lengths
#define ALBACORE_LEN 1
#define LAFFEY_LEN 2
#define BELFAST_LEN 3
#define YAMATO_LEN 4
#define SHINANO_LEN 5

// Function exit codes
#define ERR_SHIP_OUT_OF_BOUNDS -1

typedef struct {
	size_t row;
	size_t col;
} coords_t;

typedef struct {
	short len;
	char dir;

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

coords_t get_grid_coords(coords_t coords);
coords_t get_output_coords(coords_t coords);
int are_coords_equal(coords_t a, coords_t b);

short get_ship_len_from_type(char ship_type_code);
char *get_ship_type_str(int len);

char *get_direction_str(char direction);

int read_ship(ship_t *ship);
int calc_and_set_ship_end_coords(ship_t *ship);

void get_ordered_ship_coords(coords_t coords_list[MAX_SHIP_LEN], ship_t ship);

#endif
