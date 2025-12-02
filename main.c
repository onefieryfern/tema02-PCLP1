// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "battleships.h"
#include <stdio.h>

int main(void)
{
	unsigned long long num_games;
	scanf("%llu", &num_games);

	for (unsigned long long i = 1; i <= num_games; ++i) {
		read_and_play_one_game();
	}

	return 0;
}
