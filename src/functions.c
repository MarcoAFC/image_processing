#include "functions.h"

//Funcao para escrever o cabecalho da imagem, informacoes com P3, largura e altura, e nivel maximo do pixel
void escreve_cabecalho(FILE* arquivo, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	fprintf(arquivo, "%s\n", tipo_imagem);
	fprintf(arquivo, "%d %d\n", colunas, linhas);
	fprintf(arquivo, "%d\n", qualidade);
}

//lê a imagem.
void ler_imagem(Cores **imagem, char *tipo, int *qualidade, int *colunas, int *linhas)
{	
	//Alocacao para receber o caminho da imagem caso a imagem não esteja na pasta que o programa esta rodando
	char* nome_arquivo = malloc(50*sizeof(char));

	//Declaracao de i e j para poder percorrer a matriz
	int i,j;

	//Direcionando o usuario para o que deve ser feito
	printf("Insira o nome do arquivo: ");
	scanf("%s", nome_arquivo);

	//Abertura do arquivo
	FILE *arquivo = fopen(nome_arquivo,"r+");

	//String para guardar e comparar se existe comentario
	char* str = calloc(1, sizeof(char));
	char* pos;
	int posi = 3;

	//verificando se a imagem que o usuario informou existe 
	if (arquivo == NULL)
	{
		printf("Arquivo nao encontrado");
	}

	//Comparando se exites comentario e guardando a posição dele para que caso não exista, seja feita a leitura normal da imagem
	while (fgets(str, sizeof(str), arquivo) != NULL)
	{
		if (strstr(str, "#") != NULL)
		{
				posi = ftell(arquivo);
				pos = strstr(str, "#");
				printf("Comentario encontrado na posicao: %ld\n", posi - strlen(pos));
				while (fgetc(arquivo) != '\n')
				{
					posi++;
				}
		}
	}

	//Volta para o comeco do arquivo
	rewind(arquivo);

	//Lê o tipo do arquivo, no caso P3
	fscanf(arquivo, "%s", tipo);

	//Pula o comentario
	fseek(arquivo, posi, SEEK_SET);

	//Lê linha e coluna
	fscanf(arquivo, "%d %d", colunas, linhas);

	//Lê o nivel maximo que o pixel pode chegar
	fscanf(arquivo, "%d", qualidade);

	//alterando o tamanho da matriz para o tamanho real dela baseada em linhas e colunas
	imagem = realloc(imagem, *linhas * sizeof(Cores*));
	for(i=0; i < *linhas; i++)
	{
		imagem[i] = realloc(imagem[i], *colunas * sizeof(Cores));
	}

	//Lendo o RGB e prenchendo a matriz de pixel
	for (i = 0; i < *linhas; i++)
	{
		for (j = 0; j < *colunas; j++)
		{
			fscanf(arquivo, "%d %d %d", &imagem[i][j].red, &imagem[i][j].green, &imagem[i][j].blue);

		}
	}

	//Liberando o ponteiro que estava guardando o caminho da imagem
	free(nome_arquivo);

	//Fechando o arquivo
	fclose(arquivo);
}

//transforma em gray scale.
void filtro_cinza(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abrindo um novo arquivo para aplicacao do filtro
	FILE* arquivo = fopen("res/gray.ppm", "w+");

	//Declarando indices para manipulação da matriz
	int i,j;
	
	//Chamada da função para escrever o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);	

	//Criando uma matriz axiliar para receber o filtro para que os dados originais não sejam alterados
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Aplicando filtro de cinza a matriz axuliar e escrevendo direto na imagem
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			aux[i][j].red = (int) (imagem[i][j].red * 0.3) + (imagem[i][j].green * 0.59) + (imagem[i][j].blue * 0.11);
			if(aux[i][j].red > 255)
			{
				aux[i][j].red = 255;
			}
			aux[i][j].green = aux[i][j].red;
			aux[i][j].blue = aux[i][j].green;
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo,"\n");
	}

	//Liberando espaco de memoria que estava sendo ocupado pela matriz auxiliar
	for (i = 0; i < linhas; ++i)
	{
		free(aux[i]);
	}
	free(aux);

	//Fecahndo o arquivo
	fclose(arquivo);
}

//zoom na imagem.
void ampliar(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abertura de um novo arquivo
	FILE* arquivo = fopen("res/amp.ppm", "w+");

	//Variaveis para interação das matrizes
	int fator, k, l, i, j;

	//interação com o usuario para solicitar o fator de ampliação
	printf("Insira o fator de ampliacao da imagem: ");
	scanf("%d", &fator);
	
	//Escrendo cabeçalho na imagem
	escreve_cabecalho(arquivo, colunas * fator, linhas * fator, qualidade, tipo_imagem);

	//Criando matriz auxiliar para aplicação de filtro
	Cores** ampliada = calloc(linhas * fator, sizeof(Cores*));
	for (i = 0; i < linhas * fator; i++)
	{
		ampliada[i] = calloc(colunas * fator, sizeof(Cores));
	}

	//Construindo a matriz ampliada
	for(i = 0, k = 0; i < linhas && k < linhas * fator; k++)
	{
		//Fazendo verificação se o indici da matriz ampliada é divisivel pelo indice da matriz original e verificando se é difente de 0
		if(k % fator == 0 && k != 0)
		{
			i++;
		}
		
		for(j = 0, l = 0; j < colunas && l < colunas * fator;  l++)
		{
			//Fazendo verificação se o indici da matriz ampliada é divisivel pelo indice da matriz original e verificando se é difente de 0
			if(l % fator == 0 && l != 0)
			{
				j++;
			}
			ampliada[k][l].red = imagem[i][j].red;
			ampliada[k][l].green = imagem[i][j].green;
			ampliada[k][l].blue = imagem[i][j].blue;
		}
	}

	//Escrevendo matriz ampliada
	for (i=0; i<linhas * fator; i++)
	{
		for (j = 0; j<colunas * fator; j++)
		{
			fprintf(arquivo, "%d %d %d\n", ampliada[i][j].red, ampliada[i][j].green, ampliada[i][j].blue);

		}
		fprintf(arquivo, "\n");		
	}

	//Liberando espaco eque havia sido alocado para a matriz auxiliar
	for (i = 0; i < linhas; ++i)
	{
		free(ampliada[i]);
	}
	free(ampliada);

}

//gira 90º.
void gira_90 (Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Criando o arquivo
	FILE* arquivo = fopen("res/g90.ppm", "w+");
	
	int i,j;

	//Escrevendo o cabecalho no arquivo
	escreve_cabecalho(arquivo, linhas, colunas, qualidade, tipo_imagem);

	//Criando matriz auxiliar
	Cores **aux = malloc(colunas * sizeof(Cores*));
	for(i = 0; i < colunas; i++)
	{
		aux[i] = malloc(linhas * sizeof(Cores));
	}

	//Preenchendo a matriz auxiliar
	for(i = 0; i < linhas; i++)
	{
		for(j = 0; j < colunas; j ++)
		{
			aux[j][i].red = imagem[i][j].red;
			aux[j][i].green = imagem[i][j].green;
			aux[j][i].blue = imagem[i][j].blue;
		}
	}

	//Escrendo a matriz auxiliar no arqquivo
	for(i = 0; i < colunas; i++)
	{
		for(j = (linhas - 1); j >= 0; j--)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);
		}
	}

	//Liberando o espaco que havia sido alocado
	for(i = 0; i < colunas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando o arquivo
	fclose(arquivo);
}

//gira 180º.
void gira_180 (Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abrindo o arquivo 
	FILE* arquivo = fopen("res/g180.ppm", "w+");
	
	int i,j;

	//Escreve o cabecalho na imagem
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Escrevendo no arquivo 
	for(i = (linhas - 1); i >= 0; --i)
	{
		for(j = (colunas - 1); j >= 0; --j)
		{
			fprintf(arquivo, "%d %d %d \n", imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
		}
	}

	//Fecahando o arquivo
	fclose(arquivo);
}

//gira 270º.
void gira_270 (Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abrindo arquivo
	FILE* arquivo = fopen("res/g270.ppm", "w+");
	
	int i,j;

	//Escreve o cabecalho no arquivo 
	escreve_cabecalho(arquivo, linhas, colunas, qualidade, tipo_imagem);

	//Criando matriz axiliar
	Cores **aux = malloc(colunas * sizeof(Cores*));
	for(i = 0; i < colunas; i++)
	{
		aux[i] = malloc(linhas * sizeof(Cores));
	}

	//Preenchendo a matriz auxiliar 
	for(i = 0; i < linhas; i++)
	{
		for(j = 0; j < colunas; j ++)
		{
			aux[j][i].red = imagem[i][j].red;
			aux[j][i].green = imagem[i][j].green;
			aux[j][i].blue = imagem[i][j].blue;
		}
	}

	//Escrevendo no arquivo
	for(i = (colunas - 1); i >= 0; i--)
	{
		for(j = 0; j < linhas; j++)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);
		}
	}

	//Liberando espaco de memoria que havia sido resevado
	for(i = 0; i < colunas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando o arquivo
	fclose(arquivo);
}

