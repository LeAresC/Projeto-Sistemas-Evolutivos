int *torneio(int **pop, int popsize, int *fitness, int indsize)
{
    int index_1 = rand() % popsize;
    int index_2 = rand() % popsize;
    int *ret = (int *)malloc(sizeof(int) * indsize);
    if (fitness[index_1] < fitness[index_2])
        for (int i = 0; i < indsize; i++)
        {
            ret[i] = pop[index_1][i];
        }
    else
        for (int i = 0; i < indsize; i++)
        {
            ret[i] = pop[index_2][i];
        }
    return ret;
}
