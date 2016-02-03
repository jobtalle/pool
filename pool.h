#pragma once

#include <stdint.h>

#define POOL_BLOCKS_INITIAL 1

typedef struct poolFreed{
	struct poolFreed *nextFree;
} poolFreed;

typedef struct {
	uint32_t elementSize;
	uint32_t blockSize;
	uint32_t used;
	int32_t block;
	poolFreed *freed;
	uint32_t blocksUsed;
	uint8_t **blocks;
} pool;

void poolInitialize(pool *p, uint32_t elementSize, uint32_t blockSize);
void poolFreePool(pool *p);

void *poolMalloc(pool *p);
void poolFree(pool *p, void *ptr);
void poolFreeAll(pool *p);