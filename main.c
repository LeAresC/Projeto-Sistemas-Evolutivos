#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mapa.h"       // Se tiver
#include "Dijkstra.h"
#include "Evolucao.h"

#define POPSIZE 200     // Tamanho da população
#define INDSIZE 100     // Tamanho do indivíduo (Combustível suficiente)
#define NGEN 150        // Número de gerações
#define PCR 80          // Probabilidade de crossover
#define PMT 5           // Probabilidade de mutação
#define TAMANHOMAPA 10
#define NUM_TENTATIVAS 10 // Quantas vezes vamos rodar do zero

// Função auxiliar para imprimir o melhor caminho no final
void ImprimirMelhorCaminho(int *ind, int tamanho, int **mapa) {
    printf("\n--- MELHOR CAMINHO ENCONTRADO (GLOBAL) ---\n");
    int x = 0, y = 0;
    printf("(0,0)");
    
    for(int i = 0; i < tamanho; i++) {
        // Simula o movimento para mostrar na tela
        if (ind[i] == 0) x--;      // Cima
        else if (ind[i] == 1) y++; // Dir
        else if (ind[i] == 2) x++; // Baixo
        else if (ind[i] == 3) y--; // Esq
        
        // Verifica limites só para não printar lixo
        if(x >= 0 && x < TAMANHOMAPA && y >= 0 && y < TAMANHOMAPA) {
            printf(" -> (%d,%d)", x, y);
            if(x == TAMANHOMAPA-1 && y == TAMANHOMAPA-1) {
                printf(" [CHEGOU!]");
                break;
            }
        }
    }
    printf("\n");
}

// Inicialização Heurística (Viés para Direita/Baixo)
int *GerarIndividuo(int TamInd)
{
    int *ind_ = (int *)malloc(sizeof(int) * TamInd);
    for (int i = 0; i < TamInd; i++)
    {
        int r = rand() % 100;
        
        // Configuração para alvo no Sudeste (Baixo/Direita)
        if (r < 40) ind_[i] = 1;      // 40% Chance DIREITA
        else if (r < 80) ind_[i] = 2; // 40% Chance BAIXO
        else if (r < 90) ind_[i] = 0; // 10% Chance CIMA
        else ind_[i] = 3;             // 10% Chance ESQUERDA
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
    srand(time(NULL));

    // --- FASE 1: SETUP DO AMBIENTE (Executa 1 vez) ---
    
    // Aloca e Lê Mapa
    int **mapa = (int **)malloc(sizeof(int*) * TAMANHOMAPA);
    for (int i = 0; i < TAMANHOMAPA; i++)
    {
        mapa[i] = (int *)malloc(sizeof(int) * TAMANHOMAPA);
        for (int j = 0; j < TAMANHOMAPA; j++)
        {
            scanf("%d", &mapa[i][j]);
        }
    }

    // Configura Destino e Dijkstra
    int dest_x = TAMANHOMAPA - 1;
    int dest_y = TAMANHOMAPA - 1;
    int **dist = gerarMapaDist(TAMANHOMAPA, mapa, dest_x, dest_y);

    // Variáveis para guardar o MELHOR DE TODOS (Recordista Global)
    int MelhorFitnessGlobal = 1e9;
    int *MelhorIndividuoGlobal = (int *)malloc(sizeof(int) * INDSIZE);


    // --- FASE 2: LOOP DE TENTATIVAS (Multi-Start) ---
    
    for (int tentativa = 0; tentativa < NUM_TENTATIVAS; tentativa++) 
    {
        printf("\n=== TENTATIVA %d de %d ===\n", tentativa + 1, NUM_TENTATIVAS);

        // 1. Inicializa População (Nova para cada tentativa)
        int **pop = GerarPopulacao(POPSIZE, INDSIZE);
        int taxa_atual = PMT;

        // 2. Loop Evolutivo
        for (int i = 0; i < NGEN; i++)
        {
            // Opcional: Não printar todas as gerações para não poluir o console, 
            // ou printar só a cada 50 gerações
            // if (i % 50 == 0) printf("."); 
            
            ExecucaoAlgoritmo(pop, mapa, dist, POPSIZE, INDSIZE, TAMANHOMAPA, dest_x, dest_y, PCR, taxa_atual);
        }
        
        // 3. Encontrar o melhor desta tentativa específica
        int melhor_fit_local = 1e9;
        int id_melhor_local = 0;
        
        for(int i=0; i<POPSIZE; i++) {
            int f = determinarfitness(pop[i], mapa, dist, INDSIZE, TAMANHOMAPA, dest_x, dest_y);
            if(f < melhor_fit_local) {
                melhor_fit_local = f;
                id_melhor_local = i;
            }
        }

        printf("\nMelhor desta tentativa: %d\n", melhor_fit_local);

        // 4. Comparar com o Recorde Mundial
        if (melhor_fit_local < MelhorFitnessGlobal) {
            printf(">>> NOVO RECORDE GLOBAL! Atualizando de %d para %d\n", MelhorFitnessGlobal, melhor_fit_local);
            MelhorFitnessGlobal = melhor_fit_local;
            
            // Salva os genes (copia manual pois o ponteiro pop vai ser liberado)
            for(int k=0; k<INDSIZE; k++) {
                MelhorIndividuoGlobal[k] = pop[id_melhor_local][k];
            }
        }

        // 5. Limpeza da População desta tentativa
        for (int i = 0; i < POPSIZE; i++) free(pop[i]);
        free(pop);
    }


    // --- FASE 3: RESULTADOS E LIMPEZA FINAL ---

    printf("\n\n==========================================\n");
    printf("RESULTADO FINAL APÓS %d TENTATIVAS:\n", NUM_TENTATIVAS);
    printf("Melhor Fitness Global = %d\n", MelhorFitnessGlobal);
    
    ImprimirMelhorCaminho(MelhorIndividuoGlobal, INDSIZE, mapa);

    // Limpeza de estruturas estáticas
    free(MelhorIndividuoGlobal);

    for (int i = 0; i < TAMANHOMAPA; i++) {
        free(mapa[i]);
        free(dist[i]);
    }
    free(mapa);
    free(dist);

    return 0;
}