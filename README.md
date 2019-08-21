# Autores

- Cristian Deyves Oliveira De Brito
- Marco Antônio Fernandes Conceição

# Sobre

- Esse projeto tem como objetivo a manipulação de imagens do tipo ppm, as quais são do tipo "P3". Com a finalidade de implementar filtros nas imagens fornecidas pelo usuario do programa. Este programa foi criado utilizando algumas bibliotecas padrões de C, para criar uma biblioteca cujas funções manipulam imagens. Foi criado um makefile para facilitar a compilação e execução do programa pelo usuário.

## O que fariamos diferente hoje

Modularizariamos melhor, separando os filtros em diferentes arquivos, funcionalidades e utilidades. Para que dessa forma o programa funcionasse para qualquer arquivo, não só arquivos do ambito `ppm` como também para arquivos de texto e outros tipos de arquivos do tipo imagem como `jpg`, `png`, `bmp`. Utilizariamos melhor os structs, fazendo um struct que recebesse a imagem as características da imagem,  além dos pixels em RGB, foi algo que tentamos implementar porem não conseguimos levar em frente no projeto, devido à maior complexidade em trabalhar com esses valores, que levou a diversos erros que não conseguimos contornar, sendo algo que deixamos desejar e que desejariamos ter implementado.

Além disso, não conseguimos implementar alguns filtros com bases mais complexas como os filtros baseados na transformada de Fourier e nas curvas de Bezier, que possibilitariam gerar uma nova gama de possibilidades no programa, mas devido às dificuldades na aplicação do procedimento, não conseguimos implementar.

# Como compilar e executar

Utilizando o *`make`*, ele ira compilar o programa pra o usuário. Utilizando o *`gcc`* o usuário tera de executar manualmente após criar os arquivos objetos e compilar os mesmos gerando uma executável.

## Caso utilize o make

Verifica se precisa criar algum diretório(diretórios), caso precise ele cria o(os) diretório(diretórios), para compilar e executar o programa.
```sh
make mkdir
```
Compila automaticamente
```sh
make
```
Executa o programa.
```sh
make run
```
Exclui os diretórios que estavam sendo utilizados pelo programa com os arquivos objeto, com as imagens e com o executável.
```sh
make clean
```
Debugging para correção de falhas caso seja implementado alguma ferramenta ao programa esteja dando prolema.
```sh
make gdb
```
## Caso utilize o gcc

Crie o diretório **`build`**, utilizando o seguinte comando `mkdir build` na pasta do raiz do programa.

Crie o diretório **`bin`**, utilizando o seguinte comando `mkdir bin` na pasta do raiz do programa.

Compile o arquivo com o seguinte código:
```sh
gcc -g -W -I include -c src/functions.c -o build/functions.o
```
```sh
gcc -g -W -I include -c src/home-project.c -o build/home-project.o
```
```sh
gcc -g -W -I include -o bin/photo-editor build/functions.o build/home-project.o -lm
```
```sh
./bin/photo-editor
```

# Funções que dispomos

### Funções obrigatórias
- [x] **Greyscale**

 Aplica um filtro de escala de cinza na imagem, de acordo com os parâmetros já estabelecidos para esse filtro.
 By: Deyves.

- [x] **Enlarge**

 Replica os pixels da imagem de acordo com o fator fornecido pelo usuário, aumentando a imagem como resultado.
 By: Deyves.

- [x] **Blurring**

 Realiza a filtragem mediana dos pontos de uma matriz 3x3 que passa pela imagem e tem como resultado o borramento da imagem.
 By: Marco.

- [x] **Thresholding**

 Divide a imagem em duas seções, tornando preto o que estiver com coloração abaixo do valor barreira e branco o que estiver acima deste.
 O valor barreira pode ser calculado de forma automática pelo histograma das frequências, ou pode ser fornecido manualmente pelo usuário.
 By: Marco.

