#include <stdlib.h>
void AplicarMutacao(int *individuo, int pmt, int indsize)
{
    for(int i = 0; i < indsize; i++)
    {
        if(rand() % 100 < pmt)
        {
            individuo[i] = rand()%4;
        }
    }
}