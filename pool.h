#pragma once

#define POOL_BLOCKS_INITIAL 1

typedef struct poolFreed{
	struct poolFreed *nextFree;
} poolFreed;

typedef struct {
	unsigned int elementSize;
	unsigned int blockSize;
	unsigned int used;
	int block;
	poolFreed *freed;
	unsigned int blocksUsed;
	char **blocks;
} pool;

void poolInitialize(pool *p, unsigned int elementSize, unsigned int blockSize);
void poolFreePool(pool *p);

void *poolMalloc(pool *p);
void poolFree(pool *p, void *ptr);
void poolFreeAll(pool *p);

bool poolIsFreed(pool *p, void *ptr);
void *poolGetNext(pool *p, void *ptr);
