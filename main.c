#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "Dijkstra.h"
#include "Evolucao.h"
#define POPSIZE 1000 // Tamanho da população
#define INDSIZE 15  // Tamanho do indivíduo
#define NGEN 1   // Número de gerações
#define PCR 50      // Probabilidade de crossover
#define PMT 2       // Probabilidade de mutação
#define TAMANHOMAPA 5

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
    int **mapa = malloc(sizeof(int*) * 5);
    for (int i = 0; i < 5; i++)
    {
        mapa[i] = malloc(sizeof(int)*5);
        for (int j = 0; j < 5; j++)
        {
            scanf("%d", &mapa[i][j]);
        }
    }
    int dest_x = TAMANHOMAPA - 1;
    int dest_y = TAMANHOMAPA - 1;
    int **dist = gerarMapaDist(TAMANHOMAPA, mapa, dest_x, dest_y);
    int mn = 1e9;
    for (int i = 0; i < NGEN; i++)
    {
        ExecucaoAlgoritmo(pop, mapa, dist, POPSIZE, INDSIZE, TAMANHOMAPA, dest_x, dest_y, PCR, PMT);
        for (int j = 0; j < POPSIZE; j++)
        {
            int aux = determinarfitness(pop[j], dist, mapa, INDSIZE, TAMANHOMAPA, dest_x, dest_y);
            if(aux < mn){
                mn = aux;
            }
        }
    }
    printf("%d\n", mn);
    for (int i = 0; i < POPSIZE; i++)
    {
        free(pop[i]);
    }
    free(pop);
    for (int i = 0; i < TAMANHOMAPA; i++)
    {
        free(mapa[i]);
    }
    free(mapa);
    return 0;
}