void thresholding(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//binariza a imagem, separando fundo de imagem principal utilizando o metodo de Otsu
	
	//Abrind o arquivo 
	FILE* arquivo = fopen("res/thr.ppm", "w+");

	//Redefinindo qualidade do pixel
	int nova_qualidade = 1;

	//Escrendo cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, nova_qualidade, tipo_imagem);
	int tmp, i, j, valor_otsu = qualidade/2;

	//Alocando variaveis 
	int* histograma = calloc(qualidade, sizeof(int));
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Interacao com o usuario para aplicacao do filtro
	int opcao;
	printf("\nSe voce deseja utilizar o threshold automatico (Sistema de Otsu), digite 0.\nSe deseja usar um threshold manual, digite 1.\nEscolha: ");
	scanf("%d", &opcao);

	//Verificando se o usuario desejou aplicar automaticamente, caso sim entra neste condicional
	
	//Monta um histograma com as frequencias pra cada media de intensidade
	if (opcao == 0)
	{
		for (i = 0; i < linhas; i++)
		{
			for (j = 0; j < colunas; j++)
				{
					tmp = (imagem[i][j].red + imagem[i][j].blue + imagem[i][j].green)/3;
					histograma[tmp]+=1;
				}
		}

		//encontra a menor frequencia e usa como valor para diferenciar

		int menor_i = 255;

		for (i = 0; i < qualidade; i++)
		{
			if (histograma[i]>0 && histograma[i]<histograma[menor_i])
			{
				menor_i = i;
				valor_otsu = histograma[i];
			}
		}
	}

	//Caso contrario, estamos solicitanos o valor de Threshold a ser aplicado
	else
	{
		printf("\nInsira o valor de threshold desejado: ");
		scanf("%d", &valor_otsu);
	}

	//Fazendo operações para aplicação de filtros 
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < colunas; j++)
			{
				if ((imagem[i][j].red+imagem[i][j].blue+imagem[i][j].green)/3 > valor_otsu)
				{
					aux[i][j].red = nova_qualidade;
					aux[i][j].blue = nova_qualidade;
					aux[i][j].green = nova_qualidade;
				}
				else
				{
					aux[i][j].red = 0;
					aux[i][j].blue = 0;
					aux[i][j].green = 0;
				}
			}
	}

	//Escrevendo na imagem
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].blue, aux[i][j].green);

		}
		fprintf(arquivo, "\n");
	}

	//Liberando espaço de memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);
	free(histograma);

	//Fecahndo o arquivo
	fclose(arquivo);
}

//Filtro de borrar
void blurring(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abrindo o arquivo
	FILE* arquivo = fopen("res/blur.ppm", "w+");
	
	int i,j, v;

	//Escrevendo o cabecalho na imagem
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Interacao para saber o quento o usuario deseja borrar
	printf("Insira a quantidade de vezes que deseja borrar: ");
	scanf("%d", &v);

	//Criando matriz auxiliar para aplicacao do filtro
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Preenchendo a matriz auxilar
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < (colunas); j++)
			{
				aux[i][j] = imagem[i][j];
			}
	}

	//Entrando em um laco para aplicar o efeito, a quantidade de vezes que o usuario deseja borrar a imagem
	while (v>0)
	{
		for (i = 0; i < linhas; i++)
		{
			for (j = 0; j < colunas; j++)
				{
					if (i == 0 && j == 0)
					{
						aux[i][j].red = (aux[i][j].red + aux[i][j+1].red + aux[i+1][j].red + aux[i+1][j+1].red)/9.0;
						aux[i][j].green = (aux[i][j].green + aux[i][j+1].green + aux[i+1][j].green + aux[i+1][j+1].green)/9.0;
						aux[i][j].blue = (aux[i][j].blue + aux[i][j+1].blue + aux[i+1][j].blue + aux[i+1][j+1].blue)/9.0;

					}
					else if (i == linhas - 1 && j == 0)
					{
						aux[i][j].red = (aux[i-1][j].red + aux[i-1][j+1].red + aux[i][j].red + aux[i][j+1].red)/9.0;
						aux[i][j].green = (aux[i-1][j].green + aux[i-1][j+1].green + aux[i][j].green + aux[i][j+1].green)/9.0;
						aux[i][j].blue = (aux[i-1][j].blue + aux[i-1][j+1].blue + aux[i][j].blue + aux[i][j+1].blue)/9.0;
					}
					else if (i == 0 && j == colunas - 1)
					{
						aux[i][j].red = (aux[i][j-1].red + aux[i][j].red+ aux[i+1][j-1].red + aux[i+1][j].red)/9.0;
						aux[i][j].green = (aux[i][j-1].green + aux[i][j].green+ aux[i+1][j-1].green + aux[i+1][j].green)/9.0;
						aux[i][j].blue = (aux[i][j-1].blue + aux[i][j].blue+ aux[i+1][j-1].blue + aux[i+1][j].blue)/9.0;

					}
					else if (i == linhas - 1 && j == colunas - 1)
					{
						aux[i][j].red = (aux[i-1][j-1].red + aux[i-1][j].red + aux[i][j-1].red + aux[i][j].red)/9.0;
						aux[i][j].green = (aux[i-1][j-1].green + aux[i-1][j].green + aux[i][j-1].green + aux[i][j].green)/9.0;
						aux[i][j].blue = (aux[i-1][j-1].blue + aux[i-1][j].blue + aux[i][j-1].blue + aux[i][j].blue)/9.0;

					}
					else if (i == 0)
					{
						aux[i][j].red = (aux[i][j-1].red + aux[i][j].red + aux[i][j+1].red
									+ aux[i+1][j-1].red + aux[i+1][j].red + aux[i+1][j+1].red)/9.0;
						aux[i][j].blue = (aux[i][j-1].blue + aux[i][j].blue + aux[i][j+1].blue
									+ aux[i+1][j-1].blue + aux[i+1][j].blue + aux[i+1][j+1].blue)/9.0;
						aux[i][j].green = (aux[i][j-1].green + aux[i][j].green + aux[i][j+1].green
									+ aux[i+1][j-1].green + aux[i+1][j].green + aux[i+1][j+1].green)/9.0;
					}
					else if (j == 0)
					{
						aux[i][j].red = (aux[i-1][j].red + aux[i-1][j+1].red
									+ aux[i][j].red + aux[i][j+1].red
									+ aux[i+1][j].red + aux[i+1][j+1].red)/9.0;
						aux[i][j].blue = (aux[i-1][j].blue + aux[i-1][j+1].blue
									+ aux[i][j].blue + aux[i][j+1].blue
									+ aux[i+1][j].blue + aux[i+1][j+1].blue)/9.0;
						aux[i][j].green = (aux[i-1][j].green + aux[i-1][j+1].green
									+ aux[i][j].green + aux[i][j+1].green
									+ aux[i+1][j].green + aux[i+1][j+1].green)/9.0;

					}
					else if (j == colunas - 1)
					{
						aux[i][j].red = (aux[i-1][j-1].red + aux[i-1][j].red
									+ aux[i][j-1].red + aux[i][j].red
									+ aux[i+1][j-1].red + aux[i+1][j].red)/9.0;
						aux[i][j].blue = (aux[i-1][j-1].blue + aux[i-1][j].blue
									+ aux[i][j-1].blue + aux[i][j].blue
									+ aux[i+1][j-1].blue + aux[i+1][j].blue)/9.0;
						aux[i][j].green = (aux[i-1][j-1].green + aux[i-1][j].green
									+ aux[i][j-1].green + aux[i][j].green
									+ aux[i+1][j-1].green + aux[i+1][j].green)/9.0;
					}
					else if (i == linhas - 1)
					{
						aux[i][j].red = (aux[i-1][j-1].red + aux[i-1][j].red + aux[i-1][j+1].red
									+ aux[i][j-1].red + aux[i][j].red + aux[i][j+1].red)/9.0;
						aux[i][j].blue = (aux[i-1][j-1].blue + aux[i-1][j].blue + aux[i-1][j+1].blue
									+ aux[i][j-1].blue + aux[i][j].blue + aux[i][j+1].blue)/9.0;
						aux[i][j].green = (aux[i-1][j-1].green + aux[i-1][j].green + aux[i-1][j+1].green
									+ aux[i][j-1].green + aux[i][j].green + aux[i][j+1].green)/9.0;
					}
					else {
						aux[i][j].red = (aux[i-1][j-1].red + aux[i-1][j].red + aux[i-1][j+1].red
									+ aux[i][j-1].red + aux[i][j].red + aux[i][j+1].red
									+ aux[i+1][j-1].red + aux[i+1][j].red + aux[i+1][j+1].red)/9.0;
						aux[i][j].blue = (aux[i-1][j-1].blue + aux[i-1][j].blue + aux[i-1][j+1].blue
									+ aux[i][j-1].blue + aux[i][j].blue + aux[i][j+1].blue
									+ aux[i+1][j-1].blue + aux[i+1][j].blue + aux[i+1][j+1].blue)/9.0;
						aux[i][j].green = (aux[i-1][j-1].green + aux[i-1][j].green + aux[i-1][j+1].green
									+ aux[i][j-1].green + aux[i][j].green + aux[i][j+1].green
									+ aux[i+1][j-1].green + aux[i+1][j].green + aux[i+1][j+1].green)/9.0;
					}
				}
		}
		v--;
	}

	//Escrevendo o resultado na imagem 
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Liberando espaco de memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fecahando arquivo
	fclose(arquivo);	
}

