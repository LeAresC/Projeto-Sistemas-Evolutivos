#define PENALIDADE_MORTE 1000000.0
#define PENALIDADE_NAO_CHEGOU 500000.0
#include <math.h>
#include <stdlib.h>
#include "Selecao.h"
#include "Crossover.h"
#include "Mutacao.h"

int determinarfitness(int *individuo, int **dist, int **mapa, int indsize, int tamanhoMapa, int dest_x, int dest_y)
{
    int x = 0;
    int y = 0;
    int custo = 0;
    int penalidade_morte = 0;
    for (int i = 0; i < indsize; i++)
    {
        if (mapa[x][y] == -1 || (x >= tamanhoMapa || x < 0) || (y >= tamanhoMapa || y < 0))
        {
            penalidade_morte = 1;
            break;
        }
        custo += mapa[x][y];
        if (individuo[i] == 1)
            x++;
        if (individuo[i] == 3)
            x--;
        if (individuo[i] == 0)
            y++;
        if (individuo[i] == 2)
            y--;
    }
    if (penalidade_morte == 1)
    {
        return PENALIDADE_MORTE + abs(dest_x - x) + abs(dest_y - y);
    }
    else if (x != dest_x || y != dest_y)
    {
        return dist[x][y];
    }
    else
    {
        return custo;
    }
}

void ExecucaoAlgoritmo(int **pop, int **mapa, int **dist, int popsize, int indsize, int tamanhoMapa, int dest_x, int dest_y, int pcr, int pmt)
{
    int *fitness = (int *)malloc(sizeof(int) * popsize);
    int **newpop = (int **)malloc(sizeof(int *) * popsize);
    for (int i = 0; i < popsize; i++)
    {
        fitness[i] = determinarfitness(pop[i], dist, indsize, mapa, tamanhoMapa, dest_x, dest_y);
    }

    int cnt = 0;
    while (cnt < popsize)
    {

        newpop[cnt] = (int *)malloc(sizeof(int) * indsize);
        if (cnt + 1 < popsize)
            newpop[cnt + 1] = (int *)malloc(sizeof(int) * indsize);

        int *ind1 = torneio(pop, popsize, fitness, indsize);
        int *ind2 = torneio(pop, popsize, fitness, indsize);
        if (rand() % 100 < pcr)
        {
            AplicarCrossover(ind1, ind2, indsize);
        }
        AplicarMutacao(ind1, pmt, indsize);
        AplicarMutacao(ind2, pmt, indsize);

        for (int i = 0; i < indsize; i++)
            newpop[cnt] = ind1[cnt];
        cnt++;
        if (cnt < popsize)
        {
            for (int i = 0; i < indsize; i++)
                newpop[cnt] = ind2[cnt];
        }
        free(ind1);
        free(ind2);
    }
    for (int i = 0; i < popsize; i++)
    {
        for (int j = 0; j < indsize; j++)
        {
            pop[i][j] = newpop[i][j];
        }
    }
    for (int i = 0; i < popsize; i++)
    {
        free(newpop[i]);
    }
    free(fitness);
    free(newpop);
}