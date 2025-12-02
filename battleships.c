// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "battleships.h"
#include "consts.h"
#include "grid.h"
#include "logging.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>

int play_one_game(void)
{
	game_state_t game_state = { 0, 0, { NULL, NULL, 0 }, { NULL, NULL, 0 } };

	scanf("%llu %llu", &game_state.rows, &game_state.cols);

	// Grid initialisation
	get_new_grids(NULL, NULL, &game_state.player1.grid, &game_state.player2.grid, game_state.rows, game_state.cols);

	if (!game_state.player1.grid || !game_state.player2.grid) {
		fprintf(stderr, "get_new_grids() failed\n");

		// Cleanup
		free_game_state(game_state);

		return FAILURE;
	}

	// Read, save and place ships
	if (handle_input_ships(&game_state) == FAILURE) {
		return FAILURE;
	}

	// Print players' grids
	print_grid(game_state.player1.grid, game_state.rows, game_state.cols);
	printf("\n");
	print_grid(game_state.player2.grid, game_state.rows, game_state.cols);

	// Play one game
	int current_player = 1;

	unsigned long long num_ships_left_player1 = game_state.player1.num_ships;
	unsigned long long num_ships_left_player2 = game_state.player2.num_ships;

	while (1) {
		coords_t grid_hit_coords;
		scanf("%zu %zu", &grid_hit_coords.row, &grid_hit_coords.col);

		coords_t matrix_hit_coords = get_grid_coords(grid_hit_coords);

		grid_element_t **current_attacked_grid = NULL;
		unsigned long long *num_ships_currently_attacked = NULL;

		if (current_player == 1) {
			current_attacked_grid = game_state.player2.grid;
			num_ships_currently_attacked = &num_ships_left_player2;
		} else { // current_player == 2
			current_attacked_grid = game_state.player1.grid;
			num_ships_currently_attacked = &num_ships_left_player1;
		}

		// Try to hit ship
		ship_t hit_ship;

		int hit_result = try_hitting(current_attacked_grid, matrix_hit_coords, &hit_ship);

		switch (hit_result) {
		case HIT_MISS:
			log_hit_miss(current_player, grid_hit_coords);
			break;
		case HIT_SUCCESS:
			log_hit_successful(current_player, hit_ship.len, grid_hit_coords);
			break;
		case HIT_DESTROY:
			(*num_ships_currently_attacked)--;

			log_destroy(current_player, hit_ship);
			break;
		case HIT_REPEAT:
			log_already_hit(current_player, grid_hit_coords);
		}

		if (num_ships_left_player1 == 0 || num_ships_left_player2 == 0) {
			log_player_won(current_player);
			break;
		}

		current_player = get_other_player(current_player);
	}

	// Cleanup
	free_game_state(game_state);

	return SUCCESS;
}

int get_other_player(int player)
{
	switch (player) {
	case 1:
		return 2;
	case 2:
		return 1;
	default:
		return -1;
	}
}

int handle_input_ships(game_state_t *game_state)
{
	unsigned long long num_ships_per_player = get_num_ships_for_size(game_state->rows, game_state->cols);

	game_state->player1.ships = alloc_ship_vector(num_ships_per_player);
	game_state->player2.ships = alloc_ship_vector(num_ships_per_player);

	if (!game_state->player1.ships || !game_state->player2.ships) {
		fprintf(stderr, "alloc_ship_vector() failed\n");

		// Cleanup
		free_game_state(*game_state);

		return FAILURE;
	}

	game_state->player1.num_ships = 0;
	game_state->player2.num_ships = 0;

	int current_player = 1;
	for (unsigned long long i = 0; i < 2 * num_ships_per_player; ++i) {
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

		if (!add_ship_to_grid(current_grid, game_state->rows, game_state->cols, &current_ships[*current_num_ships])) {
			log_cannot_place(ship);

			--i;
			continue;
		}

		(*current_num_ships)++;

		current_player = get_other_player(current_player);
	}

	return SUCCESS;
}

int try_hitting(grid_element_t **grid, coords_t hit_coords, ship_t *hit_ship)
{
	grid_element_t *space_to_hit = &grid[hit_coords.row][hit_coords.col];

	if (space_to_hit->space == (unsigned short)-1) {
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

		if (grid[current_coords.row][current_coords.col].space != (unsigned short)-1) {
			grid[current_coords.row][current_coords.col].space = 0;
			grid[current_coords.row][current_coords.col].ship = NULL;
		}
	}

	return 1;
}