//Filtro de cores negativas
void negative(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abrindo arquivo
	FILE* arquivo = fopen("res/neg.ppm", "w+");
	int i,j, opcao;
	//Escrevendo cabecalho no arquivo 
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Mostrando as opcoes que o usuario tem para aplicacao do filtro
	printf("Escolha o estilo de negativizacao entre os seguintes: \n");
	printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");	
	printf("║                                                                                ║\n");
	printf("║ 1  - Negativo tradicional (todas as cores).                                    ║\n");
	printf("║ 2  - Negativo APENAS do vermelho.                                              ║\n");
	printf("║ 3  - Negativo APENAS do verde.                                                 ║\n");
	printf("║ 4  - Negativo APENAS do azul.                                                  ║\n");
	printf("║ 5  - Negativo do vermelho e verde.                                             ║\n");
	printf("║ 6  - Negativo do vermelho e azul.                                              ║\n");
	printf("║ 7  - Negativo do verde e azul                                                  ║\n");
	printf("║                                                                                ║\n");
	printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");

	//Recebendo escolha do usuario
	scanf("%d", &opcao);

	//Verificando a escolha do usuario
	switch (opcao)
	{
		case 1:
		
		//Faz o negativo de todas as cores.
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", qualidade-imagem[i][j].red,qualidade-imagem[i][j].green, qualidade-imagem[i][j].blue);
					}	
			}
			break;

		case 2:
		
		//Faz o negativo da vermelha
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", qualidade-imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
					}	
			}
			break;

		case 3:
		
		//Faz o negativo da verde
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", imagem[i][j].red, qualidade-imagem[i][j].green, imagem[i][j].blue);
					}	
			}
			break;

		case 4:

		//Faz o negativo da azul
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", imagem[i][j].red, imagem[i][j].green, qualidade-imagem[i][j].blue);
					}	
			}
			break;

		case 5:

		//Faz o negativo do vermelho e verde
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", qualidade-imagem[i][j].red, qualidade-imagem[i][j].green, imagem[i][j].blue);
					}	
			}
			break;

		case 6:

		//Faz o negativo do vermelho e azul
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", qualidade-imagem[i][j].red, imagem[i][j].green, qualidade-imagem[i][j].blue);
					}
			}
			break;

		case 7:

		//Faz negativo do verde e azul
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", imagem[i][j].red,qualidade-imagem[i][j].green, qualidade-imagem[i][j].blue);
					}	
			}
			break;

		default:

		//Usuario escolheu alo fora do escopo, entao foi aplicado filtro tradicional
			printf("Opcao invalida, aplicando o filtro tradicional.\n");
			for (i = 0; i < linhas; i++)
			{
				for (j = 0; j < colunas; j++)
					{
						fprintf(arquivo, "%d %d %d\n", qualidade-imagem[i][j].red,qualidade-imagem[i][j].green, qualidade-imagem[i][j].blue);
					}	
			}
			break;
	}

	//Fechando arquivo
	fclose(arquivo);
}

//Espelha a imagem horizontalmente
void invert_hor (Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Inverte a imagem
	FILE* arquivo = fopen("res/inv_hor.ppm", "w+");
	
	int i,j;

	//Escreve o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Escreve o RGB na imagem
	for(i = 0; i < linhas; i++)
	{
		for(j = (colunas - 1); j >= 0; j--)
		{
			fprintf(arquivo, "%d %d %d\n", imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
		}
	}

	//Fecha o arquivo
	fclose(arquivo);
}

//Inverte a imagem verticalmente
void invert_ver(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abre o arquivo
	FILE* arquivo = fopen("res/inv_ver.ppm", "w+");

	int i,j;

	//Escreve o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Escreve o RGB
	for(i = linhas-1; i >= 0; i--)
	{
		for(j = 0; j < colunas; j++)
		{
			fprintf(arquivo, "%d %d %d \n", imagem[i][j].red, imagem[i][j].green, imagem[i][j].blue);
		}
	}

	//Fecha o arquivo
	fclose(arquivo);
}

//Almenta os detalhes
void sharpening(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Abre o arquivo
	FILE* arquivo = fopen("res/sharp.ppm", "w+");
	
	int i,j;
	int v;

	//Escreve o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);
	printf("Insira quantas vezes voce deseja afiar: ");
	scanf("%d", &v);

	//Matrizes auxiliares para aplicacao do filtro
	Cores** aux = calloc(linhas, sizeof(Cores*));
	Cores** aux2 = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
		aux2[i] = calloc(colunas, sizeof(Cores));
	}

	//Igualando a segunda matriz auxiliar a matriz original
	for(i = 0; i < linhas; i++)
	{
		for(j = 0; j < colunas; j++)
		{
			aux2[i][j] = imagem[i][j];
		}
	}

	//Fazendo a primeira matriz receber as operações feitas com a matriz auxiliar a quantidade de vezes que o usuario desejar aplicar o efeito
	while (v>0)
	{
		for (i = 1; i < linhas-1; i++)
		{
			for (j = 1; j < colunas-1; j++)
			{
				aux[i][j].red = (-aux2[i-1][j].red
								 -aux2[i][j-1].red + 5*aux2[i][j].red - aux2[i][j+1].red
								 -aux2[i+1][j].red);

				aux[i][j].blue = (-aux2[i-1][j].blue
								  -aux2[i][j-1].blue + 5*aux2[i][j].blue - aux2[i][j+1].blue
								  -aux2[i+1][j].blue);

				aux[i][j].green = (-aux2[i-1][j].green
								   -aux2[i][j-1].green + 5*aux2[i][j].green - aux2[i][j+1].green
								   -aux2[i+1][j].green);

				//Verificando se o red do RGB é menor que 0, caso seja o igualamos a 0. E o mesmo serve para as cores do RGB.
				if (aux[i][j].red < 0)
				{
					aux[i][j].red = 0;
				}

				//Verifica se o red do RGB é maior que 255, caso seja o igualamos a 255. E o mesmo serve para as outras cores do RGB.
				else if (aux[i][j].red > qualidade)
				{
					aux[i][j].red = qualidade;
				}


				if (aux[i][j].green < 0)
				{
					aux[i][j].green = 0;
				}
				else if (aux[i][j].green > qualidade)
				{
					aux[i][j].green = qualidade;
				}
				if (aux[i][j].blue < 0)
				{
					aux[i][j].blue = 0;
				}
				else if (aux[i][j].blue > qualidade)
				{
					aux[i][j].blue = qualidade;
				}
				
			}
		}
		v--;

		//Iguala a segunda matriz auxiliar a primeira matriz auxiliar
		for(i = 0; i < linhas; i++)
		{
			for(j = 0; j < colunas; j++)
			{
				aux2[i][j] = aux[i][j];
			}
		}
	}

	//Escrevendo no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Liberando espaco que estava alocado
	for (i = 0; i < linhas; i++)
	{
		free(aux2[i]);
		free(aux[i]);
	}
	free(aux2);
	free(aux);

	//Fechando arquivo
	fclose(arquivo);	
}

//Melhora a qualidade
void mod_qualidade(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{

	//Criando arquivo
	FILE* arquivo = fopen("res/quali.ppm", "w+");

	//Declarando variaveis para aplicacao de filtro
	int i,j, nova_quali;

	//Interação com o usuario
	printf("Insira a nova resolucao de cor desejada (ex: 64, 128, 256): \n");
	scanf("%d", &nova_quali);

	//Calculando o novo fator para aplicacao do filtro
	float fator = (float)(qualidade+1)/(float)nova_quali;
	nova_quali--;

	//Escrevendo o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, nova_quali, tipo_imagem);
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%.0f %.0f %.0f\n", (float)imagem[i][j].red/fator, (float)imagem[i][j].green/fator, (float)imagem[i][j].blue/fator);

		}
		fprintf(arquivo,"\n");
	}

	//Fechando o arquivo
	fclose(arquivo);
}

