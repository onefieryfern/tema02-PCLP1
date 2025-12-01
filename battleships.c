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
	unsigned long long rows, cols;
	scanf("%llu %llu", &rows, &cols);

	unsigned long long num_ships_of_type[NUM_SHIP_TYPES + 1];
	num_ships_of_type[albacore] = (rows * cols) / 20;
	num_ships_of_type[laffey] = (rows * cols) / 30;
	num_ships_of_type[belfast] = (rows * cols) / 40;
	num_ships_of_type[yamato] = (rows * cols) / 55;
	num_ships_of_type[shinano] = (rows * cols) / 70;

	unsigned long long num_ships = 0;
	for (int i = 1; i <= NUM_SHIP_TYPES; ++i) {
		num_ships += num_ships_of_type[i];
	}

	// Grid initialisation
	grid_element_t **player1_grid = NULL;
	grid_element_t **player2_grid = NULL;

	get_new_grids(NULL, NULL, &player1_grid, &player2_grid, rows, cols);

	if (!player1_grid || !player2_grid) {
		fprintf(stderr, "get_new_grids() failed\n");

		return FAILURE;
	}

	// Read, save and place ships
	ship_t *player1_ships = alloc_ship_vector(num_ships);
	ship_t *player2_ships = alloc_ship_vector(num_ships);

	if (!player1_ships || !player2_ships) {
		fprintf(stderr, "alloc_ship_vector() failed\n");

		// Cleanup
		free_grid(player1_grid);
		free_grid(player2_grid);

		free(player1_ships);
		free(player2_ships);
		return FAILURE;
	}

	unsigned long long num_player1_ships = 0;
	unsigned long long num_player2_ships = 0;

	int current_player = 1;
	for (unsigned long long i = 0; i < 2 * num_ships; ++i) {
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
			current_grid = player1_grid;
			current_ships = player1_ships;
			current_num_ships = &num_player1_ships;

		} else { // current_player == 2
			current_grid = player2_grid;
			current_ships = player2_ships;
			current_num_ships = &num_player2_ships;
		}

		current_ships[*current_num_ships] = ship;

		if (!add_ship_to_grid(current_grid, rows, cols, &current_ships[*current_num_ships])) {
			log_cannot_place(ship);

			--i;
			continue;
		}

		(*current_num_ships)++;

		current_player = get_other_player(current_player);
	}

	// Print players' grids
	print_grid(player1_grid, rows, cols);
	printf("\n");
	print_grid(player2_grid, rows, cols);

	// Play one game
	current_player = 1;

	unsigned long long num_ships_left_player1 = num_player1_ships;
	unsigned long long num_ships_left_player2 = num_player2_ships;

	while (1) {
		coords_t grid_hit_coords;
		scanf("%zu %zu", &grid_hit_coords.row, &grid_hit_coords.col);

		coords_t matrix_hit_coords = get_grid_coords(grid_hit_coords);

		grid_element_t **current_attacked_grid = NULL;
		unsigned long long *num_ships_currently_attacked = NULL;

		if (current_player == 1) {
			current_attacked_grid = player2_grid;
			num_ships_currently_attacked = &num_ships_left_player2;
		} else { // current_player == 2
			current_attacked_grid = player1_grid;
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

	// Free used memory
	free_grid(player1_grid);
	free_grid(player2_grid);

	free(player1_ships);
	free(player2_ships);

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

int find_hit_ship_index(ship_t *ships, size_t num_ships, coords_t hit_coords, size_t *index)
{
	for (size_t i = 0; i < num_ships; ++i) {
		ship_t current_ship = ships[i];

		coords_t ship_coords[MAX_SHIP_LEN];
		get_ordered_ship_coords(ship_coords, current_ship);

		for (size_t j = 0; j < current_ship.len; ++j) {
			if (are_coords_equal(ship_coords[j], hit_coords)) {
				*index = i;

				return 1;
			}
		}
	}

	// Ship not found
	return 0;
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
