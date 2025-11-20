// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef LOGGING_H
#define LOGGING_H

#include "types.h"

void log_hit_successful(int player_num, int ship_type, coords_t coords);
void log_hit_miss(int player_num, coords_t coords);
void log_already_hit(coords_t coords, int player_num);
void log_destroy(int player_num, int ship_type, coords_t start, coords_t end);

void log_cannot_place(int ship_type, direction_t direction, coords_t coords);
void log_player_won(int player_num);

char *get_ship_type_str(int ship_type);

#endif
