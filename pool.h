#pragma once

#define POOL_BLOCKS  1024

typedef struct poolFreed{
	struct poolFreed *nextFree;
} poolFreed;

typedef struct {
	unsigned int elementSize;
	unsigned int blockSize;
	unsigned int used;
	int block;
	poolFreed *freed;
	char *blocks[POOL_BLOCKS];
} pool;

void poolInitialize(pool *p, unsigned int elementSize, unsigned int blockSize);
void poolFreePool(pool *p);

void *poolMalloc(pool *p);
void poolFree(pool *p, void *ptr);
void poolFreeAll(pool *p);