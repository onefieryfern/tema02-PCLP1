// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef MEMORY_H
#define MEMORY_H

#include "types.h"
#include <stdlib.h>

// char *alloc_str(size_t size);
char *dup_str(const char *str);

int **alloc_int_matrix(size_t rows, size_t cols);
void free_int_matrix(int **arr);
void zero_out_matrix(int **arr, size_t rows, size_t cols);

ship_t *alloc_ship_vector(size_t size);

Complex_grid **alloc_complex_grid(size_t rows, size_t cols);
void free_complex_grid(Complex_grid **arr);
void zero_out_complex_grid(Complex_grid **arr, size_t rows, size_t cols);

#endif
