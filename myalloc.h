/**
 * \file myalloc.h
 * \brief Entête des fonctions
 * \author Daniel FRANCOIS et Julien VEYSSEYRE
 * \version 0.1
 * \date 4 avril 2021
 *
 * Fichier d'entête definissant les constantes, structures et prototypes
 * nécéssaire au fonctionnement de myalloc.c
 *
 */

#ifndef MYALLOC_H_INCLUDED
#define MYALLOC_H_INCLUDED


#define MEM_SIZE 10000


typedef struct block
{
    int size;
    int free;

    char *data;

    struct block *next;
}MemBlock;

typedef struct
{
    char *buf;
    int size;
    MemBlock *first;
    MemBlock *last;
}Memory;


Memory *mem;


int initMemory(int nBytes);
void* myAlloc(int nBytes);
int myFree(void* p);
int freeMemory();

void fusionFreeBlocks();
void printAllMemory();


#endif // MYALLOC_H_INCLUDED
