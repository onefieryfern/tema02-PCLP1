// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef STATS_H
#define STATS_H

#include "types.h"

void handle_stats(int count, stats_t *player1_stats, stats_t *player2_stats);
void handle_stat(int count, stats_t *stats, char num_games, int game_index,
				 double acc, char *min_max);
double calc_game_acc(stats_t stats);
double calc_total_acc(int count, stats_t *stats);

void handle_single_game_min_stat(stats_t stat, double accuracy);
void handle_single_game_max_stat(stats_t stat, double accuracy);

void handle_all_games_min_stat(int count, stats_t *stats, double accuracy);
void handle_all_games_max_stat(int count, stats_t *stats, double accuracy);

char *double_to_str(double a);

#endif