//Reduzir a imagem
void reduzir(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	
		//Abindo arquivo
	FILE* arquivo = fopen("res/reduc.ppm", "w+");

	//Definindo variaveis para aplicacao de filtro
	int i,j, fator, col_red, lin_red;

	//Interacao com o usuario
	printf("Insira o fator de reducao da imagem (obs: 2 reduz pela metade, 4 reduz para 1/4): ");
	scanf("%d", &fator);

	//Definindo novo tamanho das colunas e linhas 
	col_red = colunas/fator;
	lin_red = linhas/fator;

	//Escrevendo cabecalho
	escreve_cabecalho(arquivo, col_red, lin_red, qualidade, tipo_imagem);

	//Alocando matriz para aplicacao de filtro
	Cores** aux = calloc(lin_red, sizeof(Cores*));
	for(i = 0; i < lin_red; i++)
	{
		aux[i] = calloc(col_red, sizeof(Cores));
	}

	//Fazendo a matriz auxiliar receber as operacoes e escrevendo no arquivo
	for (i = 0; i < lin_red; ++i)
	{
		for (j = 0; j < col_red; ++j)
		{
			aux[i][j].red = (imagem[fator*i][fator*j].red + imagem[fator*i+1][fator*j].red + imagem[fator*i][fator*j+1].red + imagem[fator*i+1][fator*j+1].red)/4;  
			aux[i][j].blue = (imagem[fator*i][fator*j].blue + imagem[fator*i+1][fator*j].blue + imagem[fator*i][fator*j+1].blue + imagem[fator*i+1][fator*j+1].blue)/4;  
			aux[i][j].green = (imagem[fator*i][fator*j].green + imagem[fator*i+1][fator*j].green + imagem[fator*i][fator*j+1].green + imagem[fator*i+1][fator*j+1].green)/4;  
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);
		}
		fprintf(arquivo,"\n");
	}

	//Liberando memoria antes alocada
	for (i = 0; i < lin_red; ++i)
	{
		free(aux[i]);
	}
	free(aux);
}

//Deteccao de bordas
void bordas(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{

	//Criando arquivo
	FILE* arquivo = fopen("res/bordas.ppm", "w+");
	
	int i,j;

	//Escrevendo cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Alocando memoria
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Fazendo a matriz auxiliar receber as operacoes da aolicacao do filtro
	for (i = 1; i < linhas-1; i++)
	{
		for (j = 1; j < (colunas-1); j++)
			{
				aux[i][j].red = (imagem[i-1][j-1].red  - imagem[i-1][j+1].red
								 - imagem[i+1][j-1].red + imagem[i+1][j+1].red);
				aux[i][j].blue = (imagem[i-1][j-1].blue  - imagem[i-1][j+1].blue
								  - imagem[i+1][j-1].blue + imagem[i+1][j+1].blue);
				aux[i][j].green = (imagem[i-1][j-1].green  - imagem[i-1][j+1].green
								   - imagem[i+1][j-1].green + imagem[i+1][j+1].green);
				
				aux[i][j].red = (+ imagem[i-1][j].red 
								+ imagem[i][j-1].red - 4*imagem[i][j].red + imagem[i][j+1].red
								 + imagem[i+1][j].red );
				aux[i][j].blue = (+ imagem[i-1][j].blue 
								+ imagem[i][j-1].blue - 4*imagem[i][j].blue + imagem[i][j+1].blue
								 + imagem[i+1][j].red );
				aux[i][j].green = (+ imagem[i-1][j].green 
								+ imagem[i][j-1].green - 4*imagem[i][j].green + imagem[i][j+1].green
								 + imagem[i+1][j].green );

				aux[i][j].red = (-imagem[i-1][j-1].red - imagem[i-1][j].red - imagem[i-1][j+1].red
								- imagem[i][j-1].red + 8*imagem[i][j].red - imagem[i][j+1].red
								- imagem[i+1][j-1].red - imagem[i+1][j].red - imagem[i+1][j+1].red);
				aux[i][j].blue = (-imagem[i-1][j-1].blue - imagem[i-1][j].blue - imagem[i-1][j+1].blue
								- imagem[i][j-1].blue + 8*imagem[i][j].blue - imagem[i][j+1].blue
								- imagem[i+1][j-1].blue - imagem[i+1][j].blue - imagem[i+1][j+1].blue);
				aux[i][j].green = (-imagem[i-1][j-1].green - imagem[i-1][j].green - imagem[i-1][j+1].green
								- imagem[i][j-1].green + 8*imagem[i][j].green - imagem[i][j+1].green
								- imagem[i+1][j-1].green - imagem[i+1][j].green - imagem[i+1][j+1].green);

				//Verifica se red e as outras cores do RGB sao menores que 0, caso seja igualamos a 0.
				if (aux[i][j].red < 0)
				{
					aux[i][j].red = 0;
				}

				//Verifica se red e as outras cores sao maiores que 255,caso sejam igualamos a 255.
				else if (aux[i][j].red > qualidade)
				{
					aux[i][j].red = qualidade;
				}

				if (aux[i][j].green < 0)
				{
					aux[i][j].green = 0;
				}

				else if (aux[i][j].green > qualidade)
				{
					aux[i][j].green = qualidade;
				}

				if (aux[i][j].blue < 0)
				{
					aux[i][j].blue = 0;
				}

				else if (aux[i][j].blue > qualidade)
				{
					aux[i][j].blue = qualidade;
				}
			}
	}

	//Escrevendo no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Liberando espaco
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando o arquivo
	fclose(arquivo);	
}

//Almentando o contraste
void contraste(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem){

	//Abrindo o arquivo
	FILE* arquivo = fopen("res/cont_lin.ppm", "w+");

	//Inicializando variaveis
	int i,j,k, tmp=0;
	
	//Escrevend cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	int histograma[3][qualidade];

	//Alocando espaco
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Cosntroi matriz de histograma
	for (k = 0; k<3; k++)
	{
		for (i = 0; i < qualidade; i++)
		{
			histograma[k][i] = 0;
		}
	}
	//monta um histograma com as frequencias pra cada media de intensidade
	for (k = 0; k<3; k++)
	{
		for (i = 0; i < linhas; i++)
		{
			for (j = 0; j < colunas; j++)
				{
					if (k==0)
					{
						tmp = imagem[i][j].red;
					}
					if (k==1)
					{
						tmp = imagem[i][j].green;
					}
					if (k==2)
					{
						tmp = imagem[i][j].blue;
					}
					histograma[k][tmp]+=1;
				}
		}
	}
	
	//encontra o menor e o maior i nao nulo
	int* menor_i = calloc(3, sizeof(int));
	int* maior_i = calloc(3, sizeof(int));
	int acum;

	for (k = 0; k < 3; k++)
	{
		acum = 0;
		for (i = 0; i < qualidade; i++)
		{
			acum+=histograma[k][i];
			if (acum >= (0.05*linhas*colunas) && menor_i[k] == 0)
			{
				menor_i[k] = i;
			}
			if (acum >= (0.95*linhas*colunas) && maior_i[k] == 0)
			{
				maior_i[k] = i;
			}
		}
	}

	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			aux[i][j].red = ((float)(imagem[i][j].red-menor_i[0])/(maior_i[0]-menor_i[0]))*qualidade;
			aux[i][j].green = ((float)(imagem[i][j].green-menor_i[1])/(maior_i[1]-menor_i[1]))*qualidade;
			aux[i][j].blue = ((float)(imagem[i][j].blue-menor_i[2])/(maior_i[2]-menor_i[2]))*qualidade;
			if (aux[i][j].red < 0)
			{
				aux[i][j].red = 0;
			}
			else if (aux[i][j].red > qualidade)
			{
				aux[i][j].red = qualidade;
			}
			if (aux[i][j].green < 0)
			{
				aux[i][j].green = 0;
			}
			else if (aux[i][j].green > qualidade)
			{
				aux[i][j].green = qualidade;
			}
			if (aux[i][j].blue < 0)
			{
				aux[i][j].blue = 0;
			}
			else if (aux[i][j].blue > qualidade)
			{
				aux[i][j].blue = qualidade;
			}
		}
	}

	//Escreve no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo,"\n");
	}

	//Libera espaco
	for (i = 0; i < linhas; ++i)
	{
		free(aux[i]);
	}
	free(aux);

	fclose(arquivo);

}

