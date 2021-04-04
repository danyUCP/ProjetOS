#include <stdio.h>
#include <stdlib.h>


#define MEM_SIZE 1000


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


    //MemBlock *firstBlock = (MemBlock*) malloc(sizeof(MemBlock));
    MemBlock *firstBlock = (MemBlock*) mem->buf;
    firstBlock->size = mem->size - sizeof(MemBlock);
    firstBlock->free = 1;
    firstBlock->data = (char*) ((void*)firstBlock + sizeof(MemBlock));
    firstBlock->next = NULL;


    mem->first = firstBlock;
    mem->last = NULL;

    printf("Mémoire initialisée\n\n");

    return mem->size;
}


void* myAlloc(int nBytes)
{
    if(nBytes < 1 || nBytes > mem->size)
    {
        printf("Erreur de taille demandée\n");
        return NULL;
    }

    MemBlock *current = mem->first;

    divideFreeBlock(current, nBytes);


}

void divideFreeBlock(MemBlock *freeBlock, int size)
{
    MemBlock *newFreeBlock = (void*)freeBlock + size + sizeof(MemBlock);
    newFreeBlock->size = freeBlock->size - size - sizeof(MemBlock);
    newFreeBlock->free = 1;
    newFreeBlock->data = (char*) ((void*)newFreeBlock + sizeof(MemBlock));
    newFreeBlock->next = freeBlock->next;

    freeBlock->size = size;
    freeBlock->free = 0;
    freeBlock->next = newFreeBlock;

}

/*
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
        //free(current);

        current = next;
        i++;
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

    printf("Taille totale : %d bytes\n", mem->size);
    printf("Adresse : %u\n\n", mem->buf);
    //printf("************************************************\n");
    if(current == NULL)
        printf("Mémoire vide\n");
    while(current != NULL)
    {
        printf("----- Bloc mémoire %d -----\n", i);
        printf("Taille : %d bytes\n", current->size);
        printf("Adresse relative : %u | Adresse absolue : %u\n", current->data - mem->buf, current->data);
        printf("Alloué : ");
        if(current->free)
            printf("NON\n");
        else
            printf("OUI\n");

        printf("\n");
        current = current->next;
        i++;
    }
    printf("\n******************************************\n\n\n\n");

}


int main()
{
    int size = initMemory(MEM_SIZE);
    printf("Memoire allouée : %d bytes\n\n", size);

    printAllMemory();


    //int sizeFree = freeMemory();
    //printf("Free size : %d\n\n", sizeFree);

    myAlloc(200);
    printAllMemory();

    /*
    printf("MA : %u\n\n", mem->buf);
    printf("MF : %u\n\n", mem->first->data);

    printf("MA : %u\n\n", mem->first->data - mem->buf);

    */

    freeMemory();

    printAllMemory();

    free(mem);

    return 0;
}
