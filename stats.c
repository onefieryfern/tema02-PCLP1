// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "stats.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_and_handle_stats(int count, stats_t *player1_stats,
						   stats_t *player2_stats)
{
	char action_to_take;
	scanf(" %c", &action_to_take);

	if (action_to_take == 'Q') {
		return;
	}

	// If wanted statistics are encrypted, do nothing
	// Encryption is not yet implemented
	if (action_to_take != '-') {
		return;
	}

	do {
		char player;
		scanf(" %c", &player);

		// Check if end of wanted statistics was reached
		if (player == 'Q') {
			break;
		}

		char num_games;
		scanf(" %c", &num_games);

		char game_index_str[GAME_INDEX_MAX_LEN + 1];
		scanf(" %3s", game_index_str);

		// Statistics are indexed from 0 internally
		int game_index = atoi(game_index_str) - 1;

		char accuracy_str[ACCURACY_MAX_LEN + 1];
		scanf(" %6s", accuracy_str);
		double accuracy = atof(accuracy_str);

		char min_or_max[MIN_MAX_LEN + 1];
		scanf(" %3s", min_or_max);

		if (player == 'O') {
			handle_stat(count, player1_stats, num_games, game_index,
						accuracy, min_or_max);
		} else { // player == 'T'
			handle_stat(count, player2_stats, num_games, game_index,
						accuracy, min_or_max);
		}
	} while (1);
}

// This function chooses which statistic to compute and display
void handle_stat(int count, stats_t *stats, char num_games, int game_index,
				 double accuracy, char *min_or_max)
{
	if (num_games == 'U') {
		if (game_index > count) {
			printf("Nu exista date pentru meciul cerut.\n");
			return;
		}

		if (strcmp(min_or_max, "MIN") == 0) {
			handle_single_game_min_stat(stats[game_index], accuracy);
		} else { // strcmp(min_max, "MAX") == 0
			handle_single_game_max_stat(stats[game_index], accuracy);
		}
	} else { // num_games == 'T'
		if (strcmp(min_or_max, "MIN") == 0) {
			handle_all_games_min_stat(count, stats, accuracy);
		} else {
			// strcmp(min_max, "MAX") == 0
			handle_all_games_max_stat(count, stats, accuracy);
		}
	}
}

void handle_single_game_min_stat(stats_t stat, double accuracy)
{
	unsigned long long min = 0;

	for (unsigned long long i = stat.num_successful_hits;
		 i <= stat.num_hits;
		 ++i) {
		stats_t test_stats = stat;
		test_stats.num_successful_hits = i;

		if (calc_game_accuracy(test_stats) >= accuracy) {
			min = i - stat.num_successful_hits;
			break;
		}
	}

	char *game_accuracy = double_to_str(calc_game_accuracy(stat));
	printf("%s.%llu\n", game_accuracy, min);

	free(game_accuracy);
}

void handle_single_game_max_stat(stats_t stat, double accuracy)
{
	unsigned long long max = 0;

	for (unsigned long long i = stat.num_successful_hits - 1; ; --i) {
		stats_t test_stats = stat;
		test_stats.num_successful_hits = i;

		if (calc_game_accuracy(test_stats) < accuracy) {
			max = stat.num_successful_hits - (i + 1);
			break;
		}

		if (i == 0) {
			break;
		}
	}

	char *game_accuracy = double_to_str(calc_game_accuracy(stat));
	printf("%s.%llu\n", game_accuracy, max);

	free(game_accuracy);
}

void handle_all_games_min_stat(int count, stats_t *stats, double accuracy)
{
	stats_t *ordered_stats = malloc(count * sizeof(stats_t));

	if (!ordered_stats) {
		fprintf(stderr, "malloc failed()\n");
		return;
	}

	for (int i = 0; i < count; ++i) {
		ordered_stats[i] = stats[i];
	}

	for (int i = 0; i < count - 1; ++i) {
		for (int j = i + 1; j < count; ++j) {
			double diff_to_100_i =
				(1 - calc_game_accuracy(ordered_stats[i]) / 100)
				* (double)ordered_stats[i].num_ships_per_grid;
			double diff_to_100_j =
				(1 - calc_game_accuracy(ordered_stats[j]) / 100)
				* (double)ordered_stats[j].num_ships_per_grid;

			if (diff_to_100_i < diff_to_100_j) {
				stats_t tmp = ordered_stats[i];
				ordered_stats[i] = ordered_stats[j];
				ordered_stats[j] = tmp;
			}
		}
	}

	int found_count = 0;
	for (int i = 0; i < count; ++i) {
		// Ignore stats that are already at 100%
		if (ordered_stats[i].num_successful_hits == ordered_stats[i].num_hits) {
			continue;
		}

		if (calc_total_accuracy(count, ordered_stats) >= accuracy) {
			break;
		}

		ordered_stats[i].num_successful_hits = ordered_stats[i].num_hits;
		++found_count;
	}

	char *total_accuracy = double_to_str(calc_total_accuracy(count, stats));
	printf("%s.%d\n", total_accuracy, found_count);

	free(ordered_stats);
	free(total_accuracy);
}

void handle_all_games_max_stat(int count, stats_t *stats, double accuracy)
{
	stats_t *ordered_stats = malloc(count * sizeof(stats_t));

	if (!ordered_stats) {
		fprintf(stderr, "malloc failed()\n");
		return;
	}

	for (int i = 0; i < count; ++i) {
		ordered_stats[i] = stats[i];
	}

	for (int i = 0; i < count - 1; ++i) {
		for (int j = i + 1; j < count; ++j) {
			double diff_to_0_i = calc_game_accuracy(stats[i]) / 100
				* (double)ordered_stats[i].num_ships_per_grid;
			double diff_to_0_j = calc_game_accuracy(stats[j]) / 100
				* (double)ordered_stats[j].num_ships_per_grid;

			if (diff_to_0_i > diff_to_0_j) {
				stats_t tmp = ordered_stats[i];
				ordered_stats[i] = ordered_stats[j];
				ordered_stats[j] = tmp;
			}
		}
	}

	int found_count = 0;
	for (int i = 0; i < count; ++i) {
		ordered_stats[i].num_successful_hits = 0;
		if (calc_total_accuracy(count, ordered_stats) >= accuracy) {
			++found_count;
		} else {
			break;
		}
	}

	char *total_accuracy = double_to_str(calc_total_accuracy(count, stats));
	printf("%s.%d\n", total_accuracy, found_count);

	free(ordered_stats);
	free(total_accuracy);
}

double calc_game_accuracy(stats_t stats)
{
	return (double)stats.num_successful_hits / (double)stats.num_hits * 100;
}

double calc_total_accuracy(int count, stats_t *stats)
{
	double numerator = 0;
	double denominator = 0;

	for (int i = 0; i < count; ++i) {
		numerator += calc_game_accuracy(stats[i])
			* (double)stats[i].num_ships_per_grid;
		denominator += (double)stats[i].num_ships_per_grid;
	}

	return numerator / denominator;
}

// Convert a double to a string with as least rounding as possible
// The double will be 0-padded and have exactly 2 digits after the `.`: `000.00`
char *double_to_str(double a)
{
	char str[17] = "";
	sprintf(str, "%016.12lf", a);
	str[6] = '\0';

	return dup_str(str);
}