//Altera a saturacao da imagem
void saturacao(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem){
	
	//Cria arquivo
	FILE* arquivo = fopen("res/sat.ppm", "w+");

	int i,j, max, min;
	float fator = 0.001;

	//Escrevendo cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Alocando espaco
	Cores** aux = calloc(linhas, sizeof(Cores*));
	HSV** hsv = calloc(linhas, sizeof(HSV*));


	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
		hsv[i] = calloc(colunas, sizeof(HSV));
	}

	//Converte de RGB para HSV
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			double h = 0, s, v;
			double diff;
 			min = imagem[i][j].red;

			if (min > imagem[i][j].green)
			{
				min = imagem[i][j].green;
			}

			if (min > imagem[i][j].blue)
			{
				min = imagem[i][j].blue;
			}

			v = imagem[i][j].red;

			if (v < imagem[i][j].green)
			{
				v = imagem[i][j].green;
			}

			if (v < imagem[i][j].blue)
			{
				v = imagem[i][j].blue;

			}

			diff = v-min;

			if (v == 0)
			{
				s = 0;
			}
			
			else{
				s = diff/(double)v;
			}

			if (s == 0)
			{
				h = 0.0;
			}

			else{
				if (v == imagem[i][j].red)
				{
					h = (imagem[i][j].green - imagem[i][j].blue)/diff;
				}

				else if (v == imagem[i][j].green)
				{
					h = (imagem[i][j].blue - imagem[i][j].red)/diff;
				}

				else if (v == imagem[i][j].blue)
				{
					h = (imagem[i][j].red - imagem[i][j].green)/diff;
				}

				h*=60;

				if (h<0)
				{
					h+=360;
				}
			}

			hsv[i][j].h = h;

			hsv[i][j].s = s;

			hsv[i][j].v = v;
		}
	}

	//Efetua a aprimoracao de saturacao no HSV
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			hsv[i][j].s = hsv[i][j].s;
		}
	}

	//transcreve de volta para RGB
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			if (hsv[i][j].s == 0)
			{
				aux[i][j].red = hsv[i][j].v;

				aux[i][j].green = hsv[i][j].v;

				aux[i][j].blue = hsv[i][j].v;
			}

			else
			{
				int m;
				double f,p,q,t;

				if((hsv[i][j].h) == 360)
				{
					hsv[i][j].h = 0;
				}

				else
				{
					hsv[i][j].h = hsv[i][j].h/60;
				}

				m = (int) trunc(hsv[i][j].h);
				f = hsv[i][j].h - m;
				p = hsv[i][j].v * (1.0 - hsv[i][j].s);
				q = hsv[i][j].v * (1.0 - (hsv[i][j].s * f));
				t = hsv[i][j].v * (1.0 - (hsv[i][j].s * (1.0-f)));

				switch(m)
			{

					case 0:

						aux[i][j].red = (int)hsv[i][j].v;
						aux[i][j].green = t;
						aux[i][j].blue = p;

						break;

					case 1:

						aux[i][j].red = q;
						aux[i][j].green = (int)hsv[i][j].v;
						aux[i][j].blue = p;

						break;

					case 2:

						aux[i][j].red = p;
						aux[i][j].green = (int)hsv[i][j].v;
						aux[i][j].blue = t;

						break;

					case 3:

						aux[i][j].red = p;
						aux[i][j].green = q;
						aux[i][j].blue = (int)hsv[i][j].v;

						break;

					case 4:

						aux[i][j].red = t;
						aux[i][j].green = p;
						aux[i][j].blue = (int)hsv[i][j].v;

						break;

					default:

						aux[i][j].red = (int)hsv[i][j].v;
						aux[i][j].green = p;
						aux[i][j].blue = q;

						break;
				}

				if (aux[i][j].red < 0)
				{
					aux[i][j].red = 0;
				}

				else if (aux[i][j].red > qualidade)
				{
					aux[i][j].red = qualidade;
				}

				if (aux[i][j].green < 0)
				{
					aux[i][j].green = 0;
				}

				else if (aux[i][j].green > qualidade)
				{
					aux[i][j].green = qualidade;
				}

				if (aux[i][j].blue < 0)
				{
					aux[i][j].blue = 0;
				}
				
				else if (aux[i][j].blue > qualidade)
				{
					aux[i][j].blue = qualidade;
				}
			}
		}
	}

	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{

			fprintf(arquivo, "%d %d %d\n", aux[i][j].blue, aux[i][j].red, aux[i][j].green);

		}
		fprintf(arquivo,"\n");
	}

	//Desalocando memoria
	for (i = 0; i < linhas; ++i)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando o arquivo
	fclose(arquivo);

}


void emboss(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem){

	//Criando arquivo
	FILE* arquivo = fopen("res/relevo.ppm", "w+");
	
	int i,j;

	//Escrevendo cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Alocando memoria
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Operacoes para aplicar o filtro na matriz auxiliar
	for (i = 1; i < linhas-1; i++)
	{
		for (j = 1; j < (colunas-1); j++)
		{
			aux[i][j].red = (-imagem[i-1][j-1].red - imagem[i-1][j].red
							 -imagem[i][j-1].red + imagem[i][j+1].red
							 +imagem[i+1][j].red + imagem[i+1][j+1].red)+128;

			aux[i][j].green = (-imagem[i-1][j-1].green - imagem[i-1][j].green
							 -imagem[i][j-1].green + imagem[i][j+1].green
							 +imagem[i+1][j].green + imagem[i+1][j+1].green)+128;

			aux[i][j].blue = (-imagem[i-1][j-1].blue - imagem[i-1][j].blue
							 -imagem[i][j-1].blue + imagem[i][j+1].blue
							 +imagem[i+1][j].blue + imagem[i+1][j+1].blue)+128;

			//Verifica se o red e as outras cores sao menores que 0, caso sejam igualamos a 0.
			if (aux[i][j].red < 0)
			{
				aux[i][j].red = 0;
			}

			//Verifica se o red e as outras cores sao maiores que 255, caso sejam igualamos a 255.
			else if (aux[i][j].red > qualidade)
			{
				aux[i][j].red = qualidade;
			}

			if (aux[i][j].green < 0)
			{
				aux[i][j].green = 0;
			}

			else if (aux[i][j].green > qualidade)
			{
				aux[i][j].green = qualidade;
			}

			if (aux[i][j].blue < 0)
			{
				aux[i][j].blue = 0;
			}

			else if (aux[i][j].blue > qualidade)
			{
				aux[i][j].blue = qualidade;
			}
		}
	}

	//Escrevendo na imagem
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}
	
	//Desalocando memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando arquivo
	fclose(arquivo);	
}

//Filtro de gauss
void gaussiano(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Cria arquivo
	FILE* arquivo = fopen("res/gauss.ppm", "w+");
	int i,j, v;

	//Variaveis para manipulacao do arquivo
	double matriz[3][3];
	double mediana = 1.5;
	double soma = 0.0;
	double dp = 1.0;
	
	//Fazendo operacoes com a matriz criada
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			{
				matriz[i][j] = exp(-0.5*pow((i-mediana)/dp, 2.0) + pow((j-mediana)/dp, 2.0))/(2*M_PI*dp*dp);
				soma += matriz[i][j];
			}
	}

	//Alterando o conteudo da matriz criada
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			{
				matriz[i][j] = matriz[i][j]/soma;
			}
	}

	//Escrevendo o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Interacao com o usuario
	printf("Insira a quantidade de vezes que deseja borrar: ");
	scanf("%d", &v);

	//Alocacao dinamica
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Igualando matriz auxiliar a matriz original
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < (colunas); j++)
			{
				aux[i][j] = imagem[i][j];
			}
	}

	//Realizando operacoes para aplicacao do filtro a quantidade de vezes que o usuario desejar
	while (v>0)
	{
		for (i = 1; i < linhas-1; i++)
		{
			for (j = 1; j < (colunas-1); j++)
				{
					aux[i][j].red = (matriz[0][0]*aux[i-1][j-1].red + matriz[0][1]*aux[i-1][j].red + matriz[0][2]*aux[i-1][j+1].red
									+ matriz[1][0]*aux[i][j-1].red + matriz[1][1]*aux[i][j].red + matriz[1][2]*aux[i][j+1].red
									+ matriz[2][0]*aux[i+1][j-1].red + matriz[2][1]*aux[i+1][j].red + matriz[2][2]*aux[i+1][j+1].red);

					aux[i][j].blue = (matriz[0][0]*aux[i-1][j-1].blue + matriz[0][1]*aux[i-1][j].blue + matriz[0][2]*aux[i-1][j+1].blue
									+ matriz[1][0]*aux[i][j-1].blue + matriz[1][1]*aux[i][j].blue + matriz[1][2]*aux[i][j+1].blue
									+ matriz[2][0]*aux[i+1][j-1].blue + matriz[2][1]*aux[i+1][j].blue + matriz[2][2]*aux[i+1][j+1].blue);

					aux[i][j].green = (matriz[0][0]*aux[i-1][j-1].green + matriz[0][1]*aux[i-1][j].green + matriz[0][2]*aux[i-1][j+1].green
									+ matriz[1][0]*aux[i][j-1].green + matriz[1][1]*aux[i][j].green + matriz[1][2]*aux[i][j+1].green
									+ matriz[2][0]*aux[i+1][j-1].green + matriz[2][1]*aux[i+1][j].green + matriz[2][2]*aux[i+1][j+1].green);
				}
		}
		v--;
	}

	//Escrevend no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Desalocacao de memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando arquivo
	fclose(arquivo);	
}

