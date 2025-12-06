// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <stdlib.h>

grid_element_t **alloc_grid(size_t rows, size_t cols);
void free_grid(grid_element_t **grid);

void clear_grid(grid_element_t **grid, size_t rows, size_t cols);

char *dup_str(const char *str);

void free_player_data(player_data_t player_data);
void free_game_state(game_state_t game_state);

#endif
