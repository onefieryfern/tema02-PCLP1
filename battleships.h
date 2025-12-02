// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "types.h"
#include <stdlib.h>

int play_one_game(void);
int get_other_player(int player);

int handle_input_ships(game_state_t *game_state);

int try_hitting(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship);
int is_ship_destroyed(grid_element_t **grid, ship_t ship);
int destroy_ship(grid_element_t **grid, ship_t ship);

#endif
