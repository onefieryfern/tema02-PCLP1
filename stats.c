// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "stats.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void handle_stats(unsigned long long count, stats_t *player1_stats, stats_t *player2_stats)
{
	char to_do;
	scanf(" %c", &to_do);

	if (to_do == 'Q') {
		return;
	}

	// Temporary
	if (to_do != '-') {
		return;
	}

	do {
		char player;
		scanf(" %c", &player);

		if (player == 'Q') {
			break;
		}

		char num_games;
		scanf(" %c", &num_games);

		char game_index_str[4];
		scanf(" %3s", game_index_str);
		unsigned short game_index = (unsigned short)(atoi(game_index_str) - 1);

		char acc_str[7];
		scanf(" %6s", acc_str);
		double acc = atof(acc_str);

		char min_max[4];
		scanf(" %3s", min_max);

		if (player == 'O') {
			handle_stat(count, player1_stats, num_games, game_index, acc, min_max);
		} else { // player == 'T'
			handle_stat(count, player2_stats, num_games, game_index, acc, min_max);
		}
	} while (1);
}

void handle_stat(unsigned long long count, stats_t *stats, char num_games, unsigned short game_index, double acc, char *min_max)
{
	if (num_games == 'U') {
		if (game_index > count) {
			printf("Nu exista date pentru meciul cerut.\n");
			return;
		}

		if (strcmp(min_max, "MIN") == 0) {
			handle_single_game_min_stat(stats[game_index], acc);
		} else { // strcmp(min_max, "MAX") == 0
			handle_single_game_max_stat(stats[game_index], acc);
		}
	} else { // num_games == 'T'
		if (strcmp(min_max, "MIN") == 0) {
			handle_all_games_min_stat(count, stats, acc);
		} else {
			// strcmp(min_max, "MAX") == 0
			handle_all_games_max_stat(count, stats, acc);
		}
	}
}

void handle_single_game_min_stat(stats_t stat, double accuracy)
{
	unsigned long long min = 0;

	for (unsigned long long i = stat.num_successful_hits; i <= stat.num_hits; ++i) {
		stats_t test_stats = stat;
		test_stats.num_successful_hits = i;

		if (cmp_double(calc_game_acc(test_stats), accuracy) >= 0) {
			min = i - stat.num_successful_hits;
			break;
		}
	}

	char *game_acc = double_to_str(calc_game_acc(stat));
	printf("%s.%llu\n", game_acc, min);

	free(game_acc);
}

void handle_single_game_max_stat(stats_t stat, double accuracy)
{
	unsigned long long max = 0;

	for (unsigned long long i = stat.num_successful_hits - 1; ; --i) {
		stats_t test_stats = stat;
		test_stats.num_successful_hits = i;

		if (cmp_double(calc_game_acc(test_stats), accuracy) < 0) {
			max = stat.num_successful_hits - (i + 1);
			break;
		}

		if (i == 0) {
			break;
		}
	}

	char *game_acc = double_to_str(calc_game_acc(stat));
	printf("%s.%llu\n", game_acc, max);

	free(game_acc);
}

void handle_all_games_min_stat(unsigned long long count, stats_t *stats, double accuracy)
{
	stats_t *ordered_stats = malloc(count * sizeof(stats_t));

	if (!ordered_stats) {
		fprintf(stderr, "malloc failed()\n");
		return;
	}

	for (unsigned long long i = 0; i < count; ++i) {
		ordered_stats[i] = stats[i];
	}

	for (unsigned long long i = 0; i < count - 1; ++i) {
		for (unsigned long long j = i + 1; j < count; ++j) {
			double change1 = (double)ordered_stats[i].num_ships_per_grid - calc_game_acc(ordered_stats[i]) / 100 * (double)ordered_stats[i].num_ships_per_grid;
			double change2 = (double)ordered_stats[j].num_ships_per_grid - calc_game_acc(ordered_stats[j]) / 100 * (double)ordered_stats[j].num_ships_per_grid;

			if (cmp_double(change1, change2) < 0) {
				stats_t tmp = ordered_stats[i];
				ordered_stats[i] = ordered_stats[j];
				ordered_stats[j] = tmp;
			}
		}
	}

	unsigned long long found_count = 0;
	for (unsigned long long i = 0; i < count; ++i) {
		if (ordered_stats[i].num_successful_hits == ordered_stats[i].num_hits) {
			continue;
		}

		if (cmp_double(calc_total_acc(count, ordered_stats), accuracy) >= 0) {
			break;
		}

		ordered_stats[i].num_successful_hits = ordered_stats[i].num_hits;
		++found_count;
	}

	char *total_acc = double_to_str(calc_total_acc(count, stats));
	printf("%s.%llu\n", total_acc, found_count);

	free(ordered_stats);
	free(total_acc);
}

void handle_all_games_max_stat(unsigned long long count, stats_t *stats, double accuracy)
{
	stats_t *ordered_stats = malloc(count * sizeof(stats_t));

	if (!ordered_stats) {
		fprintf(stderr, "malloc failed()\n");
		return;
	}

	for (unsigned long long i = 0; i < count; ++i) {
		ordered_stats[i] = stats[i];
	}

	for (unsigned long long i = 0; i < count - 1; ++i) {
		for (unsigned long long j = i + 1; j < count; ++j) {
			if (cmp_double(calc_game_acc(stats[i]) / 100 * (double)stats[i].num_ships_per_grid, calc_game_acc(stats[j]) / 100 * (double)stats[j].num_ships_per_grid) > 0) {
				stats_t tmp = ordered_stats[i];
				ordered_stats[i] = ordered_stats[j];
				ordered_stats[j] = tmp;
			}
		}
	}

	unsigned long long found_count = 0;
	for (unsigned long long i = 0; i < count; ++i) {
		if (ordered_stats[i].num_successful_hits == 0) {
			continue;
		}

		ordered_stats[i].num_successful_hits = 0;
		if (cmp_double(calc_total_acc(count, ordered_stats), accuracy) >= 0) {
			++found_count;
		} else {
			break;
		}
	}

	char *total_acc = double_to_str(calc_total_acc(count, stats));
	printf("%s.%llu\n", total_acc, found_count);

	free(ordered_stats);
	free(total_acc);
}

double calc_game_acc(stats_t stats)
{
	double acc = (double)stats.num_successful_hits / (double)stats.num_hits * 100;

	return acc;
}

double calc_total_acc(unsigned long long count, stats_t *stats)
{
	double top = 0;
	double bottom = 0;

	for (unsigned long long i = 0; i < count; ++i) {
		top += calc_game_acc(stats[i]) * (double)stats[i].num_ships_per_grid;
		bottom += (double)stats[i].num_ships_per_grid;
	}

	return top / bottom;
}

int cmp_double(double a, double b)
{
	int result;

	char *a_str = double_to_str(a);
	char *b_str = double_to_str(b);

	if (strcmp(a_str, b_str) == 0) {
		result = 0;
	}

	if (a > b) {
		result = 1;
	} else { // a < b
		result = -1;
	}

	free(a_str);
	free(b_str);

	return result;
}

char *double_to_str(double a)
{
	char str[16] = "";
	sprintf(str, "%015.11lf", a);
	str[6] = '\0';

	return dup_str(str);
}