//faz dispersao de erro para representar melhor a imagem binarizada
void dithering(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Criando arquivo	
	FILE* arquivo = fopen("res/dithering.ppm", "w+");
	
	//Escreve cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Criando variaveis para manipulacao de informacoes
	int tmp, i, j, valor_otsu = qualidade/2;
	int* histograma = calloc(qualidade, sizeof(int));
	Cores erro;
	erro.red = 0;
	erro.green = 0;
	erro.blue = 0;

	//Alocando memoria
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Fazendo a matriz auxiliar receber a matriz original e realizando media
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < colunas; j++)
			{
				aux[i][j] = imagem[i][j];
				tmp = (imagem[i][j].red + imagem[i][j].blue + imagem[i][j].green)/3;
				histograma[tmp]+=1;
			}
	}


	//encontra a menor frequencia e usa como valor para diferenciar
	int menor_i = 255;

	for (i = 0; i < qualidade; i++)
	{
		if (histograma[i]>0 && histograma[i]<histograma[menor_i])
		{
			menor_i = i;
			valor_otsu = histograma[i];
		}
	}

	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < colunas; j++)
			{
				//verifica se nenhum valor se encontra fora dos limites
				if (aux[i][j].red < 0)
				{
					aux[i][j].red = 0;
				}

				else if (aux[i][j].red > qualidade)
				{
					aux[i][j].red = qualidade;
				}

				if (aux[i][j].green < 0)
				{
					aux[i][j].green = 0;
				}

				else if (aux[i][j].green > qualidade)
				{
					aux[i][j].green = qualidade;
				}

				if (aux[i][j].blue < 0)
				{
					aux[i][j].blue = 0;
				}

				else if (aux[i][j].blue > qualidade)
				{
					aux[i][j].blue = qualidade;
				}
				//armazena o valor de variacao para ser dispersado
				erro.red = aux[i][j].red;
				erro.blue = aux[i][j].blue;
				erro.green = aux[i][j].green;
				//realiza a binarizacao
				if ((aux[i][j].red+aux[i][j].blue+aux[i][j].green)/3 < valor_otsu)
				{
					aux[i][j].red = 0;
					aux[i][j].blue = 0;
					aux[i][j].green = 0;

				}

				else
				{
					aux[i][j].red = qualidade;
					aux[i][j].blue = qualidade;
					aux[i][j].green = qualidade;

				}
				
				erro.red -= aux[i][j].red;
				erro.blue -= aux[i][j].blue;
				erro.green -= aux[i][j].green;
				//realiza a dispersao do erro de acordo com a matriz
				if (i != linhas-1 && j != colunas-1 && j != 0)
				{
				aux[i+1][j].red += 7 * erro.red/16;
				aux[i+1][j].blue += 7 * erro.blue/16;
				aux[i+1][j].green += 7 * erro.green/16;

				aux[i][j+1].red += 5 * erro.red/16;
				aux[i][j+1].blue += 5 * erro.blue/16;
				aux[i][j+1].green += 5 * erro.green/16;

				aux[i+1][j+1].red += 1 * erro.red/16;
				aux[i+1][j+1].blue += 1 * erro.blue/16;
				aux[i+1][j+1].green += 1 * erro.green/16;

				aux[i+1][j-1].red += 3 * erro.red/16;
				aux[i+1][j-1].blue += 3 * erro.blue/16;
				aux[i+1][j-1].green += 3 * erro.green/16;
				}
			}
	}

	//Verificando se as cores do RGB sao menores que 0, caso sejam igualamos a 0, e verificando tambem se as cores do RGB sao maiores que 255, caso sejam igualamos a 255
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			if (aux[i][j].red < 0)
			{
				aux[i][j].red = 0;
			}

			else if (aux[i][j].red > qualidade)
			{
				aux[i][j].red = qualidade;
			}

			if (aux[i][j].green < 0)
			{
				aux[i][j].green = 0;
			}

			else if (aux[i][j].green > qualidade)
			{
				aux[i][j].green = qualidade;
			}

			if (aux[i][j].blue < 0)
			{
				aux[i][j].blue = 0;
			}

			else if (aux[i][j].blue > qualidade)
			{
				aux[i][j].blue = qualidade;
			}

			//Escrevendo no arquivo
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Desalocando memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);
	free(histograma);

	//Fechando o arquivo
	fclose(arquivo);
}


void sepia(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Cria o arquivo
	FILE* arquivo = fopen("res/sepia.ppm", "w+");

	int i,j;
	
	//Escreve o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Alocando espaco
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
	}

	//Fazendo operacoes, verificando se as cores do RGB sao menores que 0 e maiores que 255, caso ocorra algum caso desse igualamos a 0 ou a 255 dependendo se ele é menor que 0 ou maior que 255 e escrevemos no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			//aplica o filtro multiplicando os valores por coeficientes pre-definidos
			aux[i][j].red = (int) (imagem[i][j].red * 0.393) + (imagem[i][j].green * 0.769) + (imagem[i][j].blue * 0.189);
			aux[i][j].green = (int) (imagem[i][j].red * 0.349) + (imagem[i][j].green * 0.686) + (imagem[i][j].blue * 0.168);
			aux[i][j].blue = (int) (imagem[i][j].red * 0.272) + (imagem[i][j].green * 0.534) + (imagem[i][j].blue * 0.131);
			//verifica se nenhum valor foge dos limites
			if (aux[i][j].red < 0)
			{
				aux[i][j].red = 0;
			}
			else if (aux[i][j].red > qualidade)
			{
				aux[i][j].red = qualidade;
			}
			if (aux[i][j].green < 0)
			{
				aux[i][j].green = 0;
			}
			else if (aux[i][j].green > qualidade)
			{
				aux[i][j].green = qualidade;
			}
			if (aux[i][j].blue < 0)
			{
				aux[i][j].blue = 0;
			}
			else if (aux[i][j].blue > qualidade)
			{
				aux[i][j].blue = qualidade;
			}
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo,"\n");
	}

	//Desalocando memoria
	for (i = 0; i < linhas; ++i)
	{
		free(aux[i]);
	}
	free(aux);

	//Fecahndo arquivo
	fclose(arquivo);
}

//utiliza o procedimento de binarizacao pra identificar fundo e objeto, o objeto eh afiado e o fundo eh borrado
void bokeh(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Cria o arquivo	
	FILE* arquivo = fopen("res/bokeh.ppm", "w+");
	
	//Escreve o cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	int tmp, i, j, valor_otsu = qualidade/2;

	//Alocando memoria
	int* histograma = calloc(qualidade, sizeof(int));
	Cores** aux = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));

	}

	//monta um histograma com as frequencias pra cada media de intensidade
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < colunas; j++)
			{
				tmp = (imagem[i][j].red + imagem[i][j].blue + imagem[i][j].green)/3;
				histograma[tmp]+=1;
			}
	}

	//encontra a menor frequencia e usa como valor para diferenciar
	int menor_i = 255;

	for (i = 0; i < qualidade; i++)
	{
		if (histograma[i]>0 && histograma[i]<histograma[menor_i])
		{
			menor_i = i;
			valor_otsu = histograma[i];
		}
	}

	for (i = 1; i < linhas-1; i++)
	{
		for (j = 1; j < colunas-1; j++)
			{
				if ((imagem[i][j].red+imagem[i][j].blue+imagem[i][j].green)/3 > valor_otsu)
				{
					//aplica o filtro de sharpening nas regioes BRANCAS da binarizada
					aux[i][j].red = (-imagem[i-1][j].red
								 -imagem[i][j-1].red + 5*imagem[i][j].red - imagem[i][j+1].red
								 -imagem[i+1][j].red);

					aux[i][j].blue = (-imagem[i-1][j].blue
								  -imagem[i][j-1].blue + 5*imagem[i][j].blue - imagem[i][j+1].blue
								  -imagem[i+1][j].blue);

					aux[i][j].green = (-imagem[i-1][j].green
								   -imagem[i][j-1].green + 5*imagem[i][j].green - imagem[i][j+1].green
								   -imagem[i+1][j].green);

					if (aux[i][j].red < 0)
					{
						aux[i][j].red = 0;
					}

					else if (aux[i][j].red > qualidade)
					{
						aux[i][j].red = qualidade;
					}

					if (aux[i][j].green < 0)
					{
						aux[i][j].green = 0;
					}

					else if (aux[i][j].green > qualidade)
					{
						aux[i][j].green = qualidade;
					}

					if (aux[i][j].blue < 0)
					{
						aux[i][j].blue = 0;
					}

					else if (aux[i][j].blue > qualidade)
					{
						aux[i][j].blue = qualidade;
					}
				}

				else
				{
					//aplica filtro de blur nas regioes PRETAS da imagem
					aux[i][j].red = (imagem[i-1][j-1].red + imagem[i-1][j].red + imagem[i-1][j+1].red
									+ imagem[i][j-1].red + imagem[i][j].red + imagem[i][j+1].red
									+ imagem[i+1][j-1].red + imagem[i+1][j].red + imagem[i+1][j+1].red)/9.0;

					aux[i][j].blue = (imagem[i-1][j-1].blue + imagem[i-1][j].blue + imagem[i-1][j+1].blue
									+ imagem[i][j-1].blue + imagem[i][j].blue + imagem[i][j+1].blue
									+ imagem[i+1][j-1].blue + imagem[i+1][j].blue + imagem[i+1][j+1].blue)/9.0;

					aux[i][j].green = (imagem[i-1][j-1].green + imagem[i-1][j].green + imagem[i-1][j+1].green
									+ imagem[i][j-1].green + imagem[i][j].green + imagem[i][j+1].green
									+ imagem[i+1][j-1].green + imagem[i+1][j].green + imagem[i+1][j+1].green)/9.0;
				}
			}
	}

	//Escreve no arquivo
	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);

		}
		fprintf(arquivo, "\n");
	}

	//Desaloca memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);
	free(histograma);

	//Fecha o arquivo
	fclose(arquivo);
}

