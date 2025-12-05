// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include "types.h"
#include <stdlib.h>

int read_starting_grid(game_state_t *game_state);

int read_starting_ships(game_state_t *game_state);
void play_one_game(game_state_t *game_state, stats_t *player1_stats,
				   stats_t *player2_stats);

int hit_space(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship);

#endif
