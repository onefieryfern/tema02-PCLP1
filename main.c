// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "battleships.h"
#include "grid.h"
#include "types.h"
#include <stdio.h>
#include <string.h>

#include "stats.h"

int main(void)
{
	int num_games;
	scanf("%d", &num_games);

	// Set up statistics storage
	stats_t *player1_stats = (stats_t *)calloc(num_games, sizeof(stats_t));
	stats_t *player2_stats = (stats_t *)calloc(num_games, sizeof(stats_t));

	if (!player1_stats || !player2_stats) {
		fprintf(stderr, "malloc() failed\n");

		free(player1_stats);
		free(player2_stats);

		return 0;
	}

	// Read and play num_games games
	for (int i = 0; i < num_games; ++i) {
		// Read game parameters
		game_state_t game_state;

		if (read_game_data(&game_state) == FAILURE) {
			fprintf(stderr, "read_game_data() failed\n");

			// Cleanup
			free_game_state(game_state);
			continue;
		}

		// Print players' grids
		print_grid(game_state.player1.grid, game_state.rows, game_state.cols);
		printf("\n");
		print_grid(game_state.player2.grid, game_state.rows, game_state.cols);

		// Start playing game
		play_one_game(&game_state, &player1_stats[i], &player2_stats[i]);

		// Cleanup
		free_game_state(game_state);
	}

	// Read statistics requirements and print results
	handle_stats(num_games, player1_stats, player2_stats);

	// Cleanup
	free(player1_stats);
	free(player2_stats);

	return 0;
}
