// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#include "memory.h"
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
