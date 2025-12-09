#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "mapa.h"
#include "Dijkstra.h"
#include "Evolucao.h"
#include "interface.h"

#define POPSIZE 500     // Tamanho da população
#define INDSIZE 50     // Tamanho do indivíduo 
#define NGEN 1000        // Número de gerações
#define PCR 80          // Probabilidade de crossover
#define PMT 3           // Probabilidade de mutação
#define TAMANHOMAPA 15
#define NUM_TENTATIVAS 1 // Quantas vezes vamos rodar do zero

// --- ESTADO GLOBAL DA SIMULAÇÃO ---
// Precisamos disso global para persistir entre os frames da animação
int **mapa;
int **dist;
int **pop;
int *MelhorIndividuoGlobal;
int MelhorFitnessGlobal = 1e9;
int dest_x, dest_y;

// Contadores de Estado
int geracao_atual = 0;
int tentativa_atual = 0;
int simulacao_rodando = 0; // 1 = Sim, 0 = Pausada/Terminou
int simulacao_iniciada = 0; // 1 = Usuário clicou em INICIAR
int taxa_atual = PMT;

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

// --- Função que roda 1 passo da Evolução ---
void passoDaSimulacao(int value) {
    if (!simulacao_rodando) return;

    // Se estamos no início de uma tentativa, inicializa a população
    if (geracao_atual == 0) {
        if (pop != NULL) { // Limpa anterior se existir
             for(int i=0; i<POPSIZE; i++) free(pop[i]);
             free(pop);
        }
        pop = GerarPopulacao(POPSIZE, INDSIZE);
        printf("--- Iniciando Tentativa %d ---\n", tentativa_atual + 1);
    }

    // --- RODA 1 GERAÇÃO ---
    // (Ajuste a taxa de mutação conforme sua lógica original se necessário)
    ExecucaoAlgoritmo(pop, mapa, dist, POPSIZE, INDSIZE, TAMANHOMAPA, dest_x, dest_y, PCR, &taxa_atual);
    
    // --- ATUALIZA MELHOR LOCAL E GLOBAL ---
    int melhor_fit_local = 1e9;
    int id_melhor_local = 0;
    
    for(int i=0; i<POPSIZE; i++) {
        int f = determinarfitness(pop[i], mapa, dist, INDSIZE, TAMANHOMAPA, dest_x, dest_y);
        if(f < melhor_fit_local) {
            melhor_fit_local = f;
            id_melhor_local = i;
        }
    }

    // Se achou um recorde global, atualiza e manda pra interface!
    if (melhor_fit_local < MelhorFitnessGlobal) {
        MelhorFitnessGlobal = melhor_fit_local;
        
        // Salva Globalmente
        for(int k=0; k<INDSIZE; k++) MelhorIndividuoGlobal[k] = pop[id_melhor_local][k];
        
        printf("Novo Recorde: %d (Gen %d)\n", MelhorFitnessGlobal, geracao_atual);
    }
    
    // Manda desenhar o melhor DESTA GERAÇÃO (para ver ele tentando)
    // OU mande o MelhorIndividuoGlobal se quiser ver só o recordista
    setCaminhoVisualizacao(pop[id_melhor_local], INDSIZE); 

    // Avança contadores
    geracao_atual++;

    // Verifica fim da tentativa
    if (geracao_atual >= NGEN) {
        geracao_atual = 0;
        tentativa_atual++;
        
        if (tentativa_atual >= NUM_TENTATIVAS) {
            printf("\n=== SIMULAÇÃO FINALIZADA ===\n");
            printf("Melhor Fitness Final: %d\n", MelhorFitnessGlobal);
            simulacao_rodando = 0;
            simulacao_iniciada = 0;
            // Mostra o campeão final na tela
            setCaminhoVisualizacao(MelhorIndividuoGlobal, INDSIZE);
        }
    }

    // Redesenha a tela
    glutPostRedisplay();

    // Agenda o próximo passo daqui a 1ms (ou mais para ficar mais lento)
    if (simulacao_rodando) {
        glutTimerFunc(1, passoDaSimulacao, 0); // 1ms de delay
    }
}


// --- CALLBACKS GLUT ---

// Callback de desenho
void desenhar() {
    display(TAMANHOMAPA);
    glutSwapBuffers();
}

// Callback de mouse
void mouseCallback(int button, int state, int x, int y) {
    mouse(button, state, x, y, TAMANHOMAPA);
}

// Função para iniciar a simulação (chamada pelo botão da interface)
void iniciarSimulacao() {
    if (!simulacao_iniciada) {
        simulacao_iniciada = 1;
        simulacao_rodando = 1;
        geracao_atual = 0;
        tentativa_atual = 0;
        MelhorFitnessGlobal = 1e9;
        printf("=== SIMULAÇÃO INICIADA ===\n");
        glutTimerFunc(1, passoDaSimulacao, 0); // Começa o loop
    }
}

int main(int argc, char** argv)
{
    srand(time(NULL));

    // --- FASE 1: SETUP DO AMBIENTE (Executa 1 vez) ---
    
    // Aloca e Lê Mapa (ou gera aleatório se necessário)
    mapa = (int **)malloc(sizeof(int*) * TAMANHOMAPA);
    
    for (int i = 0; i < TAMANHOMAPA; i++)
    {
        mapa[i] = (int *)malloc(sizeof(int) * TAMANHOMAPA);
        for (int j = 0; j < TAMANHOMAPA; j++)
        {
                mapa[i][j] = 0;
        }
    }
    

    // Configura Destino e Dijkstra
    dest_x = TAMANHOMAPA - 1;
    dest_y = TAMANHOMAPA - 1;
    dist = gerarMapaDist(TAMANHOMAPA, mapa, dest_x, dest_y);

    // Aloca o melhor indivíduo global
    MelhorIndividuoGlobal = (int *)malloc(sizeof(int) * INDSIZE);

    // --- FASE 2: INICIALIZA GLUT/OPENGL ---
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(LARGURA_JANELA, ALTURA_JANELA);
    glutCreateWindow("Simulação Evolutiva - Algoritmo Genético");
    
    // Inicializa a interface (dados do OpenGL)
    init(mapa, TAMANHOMAPA);
    
    // Registra callbacks
    glutDisplayFunc(desenhar);          // Função de desenho
    glutMouseFunc(mouseCallback);        // Função de mouse
    // NÃO chama glutTimerFunc aqui - só quando o usuário clicar em INICIAR
    
    glutMainLoop(); // Aguarda eventos e redesenha

    // --- FASE 3: LIMPEZA FINAL (ao sair do glutMainLoop) ---

    printf("\n\n==========================================\n");
    printf("RESULTADO FINAL APÓS %d TENTATIVAS:\n", NUM_TENTATIVAS);
    printf("Melhor Fitness Global = %d\n", MelhorFitnessGlobal);
    
    ImprimirMelhorCaminho(MelhorIndividuoGlobal, INDSIZE, mapa);

    // Limpeza
    free(MelhorIndividuoGlobal);
    if (pop != NULL) {
        for(int i = 0; i < POPSIZE; i++) free(pop[i]);
        free(pop);
    }

    for (int i = 0; i < TAMANHOMAPA; i++) {
        free(mapa[i]);
        free(dist[i]);
    }
    free(mapa);
    free(dist);

    return 0;
}