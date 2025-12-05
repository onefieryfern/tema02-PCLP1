// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "memory.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

grid_element_t **alloc_grid(size_t rows, size_t cols)
{
	grid_element_t **arr =
		(grid_element_t **)malloc(rows * sizeof(grid_element_t *));

	if (!arr) {
		fprintf(stderr, "malloc() failed\n");
		return NULL;
	}

	grid_element_t *mem_zone =
		(grid_element_t *)malloc(rows * cols * sizeof(grid_element_t));

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

void free_grid(grid_element_t **grid)
{
	if (!grid) {
		return;
	}

	grid_element_t *mem_zone = grid[0];

	free(mem_zone);
	free(grid);
}

void clear_grid(grid_element_t **grid, size_t rows, size_t cols)
{
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			grid[i][j].space = 0;
			grid[i][j].ship = NULL;
		}
	}
}
