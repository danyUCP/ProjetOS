#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "myalloc.h"


void modeI();

int main(int argc, char *argv[])
{
    int opt = 0;

    if(argc < 2 || argc > 3)
        return 0;


    while((opt = getopt(argc, argv, "ic:f:")) != EOF)
    {
        switch(opt)
        {
            case 'i':
                modeI();
                break;
            case 'c':

                break;
            case 'f':

                break;
        }
    }



    printf("\n\nFin du programme MyAlloc\n\n");

    return 0;
}

void modeI()
{
    int choice = 0, size = 0, stop = 0, stop2 = 0;

    printf("Bienvenue dans le mode interactif de MyAlloc\n\n");


    while(!stop)
    {
        do
        {
            printf("\nCommencer par sélectionner la taille totale de mémoire à allouer :\n");
            printf("1. 5000 bytes\n");
            printf("2. 10000 bytes\n");
            printf("3. 15000 bytes\n");
            printf("4. 20000 bytes\n");
            printf("Entrez 0 pour quitter\n\n");
            printf("Choix : ");
            scanf("%d", &choice);
        }while(choice < 0 || choice > 4);

        switch(choice)
        {
            case 0:
                size = 0;
                break;
            case 1:
                size = 5000;
                break;
            case 2:
                size = 10000;
                break;
            case 3:
                size = 15000;
                break;
            case 4:
                size = 20000;
                break;
            default:
                size = MEM_SIZE;
                break;
        }

        if(size == 0)
            break;

        system("clear");
        initMemory(size);
        printf("Memoire totale allouée : %d bytes\n\n", size);
        printAllMemory();

        do
        {

            do
            {
                printf("\nChoisissez une action :\n");
                printf("1. Allouer un bloc mémoire\n");
                printf("2. Libérer un bloc mémoire\n");
                printf("Entrez 0 pour quitter et libérer la mémoire\n\n");
                printf("Choix : ");
                scanf("%d", &choice);
            }while(choice < 0 || choice > 2);


            switch(choice)
            {
                case 0:
                    stop2 = 1;
                    break;
                case 1:
                    printf("\nSaisissez la taille en bytes à allouer : ");
                    scanf("%d", &choice);
                    system("clear");

                    myAlloc(choice);
                    printAllMemory();
                    break;
                case 2:
                    printf("\nChoisissez le numéro du bloc à libérer : ");
                    scanf("%d", &choice);
                    system("clear");

                    MemBlock *current = mem->first;
                    int i = 1;
                    while(i < choice && current != NULL)
                    {
                        current = current->next;
                        i++;
                    }

                    if(current == NULL)
                    {
                        printf("\nSaisie incorrecte du bloc\n");
                        break;
                    }

                    myFree(current->data);
                    printAllMemory();
                    break;
            }

        }while(!stop2);


        freeMemory();
        printAllMemory();
        free(mem);
    }
}
