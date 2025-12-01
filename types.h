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
	unsigned short len;
	direction_t dir;

	coords_t start;
	coords_t end;
} ship_t;

typedef struct {
	unsigned short space;
	ship_t *ship;
} grid_element_t;

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code);
int cast_to_direction_t(direction_t *direction, char direction_code);

int read_ship(ship_t *ship);
int calc_and_set_ship_end_coords(ship_t *ship);

coords_t get_grid_coords(coords_t coords);
coords_t get_output_coords(coords_t coords);

int are_coords_equal(coords_t a, coords_t b);

void get_ordered_ship_coords(coords_t coords_list[MAX_SHIP_LEN], ship_t ship);

#endif
