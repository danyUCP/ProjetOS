/**
 * \file demo.c
 * \brief Programme de démonstration
 * \author Daniel FRANCOIS et Julien VEYSSEYRE
 * \version 0.1
 * \date 4 avril 2021
 *
 * Programme de démonstrations effectuant quelques allocations
 * et désallocations
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "myalloc.h"


int main()
{
    //Initial memory allocation (10000 bytes)
    int size = initMemory(MEM_SIZE);
    printf("Memoire totale allouée : %d bytes\n\n", size);

    printAllMemory();

    //4 successful allocations
    char *mb1 = (char*) myAlloc(2000);
    char *mb2 = (char*) myAlloc(3000);
    char *mb3 = (char*) myAlloc(800);
    char *mb4 = (char*) myAlloc(2800);
    printAllMemory();

    //Failed allocation : overflow
    myAlloc(2100);
    printAllMemory();

    //2 successful free memory with a fusion
    myFree(mb2);
    myFree(mb3);
    printAllMemory();

    //1 allocation and 1 free with 2 fusions
    char *mb5 = (char*) myAlloc(1000);
    myFree(mb4);
    printAllMemory();

    //Final free memory
    myFree(mb1);
    myFree(mb5);
    freeMemory();
    printAllMemory();

    free(mem);

    return 0;
}

