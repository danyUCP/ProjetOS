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
    MemBlock *allocated = NULL;
    int i = 1;

    printf("========== Début allocation ==========\n\n");

    printf("Espace à allouer : %d bytes\n\n", nBytes);
    while(current != NULL)
    {
        if(!current->free)
        {
            printf("Bloc %d (%d bytes) : Déjà alloué\n", i, current->size);
            current = current->next;
            i++;
        }
        else
        {
            if(current->size < nBytes)
            {
                printf("Bloc %d (%d bytes) : Taille insuffisante\n", i, current->size);
                current = current->next;
                i++;
            }
            else if(current->size > nBytes + sizeof(MemBlock))
            {
                printf("Bloc %d (%d bytes) : Division du bloc mémoire\n", i, current->size);

                MemBlock *newFreeBlock = (void*)current + nBytes + sizeof(MemBlock);
                newFreeBlock->size = current->size - nBytes - sizeof(MemBlock);
                newFreeBlock->free = 1;
                newFreeBlock->data = (char*) ((void*)newFreeBlock + sizeof(MemBlock));
                newFreeBlock->next = current->next;

                current->size = nBytes;
                current->free = 0;
                current->next = newFreeBlock;

                allocated = current;
                break;
            }
            else
            {
                printf("Bloc %d (%d bytes) : Taille de bloc parfaite\n", i, current->size);

                current->size = nBytes;
                current->free = 0;

                allocated = current;
                break;
            }
        }
    }

    printf("\n=========== Fin allocation ===========\n");
    if(allocated != NULL)
        printf("Succès\n\n");
    else
        printf("Echec\n\n");


    return (void*)allocated;
}

void fusionFreeBlocks()
{
    MemBlock *current = mem->first;
    int i = 1;

    while(current != NULL && current->next != NULL)
    {
        if(current->free && current->next->free)
        {
            printf("Fusion du bloc %d et %d (%d + %d + %d bytes)\n", i, i + 1, current->size, current->next->size, (int)sizeof(MemBlock));
            current->size += current->next->size + sizeof(MemBlock);
            current->next = current->next->next;
        }

        current = current->next;
        i++;
    }
}


int myFree(void* p)
{
    MemBlock *current = p;

    printf("Bloc (%d bytes)\n", mem->first);


}


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

    printf("\n---------- Résumé de la mémoire ----------\n\n");

    printf("Taille totale : %d bytes\n", mem->size);
    printf("Adresse : %p\n\n", mem->buf);
    //printf("************************************************\n");
    if(current == NULL)
        printf("Mémoire vide\n");
    while(current != NULL)
    {
        printf("----- Bloc mémoire %d -----\n", i);
        printf("Taille : %d bytes\n", current->size);
        printf("Adresse relative : %u | Adresse absolue : %p\n", (int)(current->data - mem->buf), current->data);
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

    MemBlock *mb1 = myAlloc(300);
    printAllMemory();

    MemBlock *mb2 = myAlloc(80);
    printAllMemory();

    myAlloc(280);
    printAllMemory();

    myAlloc(21);
    printAllMemory();

    myFree(mb1);
    myFree(mb2);


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
