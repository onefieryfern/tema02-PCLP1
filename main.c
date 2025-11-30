// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "consts.h"
#include "grid.h"
#include "memory.h"
#include "types.h"
#include <stdio.h>

#include "logging.h"

int get_other_player(int player);

int main(void)
{
	unsigned long long num_games;
	scanf("%llu", &num_games);

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
	int **player1_grid = NULL;
	int **player2_grid = NULL;

	get_new_grids(NULL, NULL, &player1_grid, &player2_grid, rows, cols);

	if (!player1_grid || !player2_grid) {
		fprintf(stderr, "get_new_grids() failed\n");

		return -1;
	}

	ship_t *player1_ships = alloc_ship_vector(num_ships);
	ship_t *player2_ships = alloc_ship_vector(num_ships);

	if (!player1_ships || !player2_ships) {
		fprintf(stderr, "alloc_ship_vector() failed\n");

		free_int_matrix(player1_grid);
		free_int_matrix(player2_grid);

		free(player1_ships);
		free(player2_ships);
		return -1;
	}

	size_t num_player1_ships = 0;
	size_t num_player2_ships = 0;

	int current_player = 1;
	for (unsigned long long i = 0; i < 2 * num_ships; ++i) {
		ship_t ship;

		if (read_ship(&ship) == -1) {
			log_cannot_place(ship.len, ship.dir, get_grid_coords(ship.start));

			--i;
			continue;
		}

		if (current_player == 1) {
			if (!add_ship_to_grid(player1_grid, rows, cols, ship)) {
				log_cannot_place(ship.len, ship.dir, get_grid_coords(ship.start));

				--i;
				continue;
			}

			player1_ships[num_player1_ships] = ship;
			num_player1_ships++;
		} else { // current_player == 2
			if (!add_ship_to_grid(player2_grid, rows, cols, ship)) {
				log_cannot_place(ship.len, ship.dir, get_grid_coords(ship.start));

				--i;
				continue;
			}

			player2_ships[num_player2_ships] = ship;
			num_player2_ships++;
		}

		current_player = get_other_player(current_player);
	}

	print_int_array(player1_grid, rows, cols);
	printf("\n");
	print_int_array(player2_grid, rows, cols);

	// Free used memory
	free_int_matrix(player1_grid);
	free_int_matrix(player2_grid);

	free(player1_ships);
	free(player2_ships);

	return 0;
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
