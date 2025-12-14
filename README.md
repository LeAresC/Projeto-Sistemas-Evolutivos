# Projeto-Sistemas-Evolutivos
🌑 Simulação Evolutiva: O Caminho na Sombra
Este projeto implementa um sistema de Algoritmos Genéticos visualizado em tempo real via OpenGL (GLUT). O objetivo principal da simulação é encontrar o melhor trajeto entre dois pontos em uma grade, com uma restrição específica: o indivíduo deve priorizar áreas de sombra e evitar a luz, simulando um comportamento furtivo ou fotossensível.

🎯 Objetivo do Projeto
Desenvolver uma aplicação gráfica interativa que permita:

Criação de Cenários: O usuário pode desenhar mapas complexos contendo obstáculos físicos e fontes de luz com diferentes intensidades.

Visualização Evolutiva: Acompanhar, geração a geração, a evolução da população de agentes tentando encontrar o caminho até o destino.

Análise de Desempenho: Monitorar a convergência do algoritmo através de gráficos em tempo real (Melhor Fitness vs. Fitness Médio).

🖥️ Interface Gráfica (Frontend)
A interface foi construída utilizando a biblioteca FreeGLUT, dividida em três seções principais para facilitar a interação e análise:

1. Área de Simulação (Mapa)
Visualização em Grid: Representação matricial do ambiente.

Código de Cores:

🟥 Vermelho: Obstáculos físicos (paredes/bloqueios).

⬜ Escala de Cinza/Branco: Níveis de luminosidade (quanto mais claro, mais "perigoso" ou custoso para o agente).

🟨 Linha Amarela: Representação visual do melhor indivíduo da geração atual.

2. Painel de Controle (Editor de Mapas)
Um conjunto completo de ferramentas para desenhar o ambiente de teste. O usuário pode interagir com o mapa clicando nas células.

Ferramentas Disponíveis:

🔦 Bloco de Luz: Preenche uma área quadrada com luz. Permite ajustar a Intensidade (0-10) e se deve respeitar ou sobrescrever obstáculos.

🔆 Foco de Luz: Cria um gradiente de luz que diminui de intensidade conforme se afasta do centro.

🧱 Bloco Sólido: Cria áreas quadradas de obstáculos intransponíveis.

🚧 Barreira: Cria paredes lineares. Possui controle de Direção (Horizontal, Vertical, Diagonais).

📍 Ponto Único: Coloca um obstáculo em uma única célula (precisão).

🌑 Sombra: Reduz a luminosidade de uma área pela metade, criando zonas seguras.

⚡ Rota ZigZag: Gera um padrão de teste específico para validar algoritmos de busca.

Parâmetros Ajustáveis:

Tamanho: Define a área de atuação do pincel/ferramenta.

Intensidade: Controla o nível de luz aplicado.

Direção: Rotaciona estruturas lineares (Barreiras).

3. Painel de Métricas (Gráficos)
Localizado no topo da tela, exibe o histórico evolutivo em tempo real:

📈 Gráfico Verde: Evolução do Melhor Fitness (o quão bom é o melhor caminho encontrado).

📈 Gráfico Azul: Evolução do Fitness Médio da população (diversidade e convergência).

⚙️ Controles da Aplicação
Mouse:

Clique no Mapa: Aplica a ferramenta selecionada.

Clique no Painel: Seleciona ferramentas, ajusta parâmetros (+/-) e controla a simulação.

Botões de Estado:

INICIAR SIMULACAO: Começa o processo evolutivo.

RANDOMIZAR MAPA: Gera um terreno aleatório para testes rápidos.

LIMPAR TUDO: Reseta o mapa para o estado vazio.

🧠 Lógica Evolutiva (Backend)

🧬 Algoritmo Genético para Otimização de Caminhos (Pathfinding)
Este projeto implementa um Algoritmo Genético (AG) em linguagem C para encontrar o caminho de menor custo em um grid bidimensional contendo obstáculos e terrenos com diferentes "pesos" (custos de travessia).

O diferencial deste projeto é a utilização híbrida de um Algoritmo de Dijkstra para guiar a evolução, evitando que os indivíduos fiquem presos em mínimos locais (como caminhos geometricamente curtos, mas de alto custo).

🚀 Como Funciona
O algoritmo simula o processo de seleção natural. Uma população de "indivíduos" (caminhos possíveis) evolui ao longo de gerações. Os melhores caminhos têm maior chance de se reproduzir e passar suas características para a próxima geração.

1. Representação (O Genoma)
Cada indivíduo é representado por um vetor de inteiros de tamanho fixo (INDSIZE), onde cada gene representa um movimento relativo:

0: Cima

1: Direita

2: Baixo

3: Esquerda

O caminho real é construído simulando esses passos a partir do ponto de partida (0,0).

2. Função de Fitness (Hierárquica)
A avaliação dos indivíduos segue uma lógica estrita de penalidades para garantir a convergência correta. O objetivo é minimizar o valor do fitness.

A hierarquia de avaliação é:

Vitória (Chegou ao Destino):

Fitness = Soma dos Custos dos Terrenos visitados.

