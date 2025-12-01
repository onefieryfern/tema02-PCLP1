// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef LOGGING_H
#define LOGGING_H

#include "types.h"

void log_hit_successful(int player_num, ship_type_t ship_type, coords_t hit_coords);
void log_hit_miss(int player_num, coords_t hit_coords);
void log_already_hit(int player_num, coords_t hit_coords);
void log_destroy(int player_num, ship_t hit_ship);

void log_cannot_place(ship_t ship);
void log_player_won(int player_num);

char *get_ship_type_str(ship_type_t ship_type);

#endif
