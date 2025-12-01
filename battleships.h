// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "types.h"
#include <stdlib.h>

int play_one_game(void);
int get_other_player(int player);

void read_and_place_ships(unsigned long long num_ships_per_player, size_t rows, size_t cols,
				grid_element_t **player1_grid, ship_t *player1_ships, unsigned long long *num_player1_ships_ptr,
				grid_element_t **player2_grid, ship_t *player2_ships, unsigned long long *num_player2_ships_ptr);

int try_hitting(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship);
int is_ship_destroyed(grid_element_t **grid, ship_t ship);
int destroy_ship(grid_element_t **grid, ship_t ship);

#endif
