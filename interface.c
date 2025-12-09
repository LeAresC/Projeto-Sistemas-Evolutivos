#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "mapa.h" 
#include "interface.h"

// --- ESTADOS GLOBAIS ---
int** meuMapa;
int ferramentaAtual = 0; // 0=Nada, 1=Luz, 2=Bloco, 3=Barreira, 4=Ponto, 5=Sombra, 6=ZigZag

// Parâmetros Configuráveis (Inputs)
int paramTamanho = 5;
int paramDirecao = 0; // 0..3

// Cores da UI
float corBotao[3] = {0.3, 0.3, 0.3};
float corBotaoHover[3] = {0.5, 0.5, 0.5};
float corTexto[3] = {1.0, 1.0, 1.0};

// --- FUNÇÕES AUXILIARES DE DESENHO UI ---

// Desenha texto na tela
void desenhaTexto(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c); // Fonte menor
    }
}

void desenhaTextoGrande(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); // Fonte maior
    }
}

// Desenha um botão retangular e retorna 1 se o mouse estiver sobre ele (para lógica de clique futura)
int desenhaBotao(float x, float y, float w, float h, const char* label, int ativo) {
    if (ativo) glColor3f(0.0, 0.6, 0.0); // Verde se ativo
    else glColor3fv(corBotao);
    
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();

    // Borda
    glColor3f(0.8, 0.8, 0.8);
    glLineWidth(1);
    glBegin(GL_LINE_LOOP);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + h);
        glVertex2f(x, y + h);
    glEnd();

    glColor3fv(corTexto);
    desenhaTexto(x + 10, y + h/2 - 4, label);

    return 0; // A verificação de clique é feita no MouseFunc
}

// Verifica se um ponto (mx, my) está dentro de um retangulo
int checaColisao(int mx, int my, float x, float y, float w, float h) {
    // OpenGL Y cresce pra cima, Mouse Y cresce pra baixo. 
    // Na função mouse já ajustaremos isso, então assumimos coordenadas de mundo aqui.
    return (mx >= x && mx <= x + w && my >= y && my <= y + h);
}

// --- DESENHO PRINCIPAL ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. DESENHAR O MAPA (Lado Esquerdo: 0 a 600)
    for (int i = 0; i < TAM_MAPA; i++) {
        for (int j = 0; j < TAM_MAPA; j++) {
            int valor = meuMapa[i][j];
            // Mapeia índice [0..50] para pixel [0..600]
            float px = (float)i * (AREA_MAPA / TAM_MAPA);
            float py = (float)j * (AREA_MAPA / TAM_MAPA);
            float tamCel = (float)AREA_MAPA / TAM_MAPA;

            if (valor == -1) glColor3f(0.7, 0.1, 0.1); // Obstáculo
            else {
                float k = (float)valor / 10.0f;
                glColor3f(k, k, k * 0.9 + 0.1); // Tons de cinza/luz
            }

            glRectf(px, py, px + tamCel - 1, py + tamCel - 1);
        }
    }

    // 2. DESENHAR O PAINEL LATERAL (Lado Direito: 600 a 800)
    // Fundo do painel
    glColor3f(0.2, 0.2, 0.2);
    glRectf(AREA_MAPA, 0, LARGURA_JANELA, ALTURA_JANELA);

    // Linha divisória
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex2f(AREA_MAPA, 0); glVertex2f(AREA_MAPA, ALTURA_JANELA);
    glEnd();

    // -- MENU DE FERRAMENTAS --
    float uiX = AREA_MAPA + 20;
    float uiY = ALTURA_JANELA - 50;
    
    glColor3f(1, 1, 0);
    desenhaTextoGrande(uiX, uiY, "FERRAMENTAS");
    uiY -= 40;

    // Botões de Seleção
    desenhaBotao(uiX, uiY, 160, 30, "1. Foco de Luz", ferramentaAtual == 1); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "2. Bloco Solido", ferramentaAtual == 2); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "3. Barreira", ferramentaAtual == 3); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "4. Ponto Unico", ferramentaAtual == 4); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "5. Sombra", ferramentaAtual == 5); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "6. Rota ZigZag", ferramentaAtual == 6); uiY -= 50;

    // -- PAINEL DE PARAMETROS (A "Janelinha" contextual) --
    glColor3f(0.0, 1.0, 1.0);
    desenhaTextoGrande(uiX, uiY, "PARAMETROS");
    uiY -= 30;

    char buffer[50];

    if (ferramentaAtual == 1 || ferramentaAtual == 2 || ferramentaAtual == 5 || ferramentaAtual == 3) {
        // Controle de Tamanho
        sprintf(buffer, "Tamanho: %d", paramTamanho);
        desenhaTexto(uiX, uiY, buffer);
        
        // Botões - e +
        desenhaBotao(uiX, uiY - 25, 30, 20, "-", 0);
        desenhaBotao(uiX + 40, uiY - 25, 30, 20, "+", 0);
        uiY -= 50;
    }

    if (ferramentaAtual == 3) {
        // Controle de Direção
        char* dirs[] = {"Vertical", "Horizontal", "Diag. Princ.", "Diag. Inv."};
        sprintf(buffer, "Dir: %s", dirs[paramDirecao]);
        desenhaTexto(uiX, uiY, buffer);

        // Botão trocar direção
        desenhaBotao(uiX, uiY - 25, 100, 20, "Girar >>", 0);
        uiY -= 50;
    }

    if (ferramentaAtual == 0 || ferramentaAtual == 4 || ferramentaAtual == 6) {
        desenhaTexto(uiX, uiY, "Sem parametros");
        desenhaTexto(uiX, uiY - 15, "extras.");
    }

    // -- BOTÕES GERAIS --
    uiY = 80;
    desenhaBotao(uiX, uiY, 160, 30, "RANDOMIZAR MAPA", 0);
    uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "LIMPAR TUDO", 0);

    glFlush();
}