//Mostra quanto tem de cada cor
void analise(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	int nova_qualidade = 1;
	int tmp, i, j, valor_otsu = qualidade/2;
	int* acum = calloc(4, sizeof(float));
	int* histograma = calloc(qualidade, sizeof(int));

	//monta um histograma com as frequencias pra cada media de intensidade
	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < colunas; j++)
			{
				tmp = (imagem[i][j].red + imagem[i][j].blue + imagem[i][j].green)/3;
				histograma[tmp]+=1;
				acum[0] += imagem[i][j].red;
				acum[1] += imagem[i][j].green;
				acum[2] += imagem[i][j].blue;
			}
	}
	//calcula a proporcao de cada canal de cor na coloracao total da imagem
	acum[3] = acum[0] + acum[1] + acum[2];
	acum[0] = (acum[0]/(double)acum[3])*100;
	acum[1] = (acum[1]/(double)acum[3])*100;
	acum[2] = (acum[2]/(double)acum[3])*100;
	int menor_i = 255;

	for (i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			printf("Red level: \n");
		}

		else if(i == 1)
		{
			printf("Green level: \n");
		}

		else
		{
			printf("Blue level: \n");
		}

		printf("[");

		for (j = 0; j < acum[i]; j++)
		{
			printf("|");
		}

		for (j = acum[i]; j < 100; j++)
		{
			printf("-");
		}

		printf("] - %d %% \n", acum[i]);
	}

	//Desaloca memoria
	free(histograma);

}

void desenho(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	//Cria o arquivo
	FILE* arquivo = fopen("res/desenho.ppm", "w+");
	
	int i,j, v;
	double matriz[3][3];
	double mediana = 1.5;
	double soma = 0.0;
	double dp = 1.0;
	//gera a matriz para um filtro gaussiano
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			{
				matriz[i][j] = exp(-0.5*pow((i-mediana)/dp, 2.0) + pow((j-mediana)/dp, 2.0))/(2*M_PI*dp*dp);
				soma += matriz[i][j];
			}
	}

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
			{
				matriz[i][j] = matriz[i][j]/soma;
			}
	}

	//Escrevendo cabecalho
	escreve_cabecalho(arquivo, colunas, linhas, qualidade, tipo_imagem);

	//Interacao com o usuario
	printf("Insira a quantidade de vezes que deseja borrar: ");
	scanf("%d", &v);

	//Alocando memoria
	Cores** aux = calloc(linhas, sizeof(Cores*));
	Cores** cinza = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		aux[i] = calloc(colunas, sizeof(Cores));
		cinza[i] = calloc(colunas, sizeof(Cores));

	}

	for (i = 0; i < linhas; i++)
	{
		for (j = 0; j < (colunas); j++)
		{
			//cria uma imagem cinza
			cinza[i][j] = imagem[i][j];
			cinza[i][j].red = (int) (imagem[i][j].red * 0.3) + (imagem[i][j].green * 0.59) + (imagem[i][j].blue * 0.11);
			if(cinza[i][j].red > 255)
			{
				cinza[i][j].red = 255;
			}
			cinza[i][j].green = cinza[i][j].red;
			cinza[i][j].blue = cinza[i][j].green;
			//transforma a imagem cinza em negativa
			aux[i][j].red = qualidade - cinza[i][j].red;
			aux[i][j].green = qualidade - cinza[i][j].green;
			aux[i][j].blue = qualidade - cinza[i][j].blue;
		}
	}

	while (v>0)
	{
		for (i = 1; i < linhas-1; i++)
		{
			for (j = 1; j < (colunas-1); j++)
			{
				//aplica um blur na imagem negativada
				aux[i][j].red = (matriz[0][0]*aux[i-1][j-1].red + matriz[0][1]*aux[i-1][j].red + matriz[0][2]*aux[i-1][j+1].red
								+ matriz[1][0]*aux[i][j-1].red + matriz[1][1]*aux[i][j].red + matriz[1][2]*aux[i][j+1].red
								+ matriz[2][0]*aux[i+1][j-1].red + matriz[2][1]*aux[i+1][j].red + matriz[2][2]*aux[i+1][j+1].red);
				aux[i][j].blue = (matriz[0][0]*aux[i-1][j-1].blue + matriz[0][1]*aux[i-1][j].blue + matriz[0][2]*aux[i-1][j+1].blue
								+ matriz[1][0]*aux[i][j-1].blue + matriz[1][1]*aux[i][j].blue + matriz[1][2]*aux[i][j+1].blue
								+ matriz[2][0]*aux[i+1][j-1].blue + matriz[2][1]*aux[i+1][j].blue + matriz[2][2]*aux[i+1][j+1].blue);
				aux[i][j].green = (matriz[0][0]*aux[i-1][j-1].green + matriz[0][1]*aux[i-1][j].green + matriz[0][2]*aux[i-1][j+1].green
								+ matriz[1][0]*aux[i][j-1].green + matriz[1][1]*aux[i][j].green + matriz[1][2]*aux[i][j+1].green
								+ matriz[2][0]*aux[i+1][j-1].green + matriz[2][1]*aux[i+1][j].green + matriz[2][2]*aux[i+1][j+1].green);
			}
		}
		v--;
	}

	for (i = 0; i < linhas; ++i)
	{
		for (j = 0; j < colunas; ++j)
		{
			//realiza a operacao de dodge blend para unir as duas imagens
			aux[i][j].red = (aux[i][j].red*qualidade)/(qualidade - cinza[i][j].red);
			aux[i][j].green = (aux[i][j].green*qualidade)/(qualidade - cinza[i][j].green);
			aux[i][j].blue = (aux[i][j].blue*qualidade)/(qualidade - cinza[i][j].blue);

			if (aux[i][j].red < 0)
			{
				aux[i][j].red = 0;
			}

			else if (aux[i][j].red > qualidade)
			{
				aux[i][j].red = qualidade;
			}

			if (aux[i][j].green < 0)
			{
				aux[i][j].green = 0;
			}

			else if (aux[i][j].green > qualidade)
			{
				aux[i][j].green = qualidade;
			}

			if (aux[i][j].blue < 0)
			{
				aux[i][j].blue = 0;
			}

			else if (aux[i][j].blue > qualidade)
			{
				aux[i][j].blue = qualidade;
			}

			fprintf(arquivo, "%d %d %d\n", aux[i][j].red, aux[i][j].green, aux[i][j].blue);
		}

		fprintf(arquivo, "\n");
	}

	//Desalocacao de memoria
	for (i = 0; i < linhas; i++)
	{
		free(aux[i]);
	}
	free(aux);

	//Fechando o arquivo
	fclose(arquivo);	
}

