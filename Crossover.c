#include <stdlib.h>

void AplicarCrossover(int *pai1, int *pai2, int indsize)
{
    int *filho1 = (int *)malloc(sizeof(int) * indsize);
    int *filho2 = (int *)malloc(sizeof(int) * indsize);
    int ponto1 = rand() % indsize;
    int ponto2 = rand() % indsize;
    if (ponto1 > ponto2)
    {
        int temp = ponto1;
        ponto1 = ponto2;
        ponto2 = temp;
    }

    for (int i = 0; i < indsize; i++)
    {
        if(i < ponto1 || i >= ponto2)
        {
            filho1[i] = pai1[i];
            filho2[i] = pai2[i];
        }
        else
        {
            filho1[i] = pai2[i];
            filho2[i] = pai1[i];
        }
    }
    for(int i = 0; i < indsize; i++)
    {
        pai1[i] = filho1[i];
        pai2[i] = filho2[i];
    }
    free(filho1);
    free(filho2);
}
