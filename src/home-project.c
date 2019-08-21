#include "functions.h"

//função principal
int main()
{
	
	//variavel para receber o tipo, colunas, linhas e o nivel da cor, e alocação dinamica para tipo_imagem para ler como string;
	char *tipo_imagem = calloc(10, sizeof(char));
	int colunas = 1280, linhas = 720, qualidade, i;

	//Alocação dinamica para a matriz
	Cores **imagem = calloc(linhas, sizeof(Cores*));
	for(i = 0; i < linhas; i++)
	{
		imagem[i] = calloc(colunas, sizeof(Cores));
	}

	//Chamada da funcao para leitura da imagem ppm
	ler_imagem(imagem, tipo_imagem, &qualidade, &colunas, &linhas);

	//Menu para iniciar realmente o programa, mostrando as opcoes a serem escolhidas
	menu(imagem, colunas, linhas, qualidade, tipo_imagem);

	//Desalocando a memoria para liber espaco que estava sendo utilizado para matriz
	for (i = 0; i < linhas; ++i)
	{
		free(imagem[i]);
	}
	free(imagem);

	//Desalocação do ponteiro de char que guardava o tipo P3 da imagem que haviamos lido como string
	free(tipo_imagem);

	return 0;
}
