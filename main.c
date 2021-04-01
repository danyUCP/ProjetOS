#include <stdio.h>
#include <stdlib.h>

#define MEM_SIZE 10000

typedef struct block
{
    size_t size;
    int free;

    char *data;

    struct block *next;
}MemBlock;


int main()
{
    char tab[10] = "abcdef";

    printf("%c : %d\n", tab[0], &tab[0]);
    printf("%c : %d\n", tab[1], &tab[1]);
    printf("%c : %d\n", tab[2], &tab[2]);
    printf("%c : %d\n", tab[3], &tab[3]);
    printf("%c : %d\n", tab[4], &tab[4]);
    printf("%c : %d\n", tab[5], &tab[5]);
    printf("Char size : %d\n\n", sizeof(char));

    int tab2[10] = {1, 2, 3, 4, 5, 6};

    printf("%d : %d\n", tab2[0], &tab2[0]);
    printf("%d : %d\n", tab2[1], &tab2[1]);
    printf("%d : %d\n", tab2[2], &tab2[2]);
    printf("%d : %d\n", tab2[3], &tab2[3]);
    printf("%d : %d\n", tab2[4], &tab2[4]);
    printf("%d : %d\n", tab2[5], &tab2[5]);
    printf("Int size : %d\n\n", sizeof(int));


    MemBlock m[2];

    printf("MemBlock size : %d\n\n", sizeof(m));


    return 0;
}
