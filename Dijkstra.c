#include "heap.h"
#include <stdlib.h>
#define INF 1e9

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};

int **gerarMapaDist(int tamanhoMapa, int **mapa, int ini_x, int ini_y)
{
    int **mapaDist = (int **)malloc(sizeof(int *) * tamanhoMapa);
    for (int i = 0; i < tamanhoMapa; i++)
    {
        mapaDist[i] = (int *)malloc(sizeof(int) * tamanhoMapa);
    }
    for (int i = 0; i < tamanhoMapa; i++)
    {
        for (int j = 0; j < tamanhoMapa; j++)
        {
            mapaDist[i][j] = INF;
        }
    }
    mapaDist[ini_x][ini_y] = 0;
    pq_push(ini_x, ini_y, 0);
    while (!pq_empty())
    {
        Node at = pq_pop();
        if(at.custo > mapaDist[at.x][at.y]) continue;
        for (int i = 0; i < 4; i++)
        {
            int u = dx[i];
            int v = dy[i];
            int xf = at.x + u;
            int yf = at.y + v;
            if (xf < tamanhoMapa && xf >= 0 && yf < tamanhoMapa && yf >= 0 && mapa[xf][yf] != -1)
            {
                if (mapa[xf][yf] + at.custo < mapaDist[xf][yf])
                {
                    pq_push(xf, yf, mapaDist[xf][yf] = at.custo + mapa[xf][yf]);
                }
            }
        }
    }
    return mapaDist;
}
