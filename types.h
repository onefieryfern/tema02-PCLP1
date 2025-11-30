// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef TYPES_H
#define TYPES_H

typedef struct {
	unsigned long long x;
	unsigned long long y;
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
	ship_type_t type;
	coords_t start_coords;
	coords_t end_coords;
} ship_t;

int cast_to_ship_type_t(ship_type_t *ship_type, char ship_type_code);
int cast_to_direction_t(direction_t *direction, char direction_code);

int read_ship(ship_t *ship);
int get_ship_end_coords(coords_t *end_coords, coords_t start_coords,
						direction_t direction, ship_type_t ship_type);
coords_t get_matrix_coords(coords_t coords);
coords_t get_grid_coords(coords_t coords);

#endif
