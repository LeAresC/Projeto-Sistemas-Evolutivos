#include <stdlib.h>

// Retorna o PONTEIRO para o vencedor dentro da população original.
// Não aloca memória nova.
int *torneio(int **pop, int popsize, int *fitness, int indsize)
{
    int index_1 = rand() % popsize;
    int index_2 = rand() % popsize;

    // Removemos o malloc e o loop de cópia.
    // Apenas retornamos o endereço de quem ganhou.
    
    // Problema de Minimização (Menor fitness ganha)
    if (fitness[index_1] < fitness[index_2])
    {
        return pop[index_1]; 
    }
    else
    {
        return pop[index_2];
    }
}