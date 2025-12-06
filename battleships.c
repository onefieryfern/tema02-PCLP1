// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "battleships.h"
#include "consts.h"
#include "grid.h"
#include "logging.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>

int read_starting_grid(game_state_t *game_state)
{
	// Grid initialisation
	scanf("%llu %llu", &game_state->rows, &game_state->cols);

	game_state->player1.grid = alloc_grid(game_state->rows, game_state->cols);
	game_state->player2.grid = alloc_grid(game_state->rows, game_state->cols);

	// Ensure memory was allocated
	if (!game_state->player1.grid || !game_state->player2.grid) {
		fprintf(stderr, "alloc_grid() failed\n");
		return FAILURE;
	}

	clear_grid(game_state->player1.grid, game_state->rows, game_state->cols);
	clear_grid(game_state->player2.grid, game_state->rows, game_state->cols);

	// Read, save and place ships
	if (read_starting_ships(game_state) == FAILURE) {
		fprintf(stderr, "handle_input_ships() failed\n");
		return FAILURE;
	}

	return SUCCESS;
}

int read_starting_ships(game_state_t *game_state)
{
	// Ships array initialisation
	unsigned long long num_ships_per_grid =
		get_num_ships_for_size(game_state->rows, game_state->cols);

	game_state->player1.ships =
		(ship_t *)malloc(num_ships_per_grid * sizeof(ship_t));
	game_state->player2.ships =
		(ship_t *)malloc(num_ships_per_grid * sizeof(ship_t));

	// Ensure memory was allocated
	if (!game_state->player1.ships || !game_state->player2.ships) {
		fprintf(stderr, "malloc() failed\n");

		return FAILURE;
	}

	// Read ships
	game_state->player1.num_ships = 0;
	game_state->player2.num_ships = 0;

	int current_player = 1;
	for (unsigned long long i = 0; i < 2 * num_ships_per_grid; ++i) {
		player_data_t *current_player_data = NULL;

		if (current_player == 1) {
			current_player_data = &game_state->player1;
		} else { // current_player == 2
			current_player_data = &game_state->player2;
		}

		ship_t *ship =
			&current_player_data->ships[current_player_data->num_ships];

		if (read_ship(ship) == ERR_SHIP_OUT_OF_BOUNDS ||
			!is_ship_placeable(current_player_data->grid, game_state->rows,
							   game_state->cols, *ship)) {
			log_cannot_place(*ship);

			--i;
			continue;
		}

		add_ship_to_grid(current_player_data->grid, ship);
		current_player_data->num_ships++;

		// The player changes only upon successful ship placement
		current_player = current_player == 1 ? 2 : 1;
	}

	return SUCCESS;
}

void play_one_game(game_state_t *game_state, stats_t *player1_stats,
				   stats_t *player2_stats)
{
	// Set up statistics
	player1_stats->num_ships_per_grid =
		get_num_ships_for_size(game_state->rows, game_state->cols);
	player2_stats->num_ships_per_grid =
		get_num_ships_for_size(game_state->rows, game_state->cols);

	// Game loop
	int current_player = 1;

	while (1) {
		// Read space to hit
		coords_t hit_coords;
		scanf("%zu %zu", &hit_coords.row, &hit_coords.col);

		hit_coords = get_grid_coords(hit_coords);

		// Player selection
		player_data_t *attacked_player_data = NULL;
		stats_t *attacking_player_stats = NULL;

		if (current_player == 1) {
			attacked_player_data = &game_state->player2;
			attacking_player_stats = player1_stats;
		} else { // current_player == 2
			attacked_player_data = &game_state->player1;
			attacking_player_stats = player2_stats;
		}

		// Try hitting ship
		ship_t hit_ship;

		int hit_result = hit_space(attacked_player_data->grid, hit_coords,
								   &hit_ship);
		attacking_player_stats->num_hits++;

		switch (hit_result) {
		case HIT_MISS:
			log_hit_miss(current_player, hit_coords);
			break;
		case HIT_SUCCESS:
			log_hit_successful(current_player, hit_ship.len, hit_coords);

			attacking_player_stats->num_successful_hits++;
			break;
		case HIT_DESTROY:
			log_destroy(current_player, hit_ship);
			attacked_player_data->num_ships--;

			attacking_player_stats->num_successful_hits++;
			break;
		case HIT_REPEAT:
			log_already_hit(current_player, hit_coords);
		}

		// Win condition
		if (game_state->player1.num_ships == 0 ||
			game_state->player2.num_ships == 0) {
			log_player_won(current_player);
			break;
		}

		current_player = current_player == 1 ? 2 : 1;
	}
}

int hit_space(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship)
{
	grid_element_t *space_to_hit = &grid[hit_coords.row][hit_coords.col];

	if (space_to_hit->space == SPACE_HIT) {
		return HIT_REPEAT;
	}

	// Mark space as hit
	space_to_hit->space = SPACE_HIT;

	if (!space_to_hit->ship) {
		return HIT_MISS;
	}

	*hit_ship = *space_to_hit->ship;

	// A ship is destroyed when its head is hit
	if (are_coords_equal(hit_coords, hit_ship->start)) {
		remove_ship_from_grid(grid, *hit_ship);

		return HIT_DESTROY;
	}

	return HIT_SUCCESS;
}
