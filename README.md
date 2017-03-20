# Trabalhos de Visão Computacional
#### Repositório com todos os meus trabalhos de Visão Computacional 1/2017 - UnB

Cada branch possui um dos trabalhos solicitados pelo professor. O branch principal possui o template com o mínimo comum necessário para cada projeto. Esse template foi feito para que todos os projetos possam ser multiplataforma. Todos os códigos foram testados no Windows 10 64-bit e no Linux Mint Cinnamon 18.1.

## Dependências
- Mingw64 (Windows) / g++, makefile (Linux)
- CMake
- OpenCV

## Configurações das máquinas usadas
- Desktop:
    - Dual boot: Windows 10 64-bit / Linux Mint Cinnamon 18.1
    - 2TB HD + 500GB SSD
    - 32GB RAM 2133 MHz
    - Gigabyte G1 GTX 1070 1.6GHz - 8GB VRAM 4GHz
    - Intel i7 6700K 4GHz
- Surface 3:
    - Windows 10 64-bit
    - 120GB SSD + 30GB SD10
    - 4GB RAM 1600MHz
    - Intel HD Graphics 1GB VRAM
    - Intel Atom x7-Z8700 1.6GHz
    - Intel AVStream Camera (8MP Traseira, 3.5MP Frontal)

## Instruções de instalação
###### Somente prossiga se as dependências estiverem corretamente instalados e funcionando!
- ### Windows
    - Baixe o branch de seu interesse desse repositório
    - Aqui existem três possibilidades:
        - Procedimento 1 (válido somente se você usa o MinGW):
            1. Vá até o diretório raiz do projeto com um prompt de comando
            2. Execute o `build.bat`
        - Procedimento 2:
            1. Vá até o diretório build do projeto com um prompt de comando
            2. Execute `cmake ..`
            3. Execute `make`
        - Procedimento 3:
            1. Abra o CMake GUI
            2. Selecione a pasta raiz do projeto como a que possui o código fonte
            3. Selecione a pasta build (ou qualquer subpasta dela que desejar) como local para fazer o build
            4. Configure e gere
            5. Execute make gerado na pasta de destino escolhida
    - Se você seguiu o procedimento 1, acaba por aqui. O executável está na pasta `build/<Debug;Release>`.
    - Se você seguiu o procedimento 2 ou 3, copie o conteúdo da pasta win-DLLs e cole no mesmo local que o executável foi gerado.
- ### Linux
    - Baixe o branch de seu interesse desse repositório
    - Vá até o diretório build do projeto com um terminal
    - Execute `cmake ..`
    - Execute `make`