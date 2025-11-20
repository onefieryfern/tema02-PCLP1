// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "consts.h"
#include "types.h"
#include <stdio.h>

int main(void)
{
	unsigned long long num_games;
	scanf("%llu", &num_games);

	unsigned long long rows, cols;
	scanf("%llu %llu", &rows, &cols);

	unsigned long long num_ships_of_type[NUM_SHIPS + 1];
	num_ships_of_type[albacore] = (rows * cols) / 20;
	num_ships_of_type[laffey] = (rows * cols) / 30;
	num_ships_of_type[belfast] = (rows * cols) / 40;
	num_ships_of_type[yamato] = (rows * cols) / 55;
	num_ships_of_type[shinano] = (rows * cols) / 70;

	unsigned long long num_ships = 0;
	for (int i = 1; i <= NUM_SHIPS; ++i) {
		num_ships += num_ships_of_type[i];
	}

	return 0;
}
