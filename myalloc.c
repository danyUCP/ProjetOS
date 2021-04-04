/**
 * \file myalloc.c
 * \brief Définitions des fonctions
 * \author Daniel FRANCOIS et Julien VEYSSEYRE
 * \version 0.1
 * \date 4 avril 2021
 *
 * Définition de l'ensemble des fonctions nécéssaires au fonctionnement de MyAlloc
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "myalloc.h"


/**
 * C'est la déclaration du tableau qui est considéré comme notre mémoire.
 * Nous obtenons une allocation dynamique de mémoire en utilisant un tableau
 * de type char* de taille nBytes.
 *
 * Ensuite, nous initialisons un pointeur de type "MemBlock", nommé firstBlock
 * pointant vers l'adresse de départ du bloc de mémoire que nous avons
 * alloué auparavant. Ce pointeur firstBlock pointera vers le début de la
 * liste chaînée des blocs de métadonnées.
 *
 * \param nBytes Taille de la mémoire à réserver au programme
 * \return La taille de la mémoire allouée
 */
int initMemory(int nBytes)
{
    mem = (Memory*) malloc(sizeof(Memory));

    if(mem == NULL)
        return 0;

    mem->buf = (char*) malloc(nBytes);
    mem->size = nBytes;


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

/**
 * C’est dans cette fonction que se produit l’allocation d’un bloc mémoire
 * dans notre mémoire. On vérifie si la mémoire demandée est d’abord
 * disponible, si la taille demandée est insuffiante
 *
 * Nous utilisons l'algorithme First-fit pour trouver un bloc libre pour
 * allouer de la mémoire. Si nous trouvons un bloc libre qui correspond
 * exactement à la taille requise, nous n'avons pas besoin de faire le
 * fractionnement.
 *
 * \param nBytes Taille du bloc à allouer
 * \return Pointeur sur les données du bloc alloué
 */
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
    {
        printf("Echec\n\n");
        return NULL;
    }



    return (void*)allocated->data;
}

/**
 * Cette fonction est essentielle pour résoudre les problèmes de fragmentation
 * des blocs libres de mémoire. En effet, lors de la libération d’un bloc
 * mémoire, On parcours la liste chainée des blocs de mémoire afin de fusionner
 * les blocs libres successifs en supprimant le bloc de métadonnées au milieu
 * afin de palier à ce problème de fragmentation.
 */
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
            fusionFreeBlocks();
        }

        current = current->next;
        i++;
    }
}

/**
 * Ici, nous vérifions si l'adresse à laquelle le pointeur donné en argument
 * de la fonction se trouve réellement dans la plage d'adresses du tableau
 * mémoire que nous avons utilisé à cet effet. Si oui, nous définissons
 * simplement l'indicateur libre dans le bloc de métadonnées sur 1 indiquant
 * qu'il est libre et parcourons et fusionnons les blocs consécutifs qui sont
 * libres, le cas échéant.
 *
 * \param p Pointeur sur les données du bloc à libérer
 * \return La taille du bloc libéré
 */
int myFree(void* p)
{
    MemBlock *current = p - sizeof(MemBlock);
    int size = current->size;

    printf("---------- Libération : Bloc (%d bytes) ----------\n", size);

    current->free = 1;

    fusionFreeBlocks();

    return size;
}

/**
 * La fonction freeMemory a été crée, elle permet la désallocations de tous les blocs
 * de mémoire et surtout de la mémoire initialement réservée au programme.
 *
 * \return La taille de la mémoire récupérée
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

        myFree(current->data);

        current = next;
        i++;
    }

    printAllMemory();

    printf("Libération de la mémoire\n");
    free(mem->buf);
    mem->size = 0;
    mem->first = NULL;
    mem->last = NULL;

    return nBytes;
}

/**
 * Cette fonction permet afficher l'état de la mémoire avec tous ses blocs
 */
void printAllMemory()
{
    MemBlock *current = mem->first;
    int i = 1;

    printf("\n##################################################\n");
    printf("############   Résumé de la mémoire   ############\n\n");

    printf("Taille totale : %d bytes\n", mem->size);
    printf("Adresse : %p\n\n", mem->buf);

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

    printf("##################################################\n\n\n\n");
}

