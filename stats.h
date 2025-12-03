// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef STATS_H
#define STATS_H

#include "types.h"

void handle_stats(unsigned long long count, stats_t *player1_stats, stats_t *player2_stats);
void handle_stat(unsigned long long count, stats_t *stats, char num_games, unsigned short game_index, double acc, char *min_max);
double calc_game_acc(stats_t stats);

int cmp_double(double a, double b);
char *double_to_str(double a);

#endif
