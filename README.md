
README - Investigação Digital - Amplificação de Ruído em Imagens
Introdução
Este projeto tem como objetivo realizar uma análise forense simplificada em imagens, focando em identificar possíveis manipulações através da amplificação de ruído. O algoritmo amplifica o ruído já existente nas imagens, destacando áreas manipuladas ou alteradas. Técnicas como cópia e colagem de elementos em imagens podem ser detectadas dessa maneira, já que as alterações introduzem padrões anômalos no ruído natural da imagem.

Funcionamento do Algoritmo
Leitura da Imagem: O programa carrega uma imagem de entrada no formato RGB (24 bits por pixel, com 8 bits para cada canal: R - vermelho, G - verde e B - azul).

Filtro de Mediana: Um filtro de mediana é aplicado para cada pixel, baseado em sua luminância (quão clara ou escura é a cor), calculada pela fórmula:

Luminância = 0.59 ⋅ 𝐺 + 0.30 ⋅ 𝑅 + 0.11 ⋅ 𝐵

O filtro percorre a imagem em regiões (por exemplo, 3x3 pixels), ordena os pixels por sua luminância e substitui o pixel central pela mediana dessa lista.

Subtração da Mediana: Após calcular a mediana, subtraímos o valor do pixel original para amplificar o ruído da imagem. Esse valor é armazenado em uma imagem de saída.

Amplificação: Um fator de amplificação pode ser ajustado pelo usuário para aumentar ou reduzir o efeito da manipulação do ruído.

Exibição: O resultado é uma imagem em que áreas com manipulação de pixels tendem a se destacar, facilitando a identificação de alterações feitas de forma não natural.

Uso do Programa
Controles
Tecla 1: Exibe a imagem original.
Tecla 2: Exibe a imagem com o ruído amplificado.
Tecla =: Aumenta o fator de amplificação de ruído.
Tecla -: Diminui o fator de amplificação de ruído.
Execução e Compilação
O projeto pode ser compilado e executado em diferentes sistemas operacionais (Linux, Windows, macOS), e possui suporte para Visual Studio Code, além de outras IDEs e terminais.

Compilação no Linux/macOS:
Instale as dependências:
1 - sudo apt-get install freeglut3-dev
No terminal, navegue até o diretório do projeto e compile:
2 - make
Compilação no Windows:
Abra o terminal ou use o Visual Studio Code.
Para compilar no terminal:
1- mingw32-make -f Makefile.mk
Usando Visual Studio Code (em qualquer sistema operacional):
Abra a pasta do projeto.
Compile com Ctrl+Shift+B (ou ⌘+Shift+B no macOS).
Execute com F5 (debugger) ou Ctrl+F5 (sem debugger).
Estrutura do Projeto
Código-fonte: O algoritmo principal está na função processa() no arquivo de código principal, onde a amplificação do ruído é implementada.
Bibliotecas: O projeto utiliza a biblioteca SOIL para carregamento de imagens e OpenGL para renderização gráfica.
Arquivos Importantes
main.c: Contém a lógica principal do programa, incluindo o algoritmo de amplificação do ruído.
Makefile e Makefile.mk: Scripts de compilação para Linux, macOS e Windows.
SOIL.h: Biblioteca para manipulação de imagens.
Imagens de exemplo: O projeto inclui imagens de teste para demonstrar a detecção de manipulações.
