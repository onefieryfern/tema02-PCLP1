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

	game_state->player1.num_ships = 0;
	game_state->player2.num_ships = 0;

	int current_player = 1;
	for (unsigned long long i = 0; i < 2 * num_ships_per_grid; ++i) {
		ship_t ship;

		if (read_ship(&ship) == ERR_SHIP_OUT_OF_BOUNDS) {
			log_cannot_place(ship);

			--i;
			continue;
		}

		grid_element_t **current_grid = NULL;
		ship_t *current_ships = NULL;
		unsigned long long *current_num_ships = NULL;

		if (current_player == 1) {
			current_grid = game_state->player1.grid;
			current_ships = game_state->player1.ships;
			current_num_ships = &game_state->player1.num_ships;
		} else { // current_player == 2
			current_grid = game_state->player2.grid;
			current_ships = game_state->player2.ships;
			current_num_ships = &game_state->player2.num_ships;
		}

		current_ships[*current_num_ships] = ship;

		if (!add_ship_to_grid(current_grid, game_state->rows, game_state->cols,
							  &current_ships[*current_num_ships])) {
			log_cannot_place(ship);

			--i;
			continue;
		}

		(*current_num_ships)++;

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

	int current_player = 1;

	unsigned long long *num_ships_left_player1 = &game_state->player1.num_ships;
	unsigned long long *num_ships_left_player2 = &game_state->player2.num_ships;

	// Game loop
	while (1) {
		// Read space to hit
		coords_t input_hit_coords;
		scanf("%zu %zu", &input_hit_coords.row, &input_hit_coords.col);

		coords_t hit_coords = get_grid_coords(input_hit_coords);

		// Player selection
		grid_element_t **currently_attacked_grid = NULL;
		unsigned long long *num_ships_currently_attacked = NULL;
		stats_t *current_stats = NULL;

		if (current_player == 1) {
			currently_attacked_grid = game_state->player2.grid;
			num_ships_currently_attacked = num_ships_left_player2;
			current_stats = player1_stats;
		} else { // current_player == 2
			currently_attacked_grid = game_state->player1.grid;
			num_ships_currently_attacked = num_ships_left_player1;
			current_stats = player2_stats;
		}

		// Try hitting ship
		ship_t hit_ship;

		int hit_result = hit_space(currently_attacked_grid, hit_coords,
			&hit_ship);
		current_stats->num_hits++;

		switch (hit_result) {
		case HIT_MISS:
			log_hit_miss(current_player, input_hit_coords);
			break;
		case HIT_SUCCESS:
			log_hit_successful(current_player, hit_ship.len, input_hit_coords);

			current_stats->num_successful_hits++;
			break;
		case HIT_DESTROY:
			log_destroy(current_player, hit_ship);
			(*num_ships_currently_attacked)--;

			current_stats->num_successful_hits++;
			break;
		case HIT_REPEAT:
			log_already_hit(current_player, input_hit_coords);
		}

		// Win condition
		if (*num_ships_left_player1 == 0 || *num_ships_left_player2 == 0) {
			log_player_won(current_player);
			break;
		}

		current_player = current_player == 1 ? 2 : 1;
	}
}

int hit_space(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship)
{
	grid_element_t *space_to_hit = &grid[hit_coords.row][hit_coords.col];

	if (space_to_hit->space == -1) {
		return HIT_REPEAT;
	}

	// Mark space as hit
	space_to_hit->space = -1;

	if (space_to_hit->ship) {
		*hit_ship = *space_to_hit->ship;

		// Rule: if ship head is hit, the whole ship is destroyed
		int was_ship_head_hit = 0;
		if (are_coords_equal(hit_coords, hit_ship->start)) {
			was_ship_head_hit = 1;

			destroy_ship(grid, *hit_ship);
		}

		// Check if ship was destroyed as a result of the hit
		if (was_ship_head_hit || is_ship_destroyed(grid, *hit_ship)) {
			return HIT_DESTROY;
		}

		return HIT_SUCCESS;
		}

	return HIT_MISS;
}

int is_ship_destroyed(grid_element_t **grid, ship_t ship)
{
	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, ship);

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		if (grid[current_coords.row][current_coords.col].space != 0) {
			return 0;
		}
	}

	return 1;
}

int destroy_ship(grid_element_t **grid, ship_t ship)
{
	coords_t ship_coords[MAX_SHIP_LEN];
	get_ordered_ship_coords(ship_coords, ship);

	for (size_t i = 0; i < ship.len; ++i) {
		coords_t current_coords = ship_coords[i];

		if (grid[current_coords.row][current_coords.col].space != -1) {
			grid[current_coords.row][current_coords.col].space = 0;
			grid[current_coords.row][current_coords.col].ship = NULL;
		}
	}

	return 1;
}