void menu(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	int opcao = 20;
	
	while (opcao != 0)
	{
		printf("\nOla! Seja bem vindo ao editor do maroto, feito por Deyves e Marco.\nEscolha dentre as opcoes listadas aquela que voce preferir:\n");
		printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");	
		printf("║                                                                                ║\n");
		printf("║ 1  - Aplica filtro de escala de cinza na imagem.                               ║\n");
		printf("║                                                                                ║\n");
		printf("║ 2  - Amplia a imagem.                                                          ║\n");
		printf("║                                                                                ║\n");
		printf("║ 3  - Reduz a imagem.                                                           ║\n");
		printf("║                                                                                ║\n");
		printf("║ 4  - Borra a imagem.                                                           ║\n");
		printf("║                                                                                ║\n");
		printf("║ 5  - 'Afia' as bordas da imagem.                                               ║\n");
		printf("║                                                                                ║\n");
		printf("║ 6  - Detecta as bordas da imagem.                                              ║\n");
		printf("║                                                                                ║\n");
		printf("║ 7  - Binariza a imagem.                                                        ║\n");
		printf("║                                                                                ║\n");
		printf("║ 8  - Escolher outro arquivo.                                                   ║\n");
		printf("║                                                                                ║\n");
		printf("║ 9  - Abre o menu para as funcoes de rotacao e espelhamento.                    ║\n");
		printf("║                                                                                ║\n");
		printf("║ 10 - Spoilers de Spider-Man: Far from home.                                    ║\n");
		printf("║                                                                                ║\n");
		printf("║ 0  - Encerra o programa.                                                       ║\n");
		printf("║                                                                                ║\n");
		printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
		printf("Digite sua opção:\n");
		printf("\n");
		scanf("%d", &opcao);
		printf("\n");
		printf("\n");

		switch (opcao)
		{
			case FILTRO_CINZA :
				system("reset");
				filtro_cinza(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case AMPLIAR_IMAGEM :
				system("reset");
				ampliar(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;
			
			case REDUZIR_IMAGEM :
				system("reset");
				reduzir(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case FILTRO_BLURRING :
				system("reset");
				blurring(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;
			
			case FILTRO_SHARPENING :
				system("reset");
				sharpening(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;
			
			case FILTRO_DETECCAO_DE_BORDAS :
				system("reset");
				bordas(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case FILTRO_THRESHOLDING :
				system("reset");
				thresholding(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case APLICAR_EM_OUTRO_ARQUIVO :
				system("reset");
				ler_imagem(imagem, tipo_imagem, &qualidade, &colunas, &linhas);
				break;

			case ROTATION_MENU :
				system("reset");
				menu_rotacao(imagem, colunas, linhas, qualidade, tipo_imagem);
				break;
	
			case EXTRA_MENU :
				menu_extra(imagem, colunas, linhas, qualidade, tipo_imagem);
				break;

			case EXIT :
				printf("\nAte mais, e obrigado pelos peixes!\n");
				system("reset");
				exit(0);
				break;

			default:
				system("reset");
				printf("Opcao invalida, tente novamente\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				menu(imagem, colunas, linhas, qualidade, tipo_imagem);
			 	break;
		}
	}
}

void menu_extra(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	int opcao = 20;

	while (opcao!=0)
	{
		printf("\nEu nao sei o que voce fez para chegar ate aqui, mas...\nBem-vindo ao menu secreto.\n");
		printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");	
		printf("║                                                                                ║\n");
		printf("║ 1  - Abre o menu dos filtros negativos                                         ║\n");
		printf("║                                                                                ║\n");
		printf("║ 2  - Modifica a resolucao de cor da imagem                                     ║\n");
		printf("║                                                                                ║\n");
		printf("║ 3  - Melhoria de contraste.                                                    ║\n");
		printf("║                                                                                ║\n");
		printf("║ 4  - Melhoria de saturacao.                                                    ║\n");
		printf("║                                                                                ║\n");
		printf("║ 5  - Mapa de relevo da imagem.                                                 ║\n");
		printf("║                                                                                ║\n");
		printf("║ 6  - Filtro de Blur gaussiano.                                                 ║\n");
		printf("║                                                                                ║\n");
		printf("║ 7  - Espalhamento de erros para threshold.                                     ║\n");
		printf("║                                                                                ║\n");
		printf("║ 8  - Sepia.                                                                    ║\n");
		printf("║                                                                                ║\n");
		printf("║ 9  - Efeito de retrato (camera dupla ou bokeh).                                ║\n");
		printf("║                                                                                ║\n");
		printf("║ 10 - Analisa a composicao da imagem.                                           ║\n");
		printf("║                                                                                ║\n");
		printf("║ 11 - Transforma a imagem em um 'desenho'.                                      ║\n");
		printf("║                                                                                ║\n");
		printf("║ 0  - Retorna.                                                                  ║\n");
		printf("║                                                                                ║\n");
		printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
		printf("Digite sua opção:\n");
		printf("\n");
		scanf("%d", &opcao);
		printf("\n");
		printf("\n");

		switch (opcao)
		{
			case FILTROS_NEGATIVOS :
				system("reset");
				negative(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case MODIFICA_RESOLUCAO_DA_COR :
				system("reset");
				mod_qualidade(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nMalfeito, feito!\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;
				
			case MELHORA_CONTRASTE :
				system("reset");
				contraste(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nWhen you light a candle, you also cast a shadow.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case MELHORA_SATURACAO :
				system("reset");
				saturacao(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nNem tudo que reluz eh ouro.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;
				
			case RELEVO :
				system("reset");
				emboss(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nEh o terreno que determina quem vence.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case FILTRO_BLURRING_GAUSSIANO :
				system("reset");
				gaussiano(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nA miopia pode ser tratada com oculos ou cirurgicamente.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case ESPELHAMENTO_DE_ERROS :
				system("reset");
				dithering(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nPointy point pointer.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case FILTRO_DE_SEPIA :
				system("reset");
				sepia(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nA palavra sepia deriva de um tipo de polvo do genero Sepia.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case FILTRO_DE_RETRATO :
				system("reset");
				bokeh(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nBokeh eh um termo que representa as imagens com desfoque.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case COMPARA_COMPOSICAO_IMAGEM :
				system("reset");
				analise(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nDataaaaaaaaaaaaaaaaaaaaaa.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case TRANSFORMA_EM_DESENHO :
				system("reset");
				desenho(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\nUm desenho eh uma visao no papel.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				break;

			case VOLTAR_ :
				printf("Com grandes funcoes, vem grandes responsabilidades. Va.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				system("reset");
				menu(imagem, colunas, linhas, qualidade, tipo_imagem);
				break;

			default :
				system("reset");
				printf("Opcao invalida.\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				printf("\n");
				menu_extra(imagem, colunas, linhas, qualidade, tipo_imagem);
				printf("\n");
				printf("\n");
				printf("\n");
				break;
		}
	}
}

void menu_rotacao(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem)
{
	int opcao = 20;

	printf("Escolha o tipo de rotacao ou inversao entre os seguintes: \n");
	printf("╔════════════════════════════════════════════════════════════════════════════════╗\n");	
	printf("║                                                                                ║\n");
	printf("║ 1  - Girar 90 graus.                                                           ║\n");
	printf("║                                                                                ║\n");
	printf("║ 2  - Girar 180 graus.                                                          ║\n");
	printf("║                                                                                ║\n");
	printf("║ 3  - Girar 270 graus.                                                          ║\n");
	printf("║                                                                                ║\n");
	printf("║ 4  - Espelhar horizontalmente. (extra)                                         ║\n");
	printf("║                                                                                ║\n");
	printf("║ 5  - Espelhar verticalmente. (extra)                                           ║\n");
	printf("║                                                                                ║\n");
	printf("║ 0  - Retorna.                                                                  ║\n");
	printf("║                                                                                ║\n");
	printf("╚════════════════════════════════════════════════════════════════════════════════╝\n");
	printf("Digite sua opção:\n");
	printf("\n");
	scanf("%d", &opcao);
	printf("\n");
	printf("\n");
	switch (opcao)
	{
		case GIRAR_90_GRAUS :
			system("reset");
			gira_90(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\nMalfeito, feito!\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;	{
				aux[i][j].red = (matriz[0][0]*aux[i-1][j-1].red + matriz[0][1]*aux[i-1][j].red + matriz[0][2]*aux[i-1][j+1].red
								+ matriz[1][0]*aux[i][j-1].red + matriz[1][1]*aux[i][j].red + matriz[1][2]*aux[i][j+1].red
								+ matriz[2][0]*aux[i+1][j-1].red + matriz[2][1]*aux[i+1][j].red + matriz[2][2]*aux[i+1][j+1].red);
				aux[i][j].blue = (matriz[0][0]*aux[i-1][j-1].blue + matriz[0][1]*aux[i-1][j].blue + matriz[0][2]*aux[i-1][j+1].blue
								+ matriz[1][0]*aux[i][j-1].blue + matriz[1][1]*aux[i][j].blue + matriz[1][2]*aux[i][j+1].blue
								+ matriz[2][0]*aux[i+1][j-1].blue + matriz[2][1]*aux[i+1][j].blue + matriz[2][2]*aux[i+1][j+1].blue);
				aux[i][j].green = (matriz[0][0]*aux[i-1][j-1].green + matriz[0][1]*aux[i-1][j].green + matriz[0][2]*aux[i-1][j+1].green
								+ matriz[1][0]*aux[i][j-1].green + matriz[1][1]*aux[i][j].green + matriz[1][2]*aux[i][j+1].green
								+ matriz[2][0]*aux[i+1][j-1].green + matriz[2][1]*aux[i+1][j].green + matriz[2][2]*aux[i+1][j+1].green);
			}

		case GIRAR_180_GRAUS :
			system("reset");
			gira_180(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\nMalfeito, feito!\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;
			
		case GIRAR_270_GRAUS :
			system("reset");
			gira_270(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\nMalfeito, feito!\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;

		case INVERTER_HORIZONTALMENTE :
			system("reset");
			invert_hor(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\n!oteif, oteiflaM\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;
			
		case INVETER_VERTICAL :
			system("reset");
			invert_ver(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\noʇıǝɟ 'oʇıǝɟlɐɯ\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;

		case VOLTAR :
			printf("Com grandes funcoes, vem grandes responsabilidades. Va.\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			system("reset");
			menu(imagem, colunas, linhas, qualidade, tipo_imagem);
			break;

		default :
			system("reset");
			printf("Opcao invalida.\n");
			printf("\n");
			menu_rotacao(imagem, colunas, linhas, qualidade, tipo_imagem);
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			break;
	}
}
