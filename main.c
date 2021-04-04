#include <stdio.h>
#include <stdlib.h>

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

int initMemory(int nBytes)
{
    mem = (Memory*) malloc(sizeof(Memory));

    if(mem == NULL)
        return 0;

    mem->buf = (char*) malloc(nBytes);
    mem->size = nBytes;


    MemBlock *firstBlock = (MemBlock*) malloc(sizeof(MemBlock));
    firstBlock->size = mem->size;
    firstBlock->free = 1;
    firstBlock->data = mem->buf;
    firstBlock->next = NULL;


    mem->first = firstBlock;
    mem->last = NULL;

    printf("Mémoire initialisée\n\n");

    return mem->size;
}

/*
void* myAlloc(int nBytes)
{

}

int myFree(void* p)
{

}
*/

int freeMemory()
{
    MemBlock *current = mem->first, *next;
    int nBytes = mem->size, i = 1;

    if(nBytes == 0)
        return -1;

    while(current != NULL)
    {
        next = current->next;

        printf("Nettoyage du bloc %d (%d bytes)\n", i, current->size);
        free(current);

        current = next;
    }


    printf("Libération de la mémoire\n\n");
    free(mem->buf);
    mem->size = 0;
    mem->first = NULL;
    mem->last = NULL;

    return nBytes;
}

void printAllMemory()
{
    MemBlock *current = mem->first;
    int i = 1;

    printf("---------- Résumé de la mémoire ----------\n\n");

    printf("Taille totale : %d\n", mem->size);
    printf("Adresse : %p\n\n", mem->buf);
    //printf("************************************************\n");
    if(current == NULL)
        printf("Mémoire vide\n");
    while(current != NULL)
    {
        printf("----- Bloc mémoire %d -----\n", i);
        printf("Taille : %d\n", current->size);
        printf("Adresse : %p\n", current->data);
        printf("Alloué : ");
        if(current->free)
            printf("NON\n");
        else
            printf("OUI\n");

        current = current->next;
        i++;
    }
    printf("\n******************************************\n\n");

}


int main()
{
    int size = initMemory(MEM_SIZE);
    printf("Memoire allouée : %d bytes\n\n", size);

    printAllMemory();


    //int sizeFree = freeMemory();
    //printf("Free size : %d\n\n", sizeFree);


    freeMemory();

    printAllMemory();

    free(mem);

    return 0;
}
