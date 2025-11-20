// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef LOGGING_H
#define LOGGING_H

#include "types.h"

void log_hit_successful(int player_num, int ship_type, coords_t coords);

char *get_ship_type_str(int ship_type);

#endif
