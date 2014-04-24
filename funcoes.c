/*============================================================================*/
/* MÓDULO QUE BUSCA O CAMINHO MAIS CURTO PARA SAIR DE UM LABIRINTO            */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "trabalho3.h"
/*============================================================================*/
int encontraCaminho (Imagem1C* img, Coordenada** caminho){

    int **m, tam;
    Imagem1C copia_img;
    copia_img = *img;

    normaliza_imagem(img, &copia_img);
    filtroMenor(img, &copia_img);
    m = alocaMatriz(img->altura + 2,img->largura + 2); ///incluo o valor de dois na altura e largura para as bordas
    inicializaLabirinto(m , &copia_img ,img->altura + 2, img->largura + 2);
    criaMatrizCusto(&copia_img, m, img->altura + 2, img->largura + 2);
    tam = calculaCaminho(&copia_img, m, caminho, img->altura + 2, img->largura + 2);

    return tam;
}
/*==========================================================================================================================*/

/** Função responsável por fazer a alocação da matriz de custo **/

int ** alocaMatriz(int altura, int largura){

    int **mat, i;

    mat = (int**) malloc(altura*sizeof(int*));

    for (i = 0; i < altura; i++)
        mat[i] = (int*) malloc(largura*sizeof(int));

    return mat;
}

/*==========================================================================================================================*/

/** Deixa a imagem com apenas duas cores (preto e branco)**/

void normaliza_imagem(Imagem1C* img, Imagem1C* out){

   int maior, menor, i, j, limiar;
   maior = img->dados[0][0];   ///Iniciando os Valores Maior e Menor com o primeiro Pixel da Imagem
   menor = img->dados[0][0];
   /* Irei Percorrer a Imagem, para encontrar os valores de branco e os valores de preto que está na imagem,
        isso é necessário pois não se sabe se o preto na Imagem é realmente preto ou uma escala de cinza fraco.
   */
   for(i=0;i<img->altura;i++){
       for(j=0;j<img->largura;j++){
           if(img->dados[i][j]>maior)
               maior = img->dados[i][j];
           if(img->dados[i][j]<menor)
               menor = img->dados[i][j];
       }
   }
    ///após econtrar o menor e o maior valor da imagens, uso a media desses numeros para ser o limiar da binarização
    limiar=((maior+menor)/2);

    for(i = 0; i < img->altura; i++){       ///Com o limiar definido, percorro a matriz transformando
        for(j = 0; j < img->largura; j++){  ///tudo que for maior ou igual ao limar em preto
            if(out->dados[i][j]<=limiar+10){   /// eh o q for menor que o limar em brando.
                out->dados[i][j] = 0;
            }
            else
                out->dados[i][j] = 255;
        }
    }
}

/*==========================================================================================================================*/

/** Elimina as falhas no desenho da imagem, engrossando a linha **/

void filtroMenor(Imagem1C* img, Imagem1C* out){

        int i, j;
        int k=0;

        while (k<6){
            for (i=1; i< img->altura-1; i++){       ///percorro a imagem, desconsiderando as bordas,
                for (j=1; j< img->largura-1; j++){  ///até localizar um numero que faça parte do caminho
                    if(img->dados[i][j]==0){        /// então altero um pixel acima e outro
                        out->dados[i][j-1]=0;       /// ao lado direito (onde já foi percorrido), por preto.
                        out->dados[i-1][j]=0;
                    }
                }
            }
            k++; ///Repito o processo seis vezes para reduzir ao maximo a possibilidade de falhas no caminho.
        }

}

/*==========================================================================================================================*/

/** Cria uma matriz com o tamanho de pixels que tem a imagens
 *
 * e preenche ela incialmente com numeros diferentes para o fundo e o caminho (desenho da imagem)  */

void inicializaLabirinto(int **m ,Imagem1C* img,int altura,int largura){

    int i, j;

    for(i = 0; i < altura; i++)  ///Criar bordas
        for (j = 0; j < largura; j++)
            if (i == 0 || j == 0 || i == altura - 1 || j == largura - 1)
                m[i][j] = -1;

    for(i=1; i<altura - 1; i++){
        for(j=1; j<largura - 1; j++){           ///percorro a imagem transfomando
            if(img->dados[i - 1][j - 1] == 0){ ///o que for igual a 0 e -2 e o resto em -1
                m[i][j]=-2;
            }
            else
            m[i][j]=-1;
        }
    }
}

/*==========================================================================================================================*/

