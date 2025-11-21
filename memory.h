// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef MEMORY_H
#define MEMORY_H

#include <stdlib.h>

// char *alloc_str(size_t size);
char *dup_str(const char *str);

int **alloc_array(size_t rows, size_t cols);
void free_array(int **arr);

#endif
