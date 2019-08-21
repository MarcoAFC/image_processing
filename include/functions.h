#ifndef _FUNCTIONS_
#define _FUNCTIONS_
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


//struct para definir o que vai ter me cada pixel
typedef struct
{
	int red;
	int green;
	int blue;
}Cores;

typedef struct 
{
	double h;
	double s;
	double v;
}HSV;

//Enum para o menu principal
enum home_menu
{
	EXIT,
	FILTRO_CINZA,
	AMPLIAR_IMAGEM,
	REDUZIR_IMAGEM,
	FILTRO_BLURRING,
	FILTRO_SHARPENING,
	FILTRO_DETECCAO_DE_BORDAS,
	FILTRO_THRESHOLDING,
	APLICAR_EM_OUTRO_ARQUIVO,
	ROTATION_MENU,
	EXTRA_MENU,
};

//Menu de rotcao
enum rotation_menu
{
	VOLTAR,
	GIRAR_90_GRAUS,
	GIRAR_180_GRAUS,
	GIRAR_270_GRAUS,
	INVERTER_HORIZONTALMENTE,
	INVETER_VERTICAL,
};

//Menu com outras funcionalidades
enum extra_features
{
	VOLTAR_,
	FILTROS_NEGATIVOS,
	MODIFICA_RESOLUCAO_DA_COR,
	MELHORA_CONTRASTE,
	MELHORA_SATURACAO,
	RELEVO,
	FILTRO_BLURRING_GAUSSIANO,
	ESPELHAMENTO_DE_ERROS,
	FILTRO_DE_SEPIA,
	FILTRO_DE_RETRATO,
	COMPARA_COMPOSICAO_IMAGEM,
	TRANSFORMA_EM_DESENHO,
};

//Menu para filtros negativos
enum negative_menu
{
	TRADICIONAL,
	APLICADO_AO_VERMELHO,
	APLICADO_AO_VERDE,
	APLICADO_AO_AZUL,
	APLICADO_AO_VERMELHO_VERDE,
	APLICADO_AO_VERMELHO_AZUL,
	APLICADO_AO_VERDE_AZUL,
};

//escreve o cabecalho no arquivo
void escreve_cabecalho(FILE* arquivo, int colunas, int linhas, int qualidade, char* tipo_imagem);

//função para ler a imagem que vai ser dada
void ler_imagem(Cores **imagem, char *tipo, int *qualidade, int *colunas, int *linhas);

//transforma em escala de cinza
void filtro_cinza(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//amplia imagem de acordo com o fator fornecido
void ampliar(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//gira 90º
void gira_90 (Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//gira 180º
void gira_180 (Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//gira 270º
void gira_270 (Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//gera a imagem negativa
void negative(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Espelha a imagem
void invert_hor(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Espelha a imagem verticalmente
void invert_ver(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//borrar a imagem
void blurring(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Binariza a imagem
void thresholding(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//afia as bordas da imagem
void sharpening(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//modifica a qualidade da imagem, aumentando ou diminuindo a resolucao de cor
void mod_qualidade(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

// reduz a imagem de acordo com o fator
void reduzir(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//aplica as matrizes de deteccao de bordas
void bordas(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//aplica melhoramento linear de contraste de acordo com os percentis 5 e 95
void contraste(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//aplica melhoramento de contraste por meio de conversao HSV
void saturacao(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//cria um mapa de relevo da imagem
void emboss(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Aplica o filtro de blur gaussiano
void gaussiano(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//aplica filtro de coloracao sepia na imagem
void sepia(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//gera um efeito de destaque nos objetos principais e desfoque no fundo. OBS: similar ao efeito de retrato na camera dupla
void bokeh(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Gera uma analise da composicao da imagem, coloracao e distribuicao
void analise(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Cria um efeito de desenho na imagem
void desenho(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//faz dispersao de erro para representar melhor a imagem binarizada
void dithering(Cores** imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Gera o menu principal
void menu(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Gera o menu de extras
void menu_extra(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

//Gera o menu das funcoes de rotacao e inversao
void menu_rotacao(Cores **imagem, int colunas, int linhas, int qualidade, char* tipo_imagem);

#endif