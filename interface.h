#ifndef INTERFACE_H
#define INTERFACE_H

#include <GL/glut.h>

// --- Constantes Visuais ---
#define LARGURA_JANELA 800
#define ALTURA_JANELA 800
#define TAM_MAPA 50
#define AREA_MAPA 600 // Pixels dedicados ao mapa

// --- Protótipos (Funções que o main.c precisa enxergar) ---

// Configurações iniciais do OpenGL e dados
void init();

// Função de desenho principal (Callback do GLUT)
void display();

// Função de clique do mouse (Callback do GLUT)
void mouse(int button, int state, int x, int y);

#endif // INTERFACE_H