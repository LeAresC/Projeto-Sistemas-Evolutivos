#ifndef INTERFACE_H
#define INTERFACE_H

#include <GL/glut.h>

// --- Constantes Visuais ---
#define LARGURA_JANELA 800
#define ALTURA_JANELA 800
#define AREA_MAPA 600 // Pixels dedicados ao mapa

// --- Protótipos (Funções que o main.c precisa enxergar) ---

// Configurações iniciais do OpenGL e dados
void init(int** mapa, int ordem);

// Função de desenho principal (Callback do GLUT)
void display(int ordem);

// Função de clique do mouse (Callback do GLUT)
void mouse(int button, int state, int x, int y, int ordem);

// Atualiza o caminho que deve ser desenhado na tela
void setCaminhoVisualizacao(int *genes, int tamanhoIndividuo);

// Função de Timer para animar
void timer(int value);

// Inicia a simulação (chamada pelo botão INICIAR)
void iniciarSimulacao(void);

// Registra fitness do melhor e média da população em cada geração
void registrarFitnessGeracao(int fitnessMelhor, int fitnessMedia);

// Reseta o histórico de fitness (chamado ao iniciar nova simulação)
void resetarHistoricoFitness(void);

// Callback de reshape para bloquear redimensionamento
void reshape(int w, int h);

#endif // INTERFACE_H