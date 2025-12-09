#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include "mapa.h" 
#include "interface.h"

// --- ESTADOS GLOBAIS ---
int** meuMapa;
// 1=Bloco Luz, 2=Foco Luz, 3=Bloco Solido, 4=Barreira, 5=Ponto, 6=Sombra, 7=ZigZag
int ferramentaAtual = 0; 

// Parâmetros Configuráveis (Inputs)
int paramTamanho = 5;
int paramDirecao = 0; // 0..3
int paramIntensidade = 10; // Valor padrão de luz (0-10)
int respeitaObs = 1; // 1 = Sim, 0 = Não

// Cores da UI
float corBotao[3] = {0.3, 0.3, 0.3};
float corTexto[3] = {1.0, 1.0, 1.0};

// --- FUNÇÕES AUXILIARES DE DESENHO UI ---

void desenhaTexto(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void desenhaTextoGrande(float x, float y, const char *string) {
    glRasterPos2f(x, y);
    for (const char *c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

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

    return 0;
}

int checaColisao(int mx, int my, float x, float y, float w, float h) {
    return (mx >= x && mx <= x + w && my >= y && my <= y + h);
}

// --- DESENHO PRINCIPAL ---

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // 1. DESENHAR O MAPA
    for (int i = 0; i < TAM_MAPA; i++) {
        for (int j = 0; j < TAM_MAPA; j++) {
            int valor = meuMapa[i][j];
            float px = (float)i * (AREA_MAPA / TAM_MAPA);
            float py = (float)j * (AREA_MAPA / TAM_MAPA);
            float tamCel = (float)AREA_MAPA / TAM_MAPA;

            if (valor == -1) glColor3f(0.7, 0.1, 0.1); // Obstáculo
            else {
                float k = (float)valor / 10.0f;
                glColor3f(k, k, k * 0.9 + 0.1); 
            }
            glRectf(px, py, px + tamCel - 1, py + tamCel - 1);
        }
    }

    // 2. DESENHAR O PAINEL LATERAL
    glColor3f(0.2, 0.2, 0.2);
    glRectf(AREA_MAPA, 0, LARGURA_JANELA, ALTURA_JANELA);

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

    // Botões de Seleção (A ordem aqui define o ID da ferramenta)
    desenhaBotao(uiX, uiY, 160, 30, "1. Bloco de Luz", ferramentaAtual == 1); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "2. Foco de Luz", ferramentaAtual == 2); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "3. Bloco Solido", ferramentaAtual == 3); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "4. Barreira", ferramentaAtual == 4); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "5. Ponto Unico", ferramentaAtual == 5); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "6. Sombra", ferramentaAtual == 6); uiY -= 40;
    desenhaBotao(uiX, uiY, 160, 30, "7. Rota ZigZag", ferramentaAtual == 7); uiY -= 50;

    // -- PAINEL DE PARAMETROS --
    glColor3f(0.0, 1.0, 1.0);
    desenhaTextoGrande(uiX, uiY, "PARAMETROS");
    uiY -= 30;

    char buffer[50];

    // PARÂMETRO 1: TAMANHO (Ferramentas 1, 2, 3, 4, 6)
    if (ferramentaAtual == 1 || ferramentaAtual == 2 || ferramentaAtual == 3 || ferramentaAtual == 4 || ferramentaAtual == 6) {
        sprintf(buffer, "Tamanho: %d", paramTamanho);
        desenhaTexto(uiX, uiY, buffer);
        
        desenhaBotao(uiX, uiY - 25, 30, 20, "-", 0);
        desenhaBotao(uiX + 40, uiY - 25, 30, 20, "+", 0);
        uiY -= 50;
    }

    // PARÂMETRO 2: INTENSIDADE E OBSTÁCULO (Apenas Ferramenta 1)
    if (ferramentaAtual == 1) {
        sprintf(buffer, "Luz: %d", paramIntensidade);
        desenhaTexto(uiX, uiY, buffer);
        
        desenhaBotao(uiX, uiY - 25, 30, 20, "-", 0);     
        desenhaBotao(uiX + 40, uiY - 25, 30, 20, "+", 0); 
        uiY -= 50;

        // Botão de Toggle (Respeita Obstáculo)
        if(respeitaObs) sprintf(buffer, "[X] Respeita Obs");
        else sprintf(buffer, "[ ] Ignora Obs");
        
        desenhaBotao(uiX, uiY - 25, 140, 20, buffer, respeitaObs);
        uiY -= 50;
    }

    // PARÂMETRO 3: DIREÇÃO (Apenas Ferramenta 4)
    if (ferramentaAtual == 4) {
        char* dirs[] = {"Vertical", "Horizontal", "Diag. P.", "Diag. I."};
        sprintf(buffer, "Dir: %s", dirs[paramDirecao]);
        desenhaTexto(uiX, uiY, buffer);

        desenhaBotao(uiX, uiY - 25, 100, 20, "Girar >>", 0);
        uiY -= 50;
    }

    if (ferramentaAtual == 0 || ferramentaAtual == 5 || ferramentaAtual == 7) {
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

    int glY = ALTURA_JANELA - y;

    // CASO 1: CLIQUE NO MAPA
    if (x < AREA_MAPA) {
        int gridX = (x * TAM_MAPA) / AREA_MAPA;
        int gridY = (glY * TAM_MAPA) / AREA_MAPA;

        if (gridX >= 0 && gridX < TAM_MAPA && gridY >= 0 && gridY < TAM_MAPA) {
            printf("Mapa [%d][%d] Ferramenta %d\n", gridX, gridY, ferramentaAtual);

            switch(ferramentaAtual) {
                // Certifique-se que blocoLuz aceita 7 argumentos no mapa.c ou remova respeitaObs se não existir
                case 1: blocoLuz(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY, paramIntensidade, respeitaObs); break; 
                case 2: focoDeLuz(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
                case 3: obstaculoBloco(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
                case 4: obstaculoBarreira(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY, paramDirecao); break;
                case 5: obstaculo(meuMapa, TAM_MAPA, gridX, gridY); break;
                case 6: blocoSombra(meuMapa, TAM_MAPA, paramTamanho, gridX, gridY); break;
                case 7: gerarOtimoZigZag(meuMapa, TAM_MAPA, 0, 0, gridX, gridY); break;
            }
        }
    }
    // CASO 2: CLIQUE NA UI
    else {
        float uiX = AREA_MAPA + 20;
        float uiY = ALTURA_JANELA - 50; // Sincronizado com display

        uiY -= 40;

        // --- Checar Botões de Ferramentas ---
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 1; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 2; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 3; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 4; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 5; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 6; uiY -= 40;
        if (checaColisao(x, glY, uiX, uiY, 160, 30)) ferramentaAtual = 7; uiY -= 50;

        // --- Checar Parâmetros ---
        uiY -= 30; // Título "PARAMETROS"
        float paramY = uiY; 

        // 1. TAMANHO (Ferramentas 1, 2, 3, 4, 6)
        if (ferramentaAtual == 1 || ferramentaAtual == 2 || ferramentaAtual == 3 || ferramentaAtual == 4 || ferramentaAtual == 6) {
            // [-]
            if (checaColisao(x, glY, uiX, paramY - 25, 30, 20)) {
                if (paramTamanho > 1) paramTamanho--;
            }
            // [+]
            if (checaColisao(x, glY, uiX + 40, paramY - 25, 30, 20)) {
                if (paramTamanho < TAM_MAPA) paramTamanho++;
            }
            paramY -= 50; // Pula para a próxima linha
        }

        // 2. INTENSIDADE E OBSTÁCULO (Apenas 1)
        if (ferramentaAtual == 1) {
            // [-] Luz
            if (checaColisao(x, glY, uiX, paramY - 25, 30, 20)) {
                if (paramIntensidade > 0) paramIntensidade--;
            }
            // [+] Luz
            if (checaColisao(x, glY, uiX + 40, paramY - 25, 30, 20)) {
                if (paramIntensidade < 10) paramIntensidade++;
            }
            paramY -= 50;

            // Botão Toggle Obs
            if (checaColisao(x, glY, uiX, paramY - 25, 140, 20)) {
                respeitaObs = !respeitaObs; // Alterna entre 0 e 1 corretamente
            }
            paramY -= 50;
        }

        // 3. DIREÇÃO (Apenas 4)
        if (ferramentaAtual == 4) {
             // Botão [Girar]
            if (checaColisao(x, glY, uiX, paramY - 25, 100, 20)) {
                paramDirecao = (paramDirecao + 1) % 4;
            }
            paramY -= 50;
        }

        // --- Botões Gerais ---
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