

int** criarMapaZero(int ordem){ //cria um mapa que será matriz de ordem [ordem][ordem]
    int** linhas = (int**)calloc(ordem, sizeof(int)); //aloca memoria para as linhas
    for (int i = 0; i<ordem; i++){
        linhas[i] = (int*)calloc(ordem, sizeof(int)); //aloca memoria para as colunas
    }

    return linhas; //retorna a matriz gerada
}

void randomizarMapa(int** mapa, int ordem){ //
    for(int i = 0; i<ordem; i++){
        for (int j = 0; j< ordem; j++){
            mapa[i][j] = 1;
        }
    }
}