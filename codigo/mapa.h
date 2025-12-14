#ifndef MAPA_H
#define MAPA_H

#include <stdlib.h>  // para size_t, rand
#include <stdio.h>   // para printf
#include <math.h>    // para abs

// Cria uma matriz quadrada de ordem "ordem" inicializada com zeros
int** criaMapaZero(int ordem);

// Preenche o mapa com valores aleatórios de 1 a 10 (preserva obstáculos se desejar)
void randomizaMapa(int** mapa, int ordem);

// Cria um bloco de luz no mapa, centralizado em (coordx, coordy), com tamanho ordemBloco
void focoDeLuz(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy);

// Cria obstáculos no mapa em forma de linha ou diagonal
// direcao: 0=vertical, 1=horizontal, 2=diagonal principal, 3=diagonal inversa
void obstaculoBarreira(int** mapa, int ordem, int tamanho, int coordx, int coordy, int direcao);

// Cria um bloco de obstáculos de tamanho ordemBloco x ordemBloco
void obstaculoBloco(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy);

// Cria um obstáculo simples em uma posição
void obstaculo(int** mapa, int ordem, int i, int j);

// Aplica um bloco de sombra, reduzindo pela metade os valores dentro do bloco
void blocoSombra(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy);

// Gera um caminho ótimo em zig-zag (tipo L com variação) entre src e dest
void gerarOtimoZigZag(int** mapa, int ordem, int srcCoordX, int srcCoordY, int destCoordX, int destCoordY);

//Preenche um bloco com luz 
void blocoLuz(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy, int intensidade, int respeitaObst);

#endif // MAPA_H