/** Preenche o caminho da matriz criada anteriormente, com numeros sequencias,
 *
 *   iniciando do zero em um ponta e termina quando o caminho acaba  */

void criaMatrizCusto(Imagem1C* img, int **m, int altura, int largura){

    int i, j,cont = 1, num = 0, op = 0;
    Coordenada inicio;

    for(j=0; j<largura; j++){    /// Percorro a matriz até encontrar o primerio valor que pertença ao caminho.
        for(i=0; i<altura; i++){ /// Salvo esse valor e a localização dele.
            if(m[i][j]==-2){
                inicio.x=i;
                inicio.y=j;
                op = 1;
                break;
            }
        }
        if(op==1){ /// Força a sair do segundo for assim que encontrou o primeiro valor que pertença ao caminho
        break;
        }
    }

    m[inicio.x][inicio.y]=0; ///A matriz possição encontrada recebe 0(para iniciar o caminho)

    while(cont!=0){ ///Caso não hajá mais caminho para ser incrementado, sair do laço.
        cont=0;

        for(i=0; i<altura; i++){        ///Percorro a matriz, até localizar um numero igual a possição do caminho onde estamos,
            for(j=0; j<largura; j++){   ///quando localizado, a proxima possição, que pertença ao caminho,
                if(m[i][j]==num){       /// recebe o proximo numero natural.
                    if( m[i][j+1]==-2)
                        m[i][j+1]= num + 1;
                    if(m[i][j-1]==-2)
                        m[i][j-1]= num + 1;
                    if(m[i+1][j]==-2)
                        m[i+1][j]= num + 1;
                    if(m[i-1][j]==-2)
                        m[i-1][j]= num + 1;
                        cont = 1;
                }
            }
        }
        num++;
    }

}

/*==========================================================================================================================*/

/** Função responsael por encontrar o menos caminho possivel para ser percorrer em cada imagem  **/

int calculaCaminho (Imagem1C* img, int **m, Coordenada **posicoes, int altura, int largura){
    int i, j, k, interacoes, op = 0, aux, indi, indj;


    for(j=largura-1; j>=0; j--){    /// Percorro a matriz, no sentido contrario (da diretira para a esquerda e de cima para baixo)
        for(i=0; i<altura; i++){    /// até encontrar o primeior valor que pertença ao caminho.
            if(m[i][j]>0){          /// Salvo esse valor e a localização dele.
                interacoes=m[i][j];
                indi = i;
                indj = j;
                op = 1;
                break;
            }
        }
        if(op==1){ /// A op=1 força a sair do laço, caso o if seja satisfeito.
            break;
        }
    }
    for(i=0; i<altura; i++){   ///procuro na coluna onde foi localizado o fim do caminho.
        if(m[i][j] < interacoes && m[i][j] > 0){ /// para ver se há algum numero menor ao encontrado anteriormete
            interacoes = m[i][j];               ///Caso haja, salvo o valor dele é possição.
            indi = i;
            indj = j;
        }
    }

    /// Alocando o vetor de coordenadas
    *posicoes = (Coordenada *)malloc((interacoes+2)*sizeof(Coordenada));
    (*posicoes)[0].y = indi;
    (*posicoes)[0].x = indj;

    aux = interacoes - 1; ///O auxiliar recebe o numero de interações menos um, pois a possição do ultimo pixel já foi encontrado.
    i = indi;
    j = indj;
    k = 1;
    while(k!=interacoes + 1){
        if(m[i+1][j] == aux){       ///Percorro toda a matriz de custo procurando um numero que seja igual
            (*posicoes)[k].y = i+1; ///a possição do caminho em que estamos, caso encontre, salvo as possições
            (*posicoes)[k].x = j;   /// e mudo o lugar da busca.
            i++;
            aux--;

        }
        else if(m[i-1][j] == aux){
            (*posicoes)[k].y = i-1;
            (*posicoes)[k].x = j;
            i--;
            aux--;
        }
        else if(m[i][j+1] == aux){
            (*posicoes)[k].y = i;
            (*posicoes)[k].x = j+1;
            j++;
            aux--;

        }
        else if(m[i][j-1] == aux){
            (*posicoes)[k].y = i;
            (*posicoes)[k].x = j-1;
            j--;
            aux--;
        }
        k++;
    }
    (*posicoes)[interacoes+1].x = 0;
    (*posicoes)[interacoes+1].y = i;

    return interacoes + 2;
}

/*================================================================================================*/
