#define PENALIDADE_MORTE 1000000
#define PENALIDADE_NAO_CHEGOU 500000
#define JANELA_ESTAGNACAO 20
#define TAXA_CHOQUE 30
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "Selecao.h"
#include "Crossover.h"
#include "Mutacao.h"

int MELHOR_FITNESS_TOTAL = 1e9;
int ESTAGNADO = 0;

// --- CORREÇÃO 1: Lógica de Fitness Segura ---
int determinarfitness(int *individuo, int **mapa, int **dist, int indsize, int tamanhoMapa, int dest_x, int dest_y)
{
    int x = 0; // Ponto de partida
    int y = 0;
    int custo = 0;
    int penalidade = 0;

    // Opcional: Se a posição (0,0) tiver custo, comece somando ela
    // custo += mapa[0][0];

    for (int i = 0; i < indsize; i++)
    {
        // 1. Calcula a POSIÇÃO FUTURA
        int next_x = x;
        int next_y = y;

        if (individuo[i] == 0)
            next_x--; // Cima (Verifique se seu mapa é x=linha ou x=coluna)
        else if (individuo[i] == 1)
            next_y++; // Dir
        else if (individuo[i] == 2)
            next_x++; // Baixo
        else if (individuo[i] == 3)
            next_y--; // Esq

        // 2. Verifica se a Posição Futura é Válida (Antes de mover!)
        if (next_x < 0 || next_x >= tamanhoMapa || next_y < 0 || next_y >= tamanhoMapa || mapa[next_x][next_y] == -1)
        {
            penalidade = 1; // Bateu!
            break;          // Para imediatamente
        }

        // 3. Se é válido, Move e Soma
        x = next_x;
        y = next_y;
        custo += mapa[x][y];

        // 4. Checou vitória
        if (x == dest_x && y == dest_y)
            return custo; // Sucesso total! Retorna só o custo.
    }

    // Se saiu do loop, não chegou. Aplica penalidades.
    if (penalidade)
    {
        // Bateu na parede: Penalidade Morte + Distancia Manhattan de onde morreu
        return PENALIDADE_MORTE + abs(dest_x - x) + abs(dest_y - y);
    }
    else
    {
        // Acabaram os passos: Penalidade Falha + Distancia Real (Dijkstra)
        return PENALIDADE_NAO_CHEGOU + dist[x][y];
    }
}

// --- CORREÇÃO 2: Gestão de Memória e Fluxo de Geração ---
void ExecucaoAlgoritmo(int **pop, int **mapa, int **dist, int popsize, int indsize, int tamanhoMapa, int dest_x, int dest_y, int pcr, int pmt)
{
    int *fitness = (int *)malloc(sizeof(int) * popsize);

    // Aloca estrutura temporária para a nova população
    int **newpop = (int **)malloc(sizeof(int *) * popsize);
    for (int i = 0; i < popsize; i++)
    {
        newpop[i] = (int *)malloc(sizeof(int) * indsize);
    }

    // 1. Avaliação e Elitismo
    int melhor_fitness = 1e9 + 7;
    int melhor_indice = 0;
    #pragma omp parallel for
    for (int i = 0; i < popsize; i++)
    {
        // Atenção à ordem dos parametros: mapa, depois dist
        fitness[i] = determinarfitness(pop[i], mapa, dist, indsize, tamanhoMapa, dest_x, dest_y);

        if (fitness[i] < melhor_fitness)
        {
            melhor_indice = i;
            melhor_fitness = fitness[i];
        }
    }
    if (melhor_fitness == MELHOR_FITNESS_TOTAL)
    {
        ESTAGNADO++;
    }
    else
    {
        ESTAGNADO = 0;
        MELHOR_FITNESS_TOTAL = melhor_fitness;
    }
    if (ESTAGNADO >= JANELA_ESTAGNACAO)
    {
        pmt = TAXA_CHOQUE;  
        ESTAGNADO = 0; 
    }
    printf("Melhor Fitness: %d\n", melhor_fitness); // Debug útil

    // Elitismo: Copia o melhor indivíduo para a posição 0 da nova pop
    for (int j = 0; j < indsize; j++)
    {
        newpop[0][j] = pop[melhor_indice][j];
    }

    // 2. Loop de Reprodução (Começa do 1, pois 0 é a elite)
    int cnt = 1;
    while (cnt < popsize)
    {
        // Seleciona Ponteiros para os Pais (NÃO dar free neles!)
        int *pai1 = torneio(pop, popsize, fitness, indsize);
        int *pai2 = torneio(pop, popsize, fitness, indsize);

        // Copia genes dos pais para os buffers dos filhos em newpop
        // (Isso garante que não alteramos os pais originais)
        for (int k = 0; k < indsize; k++)
        {
            newpop[cnt][k] = pai1[k];
            if (cnt + 1 < popsize)
            { // Proteção se popsize for ímpar
                newpop[cnt + 1][k] = pai2[k];
            }
        }

        // Aplica Crossover (Diretamente na memória dos filhos em newpop)
        if (rand() % 100 < pcr && cnt + 1 < popsize)
        {
            // Sua função AplicarCrossover deve ler de newpop[cnt] e newpop[cnt+1]
            // e misturar os genes ali mesmo.
            AplicarCrossover(newpop[cnt], newpop[cnt + 1], indsize);
        }

        // Aplica Mutação (Nos filhos)
        AplicarMutacao(newpop[cnt], pmt, indsize);
        if (cnt + 1 < popsize)
        {
            AplicarMutacao(newpop[cnt + 1], pmt, indsize);
        }

        cnt += 2; // Avança 2 posições
    }

    // 3. Atualização da População Principal
    for (int i = 0; i < popsize; i++)
    {
        for (int j = 0; j < indsize; j++)
        {
            pop[i][j] = newpop[i][j];
        }
        free(newpop[i]); // Libera a linha auxiliar
    }

    free(newpop); // Libera o vetor de ponteiros
    free(fitness);
}