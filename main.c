// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "battleships.h"
#include "grid.h"
#include "types.h"
#include <stdio.h>

int main(void)
{
	unsigned long long num_games;
	scanf("%llu", &num_games);

	// Set up statistics
	stats_t *stats = (stats_t *)malloc(num_games * sizeof(stats_t));

	for (unsigned long long i = 0; i < num_games; ++i) {
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
		play_one_game(&game_state, &stats[i]);

		// Cleanup
		free_game_state(game_state);
	}

	// Cleanup
	free(stats);

	return 0;
}