// --- LÓGICA DE INTERAÇÃO ---

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    // Converter Y do mouse (topo=0) para Y do OpenGL (base=0)
    int glY = ALTURA_JANELA - y;

    // CASO 1: CLIQUE NO MAPA
    if (x < AREA_MAPA) {
        int gridX = (x * TAM_MAPA) / AREA_MAPA;
        int gridY = (glY * TAM_MAPA) / AREA_MAPA;

        printf("Clique no Mapa [%d][%d] com Ferramenta %d\n", gridX, gridY, ferramentaAtual);

        switch(ferramentaAtual) {
            case 1: focoDeLuz(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
            case 2: obstaculoBloco(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
            case 3: obstaculoBarreira(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY, paramDirecao); break;
            case 4: obstaculo(meuMapa, TAM_MAPA, gridX, gridY); break;
            case 5: blocoSombra(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
            case 6: gerarOtimoZigZag(meuMapa, TAM_MAPA, 0, 0, gridX, gridY); break;
        }
    }
    // CASO 2: CLIQUE NA BARRA LATERAL (UI)
    else {
        float uiX = AREA_MAPA + 20;
        float uiY = ALTURA_JANELA - 50; // Mesma logica de desenho

        // Checar Botões de Ferramentas
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 1; // Luz
        uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 2; // Bloco
        uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 3; // Barreira
        uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 4; // Ponto
        uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 5; // Sombra
        uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY - 40, 160, 30)) ferramentaAtual = 6; // ZigZag
        uiY -= 50;

        // Checar Controles Parametros (Janelinha)
        // Reset uiY para a área de parametros
        float paramY = uiY - 30; 

        if (ferramentaAtual == 1 || ferramentaAtual == 2 || ferramentaAtual == 5 || ferramentaAtual == 3) {
             // Botão [-] Tamanho
            if (checaColisao(x, glY, uiX, paramY - 25, 30, 20)) {
                if (paramTamanho > 1) paramTamanho--;
            }
            // Botão [+] Tamanho
            if (checaColisao(x, glY, uiX + 40, paramY - 25, 30, 20)) {
                if (paramTamanho < TAM_MAPA) paramTamanho++;
            }
            paramY -= 50;
        }

        if (ferramentaAtual == 3) {
             // Botão [Girar]
            if (checaColisao(x, glY, uiX, paramY - 25, 100, 20)) {
                paramDirecao = (paramDirecao + 1) % 4;
            }
        }

        // Checar Botões Gerais (fundo da tela)
        float genY = 80;
        if (checaColisao(x, glY, uiX, genY, 160, 30)) randomizaMapa(meuMapa, TAM_MAPA);
        genY -= 40;
        if (checaColisao(x, glY, uiX, genY, 160, 30)) {
            for(int i=0; i<TAM_MAPA; i++) 
                for(int j=0; j<TAM_MAPA; j++) meuMapa[i][j] = 0;
        }
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, LARGURA_JANELA, 0, ALTURA_JANELA);
    
    srand(42);
    meuMapa = criaMapaZero(TAM_MAPA);
}
