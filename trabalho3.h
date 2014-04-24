/*============================================================================*/
/* MÓDULO QUE BUSCA O CAMINHO MAIS CURTO PARA SAIR DE UM LABIRINTO            */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
#ifndef __TRABALHO3_H
#define __TRABALHO3_H

#include "imagem.h"

/*============================================================================*/
/* Um tipo simples, usado para representar uma coordenada. */

typedef struct
{
    int x;
    int y;
} Coordenada;

/*============================================================================*/
/* Função central do trabalho. */

int encontraCaminho (Imagem1C* img, Coordenada** caminho);

/*============================================================================*/
/* Funções Criadas para Otimizar as etapas de tratamento da imagem, e criação do caminho*/
void normaliza_imagem(Imagem1C* img, Imagem1C* out);
void filtroMenor(Imagem1C* img, Imagem1C* out);
void criaMatrizCusto(Imagem1C* img, int **m, int altura, int largura);
int ** alocaMatriz(int largura, int altura);
void inicializaLabirinto(int **m,Imagem1C* img,int altura,int largura);
int calculaCaminho (Imagem1C* img, int **m, Coordenada **posicoes, int altura, int largura);
/*============================================================================*/
#endif /* __TRABALHO3_H */
