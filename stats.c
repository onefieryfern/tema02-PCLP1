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
	if (num_games == 'U') { // cerintele 2, 4
		if (game_index > count) {
			printf("Nu exista date pentru meciul cerut.\n");
			return;
		}

		char *min_max_str = (char *)malloc(256 * sizeof(char));
		min_max_str[0] = '\0';

		if (strcmp(min_max, "MIN") == 0) {

		} else { // strcmp(min_max, "MAX") == 0
			for (unsigned long long i = stats->num_successful_hits - 1; ; --i) {
				stats_t fake_stats = *stats;
				fake_stats.num_successful_hits = i;

				if (cmp_double(calc_game_acc(fake_stats), acc) < 0) {
					unsigned long long max = stats->num_successful_hits - (i + 1);
					sprintf(min_max_str, "%llu", max);
					break;
				}

				if (i == 0) {
					break;
				}
			}
		}

		char *game_acc = double_to_str(calc_game_acc(*stats));

		printf("%s.%s\n", game_acc, min_max_str);

		free(game_acc);
		free(min_max_str);
	} else { // num_games == 'T' // cerintele 1,3
		if (strcmp(min_max, "MIN") == 0) {

		} else { // strcmp(min_max, "MAX") == 0

		}
	}
}

double calc_game_acc(stats_t stats)
{
	double acc = (double)stats.num_successful_hits / (double)stats.num_hits * 100;

	return acc;
}

int cmp_double(double a, double b)
{
	char a_str[8] = "";
	sprintf(a_str, "%07.3lf", a);
	a_str[6] = '\0';

	char b_str[8] = "";
	sprintf(b_str, "%07.3lf", b);
	b_str[6] = '\0';

	if (strcmp(a_str, b_str) == 0) {
		return 0;
	}

	if (a > b) {
		return 1;
	}

	// a < b
	return -1;
}

char *double_to_str(double a)
{
	char str[8] = "";
	sprintf(str, "%07.3lf", a);
	str[6] = '\0';

	return dup_str(str);
}
