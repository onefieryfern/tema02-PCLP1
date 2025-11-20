// Copyright 2025 Andrei N. FLOREA
// andrei.florea1311@stud.acs.upb.ro

#ifndef TYPES_H
#define TYPES_H

typedef struct {
	unsigned long long x;
	unsigned long long y;
} coords_t;

typedef enum { vertical, horizontal } direction_t;

typedef enum {
	albacore = 1,
	laffey = 2,
	belfast = 3,
	yamato = 4,
	shinano = 5
} ship_type_t;

#endif
