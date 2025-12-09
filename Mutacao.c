#include <stdlib.h>
#include <math.h>
void AplicarMutacao(int *individuo, int pmt, int indsize)
{
    // Percorre o genoma
    for(int i = 0; i < indsize; i++)
    {
        // Se cair na probabilidade de mutação (ex: 2% a 5%)
        if(rand() % 100 < pmt)
        {
            // Sorteia o tamanho do "surto" (ex: entre 3 e 10 passos)
            int tamanho_bloco = 3 + (rand() % 8); 
            
            // Sorteia UMA nova direção para tentar sair da armadilha
            // (Aqui é melhor ser aleatório puro, sem viés, para permitir exploração)
            int nova_direcao = rand() % 4; 

            // Aplica a nova direção repetidamente
            for (int k = 0; k < tamanho_bloco; k++) {
                if (i + k < indsize) {
                    individuo[i + k] = nova_direcao;
                }
            }
            
            // Avança o índice 'i' para não mutar imediatamente os genes que acabamos de alterar
            i += tamanho_bloco;
        }
    }
}