- [x] **Reduce**

 Une os pixels próximos de acordo com o fator estabelecido pelo usuário para formar uma imagem semelhante mas com menor tamanho.
 By: Marco.

- [x] **Edge Detection**

 Aplica as matrizes de detecção de borda para gerar uma imagem com reforço nas regiões de borda.
 By: Marco.

- [x] **Sharpening**

 Aplica uma matriz 3x3 que reforça o pixel central e subtrai os pixels vizinhos para gerar uma imagem com mais destaque nos objetos vibrantes.
 By: Marco.

- [x] **Rotate 90º**

 Gira a imagem em 90 graus.
 By: Deyves.

- [x] **Rotate 180º**

 Gira a imagem em 180 graus.
 By: Deyves.

- [x] **Rotate 270º**

 Gira a imagem em 270 graus.
 By: Deyves.


### Funções extras

- [x] **Negative**

 Gera o negativo da imagem, podendo ser aplicado em todos os 3 canais de coloração simultaneamente ou em cada conjunto de 2 ou menos deles.
 By: Marco.

- [x] **Invert Horizontally**

 Espellha a imagem horizontalmente.
 By: Deyves.

- [x] **Invert Vertically**

 Espelha a imagem verticalmente.
 By: Deyves.

- [x] **Changes Quality**

 Aumenta ou reduz a resolução máxima de coloração, dividindo a coloração dos pixels para que se encaixe nessa nova resolução.
 By: Marco.

- [x] **Improves Contrast**

 Encontra os percentis de 0.05 e 0.95 da imagem e com base neles aumenta a dispersão da coloração no espectro da qualidade, tornando as imagens mais nítidas.
 By: Marco.

- [x] **Saturation**

 Converte a imagem do domínio RGB para o domínio HSV para poder alterar as condições de saturação e tornar a imagem mais ou menos vibrante.
 By: Marco.

- [x] **Relay Map**

 Detecta as regiões de borda na imagem e forma um efeito tridimensional que simula uma imagem texturizada.
 By: Marco.

- [x] **Gaussiano**

 Gera uma matriz de convolução com base na equação de Gauss e aplica ela para gerar uma mediana dos pontos e borrar a imagem.
 By: Marco.

- [x] **Sepia**

 Aplica os coeficientes padrão para o filtro Sepia e gera uma imagem dourada/acobreada.
 By: Marco.

- [x] **Bokeh**

 Utiliza os conceitos de thresholding para separar o que é fundo e o que é destaque, aplicando o filtro de sharpening na região de destaque e borrando a região de fundo. Isso gera um efeito similar ao de câmeras duplas no modo retrato.
 By: Marco.

- [x] **Image Composition Analysis**

 Acumula e analisa a composição da imagem e imprime para o usuário.
 By: Marco.

- [x] **Drawing**

 Utiliza uma série de conversões, gerando duas imagens que são, então, misturadas para gerar uma imagem com aparência de desenho feito à mão.
 By: Marco.

- [x] **Dithering**

 Aplica os conceito de espalhamento de erros para gerar uma binarização mais fidedigna para a imagem, sendo que sempre que um ponto é binarizado, a distância dele para o ponto em que é levado é distribuído para os pixels vizinhos, fazendo com que esses se tornem mais propensos a ficarem claros, quando esse erro é positivo, ou escuros, quando o erro é negativo.
 By: Marco.

# Observação

O *`make`* já vai compilar, porém é necessario dar o comando `make mkdir`.

Após a aplicação do filtro o arquivo sera salvo no diretorio res com o nome do efeito aplicado.

A função de escolher outra imagem para aplicação de filtros funciona até tres vezes.

Para usar o `make gdb` é necessario ter o *`gdb`* instalado.

## Não foi implementado

Tentamos montar funções recursivamente, porem foi apontada falha de segmentação, sendo assim, decidimos deixar as funções funcionando interativamente, para garantir a utilização do filtro pelo usuário. Alguns dos extras possíveis não foram aplicados, devido à maior complexidade dos mesmos.
