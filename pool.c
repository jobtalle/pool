#include <string.h>
#include <stdlib.h>

#include "pool.h"

#ifndef max
#define max(a,b) (a<b?b:a)
#endif

void poolInitialize(pool *p, unsigned int elementSize, unsigned int blockSize)
{
	unsigned int i;

	p->elementSize = max(elementSize, sizeof(poolFreed));
	p->blockSize = blockSize;
	p->used = blockSize - 1;
	p->block = -1;

	p->blocksUsed = POOL_BLOCKS_INITIAL;
	p->blocks = malloc(sizeof(char*)* p->blocksUsed);

	for(i = 0; i < p->blocksUsed; i++)
		p->blocks[i] = NULL;
	
	p->freed = NULL;
}

void poolFreePool(pool *p)
{
	unsigned int i;
	for(i = 0; i < p->blocksUsed; i++) {
		if(p->blocks[i] == NULL) {
			break;
		}
		else {
			free(p->blocks[i]);
		}
	}

	free(p->blocks);
}

void *poolMalloc(pool *p)
{
	if(p->freed != NULL) {
		void *recycle = p->freed;
		p->freed = p->freed->nextFree;
		return recycle;
	}

	p->used++;
	if(p->used == p->blockSize) {
		p->used = 0;
		p->block++;
		if(p->block == p->blocksUsed) {
			unsigned int i;

			p->blocksUsed <<= 1;
			p->blocks = realloc(p->blocks, sizeof(char*)* p->blocksUsed);

			for(i = p->blocksUsed >> 1; i < p->blocksUsed; i++) {
				p->blocks[i] = NULL;
			}
		}

		if(p->blocks[p->block] == NULL) {
			p->blocks[p->block] = malloc(p->elementSize * p->blockSize);
		}
	}
	
	return p->blocks[p->block] + p->used * p->elementSize;
}

void poolFree(pool *p, void *ptr)
{
	poolFreed *pFreed = p->freed;
	p->freed = ptr;
	p->freed->nextFree = pFreed;
}

void poolFreeAll(pool *p)
{
	p->used = p->blockSize - 1;
	p->block = -1;
	p->freed = NULL;
}

bool poolIsFreed(pool *p, void *ptr)
{
	if(ptr == NULL){
		return true;
	}else if(p->freed == NULL){
		return false;
	}

	poolFreed *freed = p->freed;
	while(freed->nextFree != NULL){
		freed = freed->nextFree;
		if(freed == ptr){
			return true;
		}
	}
	return false;
}

void *poolGetNext(pool *p, void *ptr)
{
	unsigned int i, j;
	for(i = 0; i <= p->block; i++){
		for(j = 0; j < p->blockSize; j += p->elementSize){
			if(p->blocks[i] + j == ptr){
				goto found;
			}
		}
	}
	return NULL;
found: ;
			 void *next = NULL;
			 do{
				 if(!poolIsFreed(p, next)){
					 return next;
				 }
				 if((j += p->elementSize) == p->blockSize){
					 j = 0;
					 i++;
					 if(i == p->block){
						 return NULL;
					 }
				 }
				 next = p->blocks[i] + j;
			 } while(next != NULL && i * p->blockSize + j < p->block * p->blockSize + (p->used + 1) * p->elementSize);
			 return NULL;
}