Objetivo: Otimizar o caminho escolher caminhos com menos luminosidade

Falha (Acabaram os passos sem chegar):

Fitness = Penalidade Média + Distância Real até o Alvo (via Dijkstra).

Objetivo: Incentivar a aproximação do alvo desviando de paredes.

Morte (Bateu na parede ou saiu do mapa):

Fitness = Penalidade Máxima + Distância Manhattan.

Objetivo: Ensinar o indivíduo a permanecer dentro dos limites válidos.

3. O "GPS" Evolutivo (Dijkstra)
Para evitar que o algoritmo genético fique "cego" em labirintos complexos, executamos o algoritmo de Dijkstra uma única vez no início do programa (gerando um mapa de calor a partir do destino). Isso permite que indivíduos que ainda não chegaram ao final saibam a distância real (custo) que falta, ajudando-os a contornar obstáculos em forma de "U" ou evitar caminhos de alta luminosidade.

⚙️ Operadores Genéticos
Seleção por Torneio (k=2): Dois indivíduos são escolhidos aleatoriamente; o de menor fitness vence e se torna pai. Isso preserva diversidade genética.

Crossover de Dois Pontos (Two-Point): Dois pontos de corte são escolhidos no vetor dos pais. Os filhos herdam as pontas de um pai e o "miolo" do outro, preservando sequências de movimentos válidas.

Mutação:

Taxa baixa (ex: 2% a 5% por gene).

Altera aleatoriamente a direção de um passo.

Inlcui estratégias de Mutação em Bloco para escapar de armadilhas locais.

📊 Parâmetros Configuráveis

| Parâmetro | Descrição | Valor Recomendado |
| :--- | :--- | :--- |
| `POPSIZE` | Tamanho da População | 200 a 500 |
| `INDSIZE` | Tamanho do Genoma (Passos máximos) | 4x a 5x a largura do mapa |
| `NGEN` | Número de Gerações | 150 a 300 |
| `PCR` | Probabilidade de Crossover | 80 (%) |
| `PMT` | Probabilidade de Mutação | 2 a 5 (%) |
| `NUM_TENTATIVAS` | Execuções completas (Multi-Start) | 10 |

🛠️ Instruções de Compilação
Para compilar e executar o Projeto-Sistemas-Evolutivos, você precisará do compilador GCC e das bibliotecas de desenvolvimento do FreeGLUT e do GLU.

1. Pré-requisitos
Compilador C: GCC (GNU Compiler Collection).

Bibliotecas de Desenvolvimento:

FreeGLUT: Para gerenciamento de janelas e eventos OpenGL (ex: freeglut.h, freeglut.lib).

OpenGL & GLU: As bibliotecas gráficas padrão (ex: opengl32.lib, glu32.lib).

Biblioteca Padrão C: Para funções como malloc, printf, time, etc.

Se estiver usando MinGW ou MinGW-w64 no Windows, as bibliotecas geralmente vêm com o pacote, mas você pode precisar instalá-las se estiver em um ambiente Linux (via apt, yum, etc.).

Exemplo de Instalação (Debian/Ubuntu):
sudo apt update
sudo apt install build-essential libglu1-mesa-dev freeglut3-dev

2. Compilação (Usando o GCC)
O projeto é composto por vários arquivos .c e .h. Você deve compilar todos os arquivos de implementação (.c) e vinculá-los com as bibliotecas necessárias.

Comando de Compilação Único:

Navegue até o diretório codigo/ e execute o seguinte comando:
gcc -o main *.c -lglut -lglu32 -lopengl32 -lm

Opção,Descrição
-o main,Define o nome do arquivo de saída executável como evolucao (ou evolucao.exe no Windows).
*.c,Lista todos os arquivos C do projeto.
-lglut,Vincula com a biblioteca FreeGLUT.
-lglu32,Vincula com a biblioteca GLU (GL Utility).
-lopengl32,Vincula com a biblioteca principal do OpenGL.
-lm,Vincula com a biblioteca matemática (pode ser necessário para algumas funções de C).

3. Execução
Após a compilação, o executável estará pronto.

Certifique-se de que o executável (evolucao ou evolucao.exe) está na mesma pasta (ou em uma pasta listada na variável de ambiente PATH).

Execute o programa:
./evolucao

4. Solução de Problemas Comuns (Windows/MinGW)
Se você receber um erro como: The code execution cannot proceed because freeglut.dll was not found.

Isso significa que a biblioteca dinâmica freeglut.dll (que já tem na pasta codigo.zip/codigo/) não está sendo encontrada pelo sistema operacional.

Solução:

Copie o arquivo freeglut.dll da sua pasta de projeto para:

O mesmo diretório onde o evolucao.exe foi gerado (recomendado para fácil distribuição).

Ou, para um diretório que esteja listado na variável de ambiente PATH do sistema.

Dessa forma, o sistema encontrará a DLL necessária para executar a aplicação.


📝 Autores
Desenvolvido como parte de um estudo sobre Sistemas Evolutivos e Otimização Combinatória. 

Luis Eduardo Aires Coimbra; Pedro Rodrigues Luiz Ferreira
