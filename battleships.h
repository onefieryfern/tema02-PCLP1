// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "types.h"
#include <stdlib.h>

int play_one_game(void);
int get_other_player(int player);

int try_hitting(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship);
int find_hit_ship_index(ship_t *ships, size_t num_ships, coords_t hit_coords, size_t *index);
int is_ship_destroyed(grid_element_t **grid, ship_t ship);
void remove_ship_from_list(ship_t *ships, size_t *num_ships, size_t index);
int destroy_ship(grid_element_t **grid, ship_t ship);

#endif
