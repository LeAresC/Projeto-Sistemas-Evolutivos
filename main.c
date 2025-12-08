#include <stdio.h>
#include <stdlib.h>

#define POPSIZE 100 // Tamanho da população
#define INDSIZE 20  // Tamanho do indivíduo
#define NGEN 100    // Número de gerações
#define PCR 5       // Probabilidade de crossover
#define PMT 2       // Probabilidade de mutação

int *GerarIndividuo(int TamInd)
{
    int *ind_ = (int *)malloc(sizeof(int) * TamInd);
    for (int i = 0; i < TamInd; i++)
    {
        ind_[i] = (rand() % 4); // 0 - CIMA , 1 - DIREITA , 2 - BAIXO, 3 - ESQUERDA
    }
    return ind_;
}

int **GerarPopulacao(int TamPop, int TamInd)
{
    int **pop_ = (int **)malloc(sizeof(int *) * TamPop);
    for (int i = 0; i < TamPop; i++)
    {
        pop_[i] = GerarIndividuo(TamInd);
    }
    return pop_;
}

int main()
{
    int **pop = GerarPopulacao(POPSIZE, INDSIZE);
    for (int i = 0; i < POPSIZE; i++)
    {
        for (int j = 0; j < INDSIZE; j++)
        {
            printf("%d ", pop[i][j]);
        }
        printf("\n");
    }
    return 0;
}
