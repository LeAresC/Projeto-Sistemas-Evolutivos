#define PENALIDADE_MORTE 1000000
#define PENALIDADE_NAO_CHEGOU 500000
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
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
        if ((x >= tamanhoMapa || x < 0) || (y >= tamanhoMapa || y < 0) || mapa[x][y] == -1)
        {
            penalidade_morte = 1;
            break;
        }
        custo += mapa[x][y];
        if (x == dest_x && y == dest_y)
            break;
        if (individuo[i] == 1)
            y++;
        if (individuo[i] == 3)
            y--;
        if (individuo[i] == 0)
            x--;
        if (individuo[i] == 2)
            x++;
    }
    if ((x >= tamanhoMapa || x < 0) || (y >= tamanhoMapa || y < 0) || mapa[x][y] == -1)
    {
        penalidade_morte = 1;
    }
    if (penalidade_morte == 1)
    {
        return PENALIDADE_MORTE + abs(dest_x - x) + abs(dest_y - y);
    }
    else if (x != dest_x || y != dest_y)
    {
        return PENALIDADE_NAO_CHEGOU + dist[x][y];
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

    int melhor_fitness = 1e9;
    int melhor_indice = 0;
    for (int i = 0; i < popsize; i++)
    {
        fitness[i] = determinarfitness(pop[i], dist, mapa, indsize, tamanhoMapa, dest_x, dest_y);
        if(fitness[i] < melhor_fitness)
        {
            melhor_indice = i;
            melhor_fitness = fitness[i];
        }
    }
    newpop[0] = (int *)malloc(sizeof(int) * indsize);
    for(int i = 0; i < indsize; i++)
    {
        newpop[0][i] = pop[melhor_indice][i];
    }
    int cnt = 1;
    while (cnt < popsize)
    {
        int *ind1 = torneio(pop, popsize, fitness, indsize);
        int *ind2 = torneio(pop, popsize, fitness, indsize);
        if (rand() % 100 < pcr)
        {
            AplicarCrossover(ind1, ind2, indsize);
        }
        AplicarMutacao(ind1, pmt, indsize);
        AplicarMutacao(ind2, pmt, indsize);

        newpop[cnt] = (int *)malloc(sizeof(int) * indsize);
        for (int i = 0; i < indsize; i++)
            newpop[cnt][i] = ind1[i];
        cnt++;
        if (cnt < popsize)
        {
            newpop[cnt] = (int *)malloc(sizeof(int) * indsize);
            for (int i = 0; i < indsize; i++)
                newpop[cnt][i] = ind2[i];
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
    int mn = 1e9;
    for (int j = 0; j < popsize; j++)
    {
        int aux = fitness[j];
        if (aux < mn)
        {
            mn = aux;
        }
    }
    printf("%d\n", mn);
    for (int i = 0; i < popsize; i++)
    {
        free(newpop[i]);
    }
    free(fitness);
    free(newpop);
}
