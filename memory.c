// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
char *alloc_str(size_t size)
{
	char* str = (char *)malloc(size * sizeof(char));

	if (str == NULL) {
		fprintf(stderr, "malloc() failed\n");
	}

	return str;
}
*/

char *dup_str(const char *str)
{
	size_t len = strlen(str);
	char *new_str = (char *)malloc((len + 1) * sizeof(char));

	if (!new_str) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}

	strcpy(new_str, str);
	return new_str;
}

int **alloc_int_matrix(size_t rows, size_t cols)
{
	int **arr = (int **)malloc(rows * sizeof(int *));

	if (!arr) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}

	int *mem_zone = (int *)malloc(rows * cols * sizeof(int));

	if (!mem_zone) {
		fprintf(stderr, "malloc() failed\n");

		free(arr);
		return NULL;
	}

	for (size_t i = 0; i < rows; ++i) {
		arr[i] = mem_zone;

		mem_zone += cols;
	}
	mem_zone = NULL;

	return arr;
}

void free_int_matrix(int **arr)
{
	if (!arr) {
		return;
	}

	int *mem_zone = arr[0];

	free(mem_zone);
	free(arr);
}

void zero_out_matrix(int **arr, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			arr[i][j] = 0;
		}
	}
}

ship_t *alloc_ship_vector(size_t size)
{
	ship_t *ships = (ship_t *)malloc(size * sizeof(ship_t));

	if (!ships) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}

	return ships;
}
