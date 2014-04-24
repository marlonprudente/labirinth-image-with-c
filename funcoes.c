/*============================================================================*/
/* MÓDULO QUE BUSCA O CAMINHO MAIS CURTO PARA SAIR DE UM LABIRINTO            */
/*----------------------------------------------------------------------------*/
/* Autor: Bogdan T. Nassu - nassu@dainf.ct.utfpr.edu.br                       */
/*============================================================================*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "trabalho3.h"

int encontraCaminho (Imagem1C* img, Coordenada** caminho){

    int **m, tam;

    Imagem1C copia_img;
    copia_img = *img;
    normaliza_imagem(img, &copia_img);
    filtroMenor(img, &copia_img);
    m = alocaMatriz(img->altura + 2,img->largura + 2);
    inicializaLabirinto(m , &copia_img ,img->altura + 2, img->largura + 2);
    criaMatrizCusto(&copia_img, m, img->altura + 2, img->largura + 2);
    tam = calculaCaminho(&copia_img, m, caminho, img->altura + 2, img->largura + 2);
    printf("Salvando Imagem! \n");
    salvaImagem1C (img, "Imagem_apos_processo.bmp");
    return tam;
}
int ** alocaMatriz(int altura, int largura){                    //Alocação Dinâmica
    int **mat, i;

    mat = (int**) malloc(altura*sizeof(int*));

    for (i = 0; i < altura; i++)
        mat[i] = (int*) malloc(largura*sizeof(int));

    return mat;
}
void normaliza_imagem(Imagem1C* img, Imagem1C* out){

   int maior, menor, i, j, limiar;
   maior = img->dados[0][0];   //Iniciando os Valores Maior e Menor com o primeiro Pixel da Imagem
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

    limiar=((maior+menor)/2);

    for(i = 0; i < img->altura; i++){
        for(j = 0; j < img->largura; j++){
            if(out->dados[i][j]<=limiar){
                out->dados[i][j] = 0;
            }
            else
                out->dados[i][j] = 255;
        }
    }
}
void filtroMenor(Imagem1C* img, Imagem1C* out){

        int i, j;
        int k=0;

        while (k<6){
            for (i=1; i< img->altura-1; i++){
                for (j=1; j< img->largura-1; j++){
                    if(img->dados[i][j]==0){
                        if(i > 0 || j > 0 || i < img->altura || j < img->largura){
                            out->dados[i][j-1]=0;
                            out->dados[i-1][j]=0;
                        }
                    } //  Uso p J menos um para deixar a coluna ao lado od risco preto, uma vez já passou por esse campo
                }
            }

          /*for (i=img->altura-1; i< 1; i++){
                for (j=img->largura-1; j< 1; j++){
                    if(img->dados[i][j]==0){
                        if(i > 0 || j > 0 || i < img->altura || j < img->largura){
                            out->dados[i][j-1]=0;
                            out->dados[i-1][j]=0;
                        }
                    } //  Uso p J menos um para deixar a coluna ao lado od risco preto, uma vez já passou por esse campo
                }
            }*/


            k++;
        }

}
void inicializaLabirinto(int **m ,Imagem1C* img,int altura,int largura){

    int i, j;

    for(i = 0; i < altura; i++){  //Criar bordas
        for (j = 0; j < largura; j++){
            if (i == 0 || j == 0 || i == altura - 1 || j == largura - 1)
                m[i][j] = -1;
        }
    }

    for(i=1; i<altura - 1; i++){
        for(j=1; j<largura - 1; j++){
            if(img->dados[i - 1][j - 1] == 0){
                m[i][j]=-2;
            }
            else
            m[i][j]=-1;
        }
    }
}
void criaMatrizCusto(Imagem1C* img, int **m, int altura, int largura){         //Calcula a que distancia do queijo estį a posiēćo atual

    int i, j,cont = 1, num = 0, op = 0;
    Coordenada inicio;

    for(j=0; j<largura; j++){
        for(i=0; i<altura; i++){
            if(m[i][j]==-2){
                inicio.x=i;
                inicio.y=j;
                op = 1;
                break;
            }
        }
        if(op==1){
        break;
        }
    }

    m[inicio.x][inicio.y]=0;

    while(cont!=0){
        cont=0;

        for(i=0; i<altura; i++){
            for(j=0; j<largura; j++){
                if(m[i][j]==num){
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

int calculaCaminho (Imagem1C* img, int **m, Coordenada **posicoes, int altura, int largura){
    int i, j, k, interacoes, op = 0, aux, indi, indj;

/* Percorrendo a Matriz da Direita, para a Esquerda, de Cima para Baixo*/
    for(j=largura-1; j>=0; j--){
        for(i=0; i<altura; i++){
            if(m[i][j]>0){
                interacoes=m[i][j];
                indi = i;
                indj = j;
                op = 1;
                break;
            }
        }
        if(op==1){
            break;
        }
    }
    for(i=0; i<altura; i++){
        if(m[i][j] < interacoes && m[i][j] > 0){
            interacoes = m[i][j];
            indi = i;
            indj = j;
        }
    }


    *posicoes = (Coordenada *)malloc((interacoes+2)*sizeof(Coordenada));
    (*posicoes)[0].y = indi;
    (*posicoes)[0].x = indj;

    aux = interacoes - 1;
    i = indi;
    j = indj;
    k = 1;
    while(k!=interacoes + 1){
        if(m[i+1][j] == aux){
            (*posicoes)[k].y = i+1;
            (*posicoes)[k].x = j;
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

