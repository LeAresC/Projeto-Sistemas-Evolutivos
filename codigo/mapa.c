#include <stdlib.h>

int** criaMapaZero(int ordem){ //cria um mapa que será matriz de ordem [ordem][ordem]
    if (ordem < 1){
        return NULL;
    }
    int** linhas = (int**)calloc(ordem, sizeof(int*)); //aloca memoria para as linhas
    for (int i = 0; i<ordem; i++){
        linhas[i] = (int*)calloc(ordem, sizeof(int)); //aloca memoria para as colunas
    }

    return linhas; //retorna a matriz gerada
}

void randomizaMapa(int** mapa, int ordem){ //recebe um mapa e coloca valores aleatorios 
    for(int i = 0; i<ordem; i++){
        for (int j = 0; j< ordem; j++){
            mapa[i][j] = (rand() % 10) + 1; //valores de 1 a 10 para a luminosidade
        }
    }
}

void blocoLuz(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy, int intensidade, int respeitaObst) {
    
    // Garante que a intensidade esteja entre 0 (escuro) e 10 (claro)
    if (intensidade < 0) intensidade = 0;
    if (intensidade > 10) intensidade = 10;

    for (int i = coordx; i < coordx + ordemBloco; i++) {
        for (int j = coordy; j < coordy + ordemBloco; j++) {
            
            // Verifica limites do mapa
            if (i < 0 || i >= ordemMapa || j < 0 || j >= ordemMapa)
                continue;

            
            if(respeitaObst){
                // Preserva obstáculos (-1)
                if (mapa[i][j] == -1)
                    continue;
            }

            // Aplica a intensidade definida
            mapa[i][j] = intensidade;
        }
    }
}

void focoDeLuz(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy) {

    if (ordemBloco < 1)
        return;

    int centroI = coordx + ordemBloco / 2;
    int centroJ = coordy + ordemBloco / 2;

    // distância máxima possível até o centro
    int distMax = (ordemBloco - 1 - ordemBloco/2) + (ordemBloco - 1 - ordemBloco/2);
    if (distMax < 1) distMax = 1;

    for (int i = coordx; i < coordx + ordemBloco; i++) {
        for (int j = coordy; j < coordy + ordemBloco; j++) {

            // fora do mapa ignora
            if (i < 0 || i >= ordemMapa || j < 0 || j >= ordemMapa)
                continue;

            // preserva células com -1 (obstaculos)
            if (mapa[i][j] == -1)
                continue;

            int distI = abs(i - centroI);
            int distJ = abs(j - centroJ);
            int dist = distI + distJ;

            // 0 no canto, 10 no centro, tudo inteiro
            int valor = 10 - (dist * 10) / distMax;
            if (valor < 0) valor = 0;
            if (valor > 10) valor = 10;

            // mesclar: mantém o mais forte
            if (valor > mapa[i][j])
                mapa[i][j] = valor;
        }
    }
}

void obstaculoBarreira(int** mapa, int ordem, int tamanho, int coordx, int coordy, int direcao){
    direcao = direcao % 4;
    switch (direcao) {

        // 0 = vertical
        case 0:
            for (int i = coordx; i < coordx + tamanho; i++) {

                if (i < 0 || i >= ordem || coordy < 0 || coordy >= ordem)
                    continue;

                mapa[i][coordy] = -1;
            }
            break;

        // 1 = horizontal
        case 1:
            for (int j = coordy; j < coordy + tamanho; j++) {

                if (j < 0 || j >= ordem || coordx < 0 || coordx >= ordem)
                    continue;

                mapa[coordx][j] = -1;
            }
            break;

        // 2 = diagonal principal
        case 2:
            for (int k = 0; k < tamanho; k++) {

                int i = coordx + k;
                int j = coordy + k;

                if (i < 0 || i >= ordem || j < 0 || j >= ordem)
                    continue;

                mapa[i][j] = -1;
            }
            break;

        // 3 = diagonal inversa
        case 3:
            for (int k = 0; k < tamanho; k++) {

                int i = coordx + k;
                int j = coordy - k;

                if (i < 0 || i >= ordem || j < 0 || j >= ordem)
                    continue;

                mapa[i][j] = -1;
            }
            break;
    }
}



void obstaculoBloco(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy){
    for(int i = coordx; i < coordx + ordemBloco; i++){
        for(int j = coordy; j < coordy + ordemBloco; j++){
            if((i>= 0)&&(i<ordemMapa)&&(j>=0)&&(j<ordemMapa)){
                mapa[i][j] = -1;
            }
        }
    }
}

void obstaculo(int** mapa, int ordem, int i, int j) {
    if((i>= 0)&&(i<ordem)&&(j>=0)&&(j<ordem)){
        mapa[i][j] = -1;
    }
}



void blocoSombra(int** mapa, int ordemMapa, int ordemBloco, int coordx, int coordy) {

    for (int i = coordx; i < coordx + ordemBloco; i++) {
        for (int j = coordy; j < coordy + ordemBloco; j++) {

            // verifica limites
            if (i < 0 || i >= ordemMapa || j < 0 || j >= ordemMapa)
                continue;

            // preserva obstáculos
            if (mapa[i][j] == -1)
                continue;

            // reduz intensidade pela metade
            mapa[i][j] = mapa[i][j] / 2;
        }
    }
}

void gerarOtimoZigZag(int** mapa, int ordem, int srcCoordX, int srcCoordY, int destCoordX, int destCoordY) {
    int x = srcCoordX;
    int y = srcCoordY;

    // Alterna horizontal e vertical aleatoriamente
    while (x != destCoordX || y != destCoordY) {

        // marca célula atual
        if (x >= 0 && x < ordem && y >= 0 && y < ordem)
            mapa[x][y] = 1;

        int moveHorizontal = 0;
        int moveVertical = 0;

        if (x != destCoordX && y != destCoordY) {
            // escolha aleatória quando ambos precisam se mover
            moveHorizontal = rand() % 2;
            moveVertical = 1 - moveHorizontal;
        } else if (x != destCoordX) {
            moveHorizontal = 1;
        } else if (y != destCoordY) {
            moveVertical = 1;
        }

        // mover horizontal
        if (moveHorizontal) {
            if (x < destCoordX) x++;
            else x--;
        }

        // mover vertical
        if (moveVertical) {
            if (y < destCoordY) y++;
            else y--;
        }
    }

    // marca célula final
    if (x >= 0 && x < ordem && y >= 0 && y < ordem)
        mapa[x][y] = 1;
}