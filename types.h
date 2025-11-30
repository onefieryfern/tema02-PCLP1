// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef TYPES_H
#define TYPES_H

typedef struct {
	unsigned long long row;
	unsigned long long col;
} coords_t;

typedef enum { vertical, horizontal } direction_t;

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

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code);
int cast_to_direction_t(direction_t *direction, char direction_code);

int read_ship(ship_t *ship);
int calc_and_set_ship_end_coords(ship_t *ship);

coords_t get_matrix_coords(coords_t coords);
coords_t get_grid_coords(coords_t coords);

int are_coords_equal(coords_t a, coords_t b);

coords_t *get_ordered_ship_coords(ship_t ship);

#endif
