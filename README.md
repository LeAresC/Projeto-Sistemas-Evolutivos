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
(Espaço reservado para a descrição do funcionamento do Algoritmo Genético, Crossover, Mutação e Função de Fitness)

[A SER PREENCHIDO PELA EQUIPE DE BACKEND]

Detalhes sobre a representação do cromossomo.

Explicação do cálculo de Dijkstra (heurística).

Parâmetros genéticos utilizados (Taxa de mutação, tamanho da população, etc).