#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int amostras = 21; // 3 TIPOS DE CADA LETRA, ATÉ O G
    int entradas = 64; // imagens 8x8, vetor = 64
    int numclasses = 7; // 7 LETRAS, A-G
    int targets = 21;
    float entrada[amostras][entradas]; // MATRIZ DE AMOSTRAS
    float target[numclasses][targets]; //MATRIZ TARGET
    float yin[] = {0,0,0,0,0,0,0}; //numclasses - y inicial
    float y[] = {0,0,0,0,0,0,0}; //numclasses - saida liquida
    float erro = 10; // ERRO QUADRATICO TOTAL
    int ciclo = 0;
    float w[entradas][numclasses];
    float wanterior[entradas][numclasses];
    float w0[numclasses];
    float w0anterior[numclasses];
    float xaux[entradas];
    float entrada_teste[entradas];
    float soma;
    float limiar = 0.0;
    float alfa = 0.01;
    float errotolerado = 0.001;
    int letra = 0;

    //float vetor1[100];
    //float vetor2[100];
    int lin, col;
    lin = col = 0;

    for(lin = 0; lin < amostras; lin++){
        for(col = 0; col < entradas; col++){

            entrada[lin][col] = -1.0;
        }
    }

    for(lin = 0; lin < numclasses; lin++){
        for(col = 0; col < targets; col++){

            target[lin][col] = -1.0;
        }
    }

    //LETRAS A
    entrada[0][3] = entrada[0][4] = 1;
    entrada[0][10] = entrada[0][13] = 1;
    entrada[0][17] = entrada[0][22] = 1;
    entrada[0][25] = entrada[0][30] = 1;
    entrada[0][33] = entrada[0][34] = entrada[0][35] = entrada[0][36] = entrada[0][37] = entrada[0][38] = 1;
    entrada[0][41] = entrada[0][46] = 1;
    entrada[0][49] = entrada[0][54] = 1;
    entrada[0][57] = entrada[0][62] = 1;

    entrada[1][3] = entrada[1][4] = entrada[1][5] = 1;
    entrada[1][10] = entrada[1][12] = 1;
    entrada[1][17] = entrada[1][22] = 1;
    entrada[1][25] = entrada[1][30] = 1;
    entrada[1][33] = entrada[1][34] = entrada[1][35] = entrada[1][36] = entrada[1][37] = entrada[1][38] = entrada[1][39]= 1;
    entrada[1][41] = entrada[1][46] = 1;
    entrada[1][49] = entrada[1][54] = 1;
    entrada[1][56] = entrada[1][57] = entrada[1][62] = 1;

    entrada[2][4] = 1;
    entrada[2][10] = entrada[2][13] = 1;
    entrada[2][17] = entrada[2][22] = 1;
    entrada[2][25] = entrada[2][30] = 1;
    entrada[2][34] = entrada[2][35] = entrada[2][36] = entrada[2][37] = entrada[2][38] = 1;
    entrada[2][41] = entrada[2][45] = 1;
    entrada[2][49] = entrada[2][54] = 1;
    entrada[2][57] = entrada[2][63] = 1;

    //LETRAS B
    entrada[3][9] = entrada[3][10] = entrada[3][11] = entrada[3][12] = 1;
    entrada[3][17] = entrada[3][21] = 1;
    entrada[3][25] = entrada[3][29] = 1;
    entrada[3][33] = entrada[3][34] = entrada[3][35] = entrada[3][36] = 1;
    entrada[3][41] = entrada[3][45] = 1;
    entrada[3][49] = entrada[3][53] = 1;
    entrada[3][57] = entrada[3][58] = entrada[3][59] = entrada[3][60] = 1;

    entrada[4][9] = entrada[4][10] = entrada[4][11] = entrada[4][12] = entrada[4][13] = 1;
    entrada[4][17] = entrada[4][20] = 1;
    entrada[4][25] = entrada[4][28] = 1;
    entrada[4][33] = entrada[4][34] = entrada[4][35] = entrada[4][36] = entrada[4][37] = 1;
    entrada[4][41] = entrada[4][45] = 1;
    entrada[4][49] = entrada[4][53] = 1;
    entrada[4][57] = entrada[4][58] = entrada[4][59] = entrada[4][60] = 1;

    entrada[4][8] = entrada[5][9] = entrada[5][10] = entrada[5][11] = entrada[5][12] = 1;
    entrada[5][17] = entrada[5][21] = 1;
    entrada[5][25] = entrada[5][29] = 1;
    entrada[4][32] = entrada[5][33] = entrada[5][34] = entrada[5][35] = entrada[5][36] = 1;
    entrada[5][41] = entrada[5][45] = 1;
    entrada[5][49] = entrada[5][53] = 1;
    entrada[4][58] = entrada[5][57] = entrada[5][58] = entrada[5][59] = entrada[5][60] = 1;

    //LETRAS C
    entrada[6][9] = entrada[6][10] = entrada[6][11] = entrada[6][12] = entrada[6][13] = entrada[6][14] = 1;
    entrada[6][17] = 1;
    entrada[6][25] = 1;
    entrada[6][33] = 1;
    entrada[6][41] = 1;
    entrada[6][49] = 1;
    entrada[6][57] = entrada[6][58] = entrada[6][59] = entrada[6][60] = entrada[6][61] = entrada[6][62] = 1;

    entrada[7][9] = entrada[7][10] = entrada[7][11] = entrada[7][12] = entrada[7][13] = 1;
    entrada[7][17] = 1;
    entrada[7][24] = 1;
    entrada[7][32] = 1;
    entrada[7][40] = 1;
    entrada[7][49] = 1;
    entrada[7][57] = entrada[7][58] = entrada[7][59] = entrada[7][60] = entrada[7][61] = 1;

    entrada[8][9] = entrada[8][10] = entrada[8][11] = entrada[8][12] = entrada[8][13] = entrada[8][14] = 1;
    entrada[8][25] = 1;
    entrada[8][33] = 1;
    entrada[8][41] = 1;
    entrada[8][57] = entrada[8][58] = entrada[8][59] = entrada[8][60] = entrada[8][61] = entrada[8][62] = 1;

    //LETRAS D
    entrada[9][9] = entrada[9][10] = entrada[9][11] = entrada[9][12] = 1;
    entrada[9][17] = entrada[9][21] = 1;
    entrada[9][25] = entrada[9][29] = 1;
    entrada[9][33] = entrada[9][37] = 1;
    entrada[9][41] = entrada[9][45] = 1;
    entrada[9][49] = entrada[9][53] = 1;
    entrada[9][56] = entrada[9][57] = entrada[9][58] = entrada[9][59] = 1;

    entrada[10][9] = entrada[10][10] = entrada[10][11] = entrada[10][12] = 1;
    entrada[10][17] = entrada[10][21] = 1;
    entrada[10][25] = entrada[10][29] = 1;
    entrada[10][33] = entrada[10][37] = 1;
    entrada[10][41] = entrada[10][45] = 1;
    entrada[10][49] = entrada[10][53] = 1;
    entrada[10][56] = entrada[10][57] = entrada[10][58] = entrada[10][59] = 1;

    entrada[11][9] = entrada[11][10] = entrada[11][11] = entrada[11][12] = 1;
    entrada[11][17] = entrada[11][21] = 1;
    entrada[11][25] = entrada[11][29] = 1;
    entrada[11][33] = entrada[11][37] = 1;
    entrada[11][41] = entrada[11][45] = 1;
    entrada[11][49] = entrada[11][53] = 1;
    entrada[11][56] = entrada[11][57] = entrada[11][58] = entrada[11][59] = 1;

    //LETRAS E
    entrada[12][9] = entrada[12][10] = entrada[12][11] = entrada[12][12] = entrada[12][13] = 1;
    entrada[12][17] = 1;
    entrada[12][25] = 1;
    entrada[12][33] = entrada[12][34] = entrada[12][35] = entrada[12][36] = 1;
    entrada[12][41] = 1;
    entrada[12][49] = 1;
    entrada[12][57] = entrada[12][58] = entrada[12][59] = entrada[12][60] = entrada[12][61] = 1;

    entrada[13][9] = entrada[13][10] = entrada[13][11] = entrada[13][12] = entrada[13][13] = 1;
    entrada[13][17] = 1;
    entrada[13][25] = 1;
    entrada[13][33] = entrada[13][34] = entrada[13][35] = entrada[13][36] = 1;
    entrada[13][41] = 1;
    entrada[13][49] = 1;
    entrada[13][57] = entrada[13][58] = entrada[13][59] = entrada[13][60] = entrada[13][61] = 1;

    entrada[14][9] = entrada[14][10] = entrada[14][11] = entrada[14][12] = entrada[14][13] = 1;
    entrada[14][17] = 1;
    entrada[14][25] = 1;
    entrada[14][33] = entrada[14][34] = entrada[14][35] = entrada[14][36] = 1;
    entrada[14][41] = 1;
    entrada[14][49] = 1;
    entrada[14][57] = entrada[14][58] = entrada[14][59] = entrada[14][60] = entrada[14][61] = 1;

    //LETRAS F
    entrada[15][9] = entrada[15][10] = entrada[15][11] = entrada[15][12] = entrada[15][13] = 1;
    entrada[15][17] = 1;
    entrada[15][25] = 1;
    entrada[15][33] = entrada[15][34] = entrada[15][35] = entrada[15][36] = 1;
    entrada[15][41] = 1;
    entrada[15][49] = 1;
    entrada[15][57] = 1;

    entrada[16][9] = entrada[16][10] = entrada[16][11] = entrada[16][12] = entrada[16][13] = 1;
    entrada[16][17] = 1;
    entrada[16][25] = 1;
    entrada[16][33] = entrada[16][34] = entrada[16][35] = entrada[16][36] = 1;
    entrada[16][41] = 1;
    entrada[16][49] = 1;
    entrada[16][57] = 1;

    entrada[17][9] = entrada[17][10] = entrada[17][11] = entrada[17][12] = entrada[17][13] = 1;
    entrada[17][17] = 1;
    entrada[17][25] = 1;
    entrada[17][33] = entrada[17][34] = entrada[17][35] = entrada[17][36] = 1;
    entrada[17][41] = 1;
    entrada[17][49] = 1;
    entrada[17][57] = 1;

    //LETRAS G
    entrada[18][10] = entrada[18][11] = entrada[18][12] = entrada[18][13] = 1;
    entrada[18][17] = 1;
    entrada[18][25] = 1;
    entrada[18][33] = 1;
    entrada[18][41] = entrada[18][46] = entrada[18][45] = entrada[18][46] = 1;
    entrada[18][49] = entrada[18][54] = 1;
    entrada[18][58] = entrada[18][59] = entrada[18][60] = entrada[18][61] = entrada[18][62] = 1;

    entrada[19][10] = entrada[19][11] = entrada[19][12] = entrada[19][13] = 1;
    entrada[19][17] = 1;
    entrada[19][25] = 1;
    entrada[19][33] = 1;
    entrada[19][41] = entrada[19][46] = entrada[19][45] = entrada[19][46] = 1;
    entrada[19][49] = entrada[19][54] = 1;
    entrada[19][58] = entrada[19][59] = entrada[19][60] = entrada[19][61] = entrada[19][62] = 1;

    entrada[20][10] = entrada[20][11] = entrada[20][12] = entrada[20][13] = 1;
    entrada[20][17] = 1;
    entrada[20][25] = 1;
    entrada[20][33] = 1;
    entrada[20][41] = entrada[20][46] = entrada[20][45] = entrada[20][46] = 1;
    entrada[20][49] = entrada[20][54] = 1;
    entrada[20][58] = entrada[20][59] = entrada[20][60] = entrada[20][61] = entrada[20][62] = 1;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////
    // MATRIZ TARGET
    target[0][0] = target[0][1] = target[0][2] = 1;
    target[1][3] = target[1][4] = target[1][5] = 1;
    target[2][6] = target[2][7] = target[2][8] = 1;
    target[3][9] = target[3][10] = target[3][11] = 1;
    target[4][12] = target[4][13] = target[4][14] = 1;
    target[5][15] = target[5][16] = target[5][17] = 1;
    target[6][18] = target[6][19] = target[6][20] = 1;

    for(lin = 0; lin < entradas;lin++){
        for(col = 0; col < numclasses; col++){

            srand (time(NULL));
            w[lin][col] = (rand() % 100);
            w[lin][col] = w[lin][col]/100;
        }
    }

    /*----------------------------------------*/
    while (erro > errotolerado){
        ciclo++;
        erro = 0;
        for(int i = 0; i < amostras;i++){
            for(col = 0; col < entradas; col++){

                xaux[col] = entrada[i][col];
            }

            for(int m = 0; m < numclasses; m++){

                soma = 0;
                for(int n = 0; n < entradas; n++){

                    soma = soma + xaux[n] * w[n][m];
                }

                yin[m] = soma + w0[m];
            }

            for(int m = 0; m < numclasses; m++){

                if (yin[m] >= limiar){

                    y[m] = 1.0;
                }
                else{

                    y[m] = -1.0;
                }
            }

            for(int m = 0; m < numclasses; m++){

                erro = erro + 0.5 * pow((target[m][i] - y[m]), 2);
            }

            ///////////////////////////////////
            for(int lin2 = 0; lin2 < entradas;lin2++){
                for(int col2 = 0; col2 < numclasses; col2++){

                    wanterior[lin2][col2] = w[lin2][col2];
                }
            }
            ///////////////////////////////

            for(int m = 0; m < entradas; m++){
                for(int n = 0; n < numclasses; n++){

                    w[m][n] = wanterior[m][n] + alfa * (target[n][i] - y[n]) * xaux[m];
                }
            }

            ///////////////////////////////////
            for(int lin2 = 0; lin2 < numclasses;lin2++){

                w0anterior[lin2] = w0[lin2];
            }
            ///////////////////////////////

            for (int m = 0; m < numclasses; m++){
                w0[m] = w0anterior[m] + alfa * (target[m][i] - y[m]);
            }
        }

        //vetor1[ciclo] = ciclo;
        //vetor2[ciclo] = erro;

        printf("Ciclo %d -> Erro: %.5f\n", ciclo, erro);
    }

    printf("\n====== TESTANDO A REDE ========\n");

    //TESTANDO A REDE
    //alterar os valores da variavel letra para testar
    //0, 1, 2 - letra A
    //3, 4, 5 - letra B
    //6, 7, 8 - letra C
    //9, 10, 11 - letra D
    //12, 13, 14 - letra E
    //15, 16, 17 - letra F
    //18, 19, 20 - letra G
    letra = 7;

    for(col = 0; col < entradas; col++){

        entrada_teste[col] = entrada[letra][col];
    }

    for(int m = 0; m < numclasses; m++){

        soma = 0;
        for(int n = 0; n < entradas; n++){

            soma = soma + entrada_teste[n] * w[n][m];
            yin[m] = soma + w0[m];
        }
    }

    for(int m = 0; m < numclasses; m++){
        if(yin[m] >= limiar){

            y[m] = 1.0;
        }
        else{

            y[m] = -1.0;
        }

        printf("[%.1f]", y[m]);
    }

    printf("\n\n");

    if (y[0] == 1.0) { printf("Letra A reconhecida\n"); }
    if (y[1] == 1.0) { printf("Letra B reconhecida\n"); }
    if (y[2] == 1.0) { printf("Letra C reconhecida\n"); }
    if (y[3] == 1.0) { printf("Letra D reconhecida\n"); }
    if (y[4] == 1.0) { printf("Letra E reconhecida\n"); }
    if (y[5] == 1.0) { printf("Letra F reconhecida\n"); }
    if (y[6] == 1.0) { printf("Letra G reconhecida\n"); }
    printf("-=-=-=-=-=-=-=-=-=-=-=-\n");

}
