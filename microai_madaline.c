#include "microai_madaline.h"


float entrada[20][70]; 	// MATRIZ DE AMOSTRAS
float target[10][30]; 	// MATRIZ TARGET
float yin[] = {0,0,0,0,0,0,0}; 			// numclasses - y inicial
float y[] = {0,0,0,0,0,0,0}; 				// numclasses - saida liquida
float erro = 10; 										// ERRO QUADRATICO TOTAL
int ciclo = 0;
//float w[70][10];
float wanterior[70][10];
float w0[10];
float w0anterior[10];
float xaux[70];
float soma;
float limiar = 0.0;
float alfa = 0.01;
float errotolerado = 0.001;
int letra = 0;


void treinarRede(int amostras, int entradas, int numclasses, int targets, 
									int entrada[amostras][entradas], int target[numclasses][targets], float w[entradas][numclasses])
{
	
		for(int lin = 0; lin < entradas;lin++)
		{
				for(int col = 0; col < numclasses; col++)
				{

						srand(rand8());
						w[lin][col] = (rand() % 100);
						w[lin][col] = w[lin][col]/100;
				}
		}

    while (erro > errotolerado)
		{
        ciclo++;
        erro = 0;
        for(int i = 0; i < amostras;i++)
				{
            for(int col = 0; col < entradas; col++)
						{

                xaux[col] = entrada[i][col];
            }

            for(int m = 0; m < numclasses; m++)
						{

                soma = 0;
                for(int n = 0; n < entradas; n++)
								{

                    soma = soma + xaux[n] * w[n][m];
                }

                yin[m] = soma + w0[m];
            }

            for(int m = 0; m < numclasses; m++)
						{

                if (yin[m] >= limiar)
								{

                    y[m] = 1.0;
                }
                else
								{

                    y[m] = -1.0;
                }
            }

            for(int m = 0; m < numclasses; m++)
						{

                erro = erro + 0.5 * pow((target[m][i] - y[m]), 2);
            }

            ///////////////////////////////////
            for(int lin2 = 0; lin2 < entradas;lin2++)
						{
                for(int col2 = 0; col2 < numclasses; col2++)
								{

                    wanterior[lin2][col2] = w[lin2][col2];
                }
            }
            ///////////////////////////////

            for(int m = 0; m < entradas; m++)
						{
                for(int n = 0; n < numclasses; n++)
								{

                    w[m][n] = wanterior[m][n] + alfa * (target[n][i] - y[n]) * xaux[m];
                }
            }

            ///////////////////////////////////
            for(int lin2 = 0; lin2 < numclasses;lin2++)
						{

                w0anterior[lin2] = w0[lin2];
            }
            ///////////////////////////////

            for (int m = 0; m < numclasses; m++)
						{
                w0[m] = w0anterior[m] + alfa * (target[m][i] - y[m]);
            }
        }

        //printf("Ciclo %d -> Erro: %.5f\n", ciclo, erro);
    }

}

void testarRede(int entradas, int numclasses, float w[entradas][numclasses], 
				 int entrada_teste[entradas], float y_final[numclasses])
{

    //printf("\n====== TESTANDO A REDE ========\n");

    //TESTANDO A REDE
    //alterar os valores da variavel letra para testar
    //0, 1, 2 - letra A
    //3, 4, 5 - letra B
    //6, 7, 8 - letra C
    //9, 10, 11 - letra D
    //12, 13, 14 - letra E
    //15, 16, 17 - letra F

    for(int m = 0; m < numclasses; m++)
		{
        soma = 0;
        for(int n = 0; n < entradas; n++)
				{

            soma = soma + entrada_teste[n] * w[n][m];
            yin[m] = soma + w0[m];
        }
    }

    for(int m = 0; m < numclasses; m++)
		{
        if(yin[m] >= limiar)
				{

            y_final[m] = 1.0;
        }
        else
				{

            y_final[m] = -1.0;
        }

        //printf("[%.1f]", y[m]);
    }

    //printf("\n\n");

//    if (y[0] == 1.0) { printf("Letra A reconhecida\n"); }
//    if (y[1] == 1.0) { printf("Letra B reconhecida\n"); }
//    if (y[2] == 1.0) { printf("Letra C reconhecida\n"); }
//    if (y[3] == 1.0) { printf("Letra D reconhecida\n"); }
//    if (y[4] == 1.0) { printf("Letra E reconhecida\n"); }
//    if (y[5] == 1.0) { printf("Letra F reconhecida\n"); }
//    if (y[6] == 1.0) { printf("Letra G reconhecida\n"); }
    //printf("-=-=-=-=-=-=-=-=-=-=-=-\n");
}

uint8_t rand8(void)
{
    static uint8_t state[STATE_BYTES] =
    { 0x87, 0xdd, 0xdc, 0x10, 0x35, 0xbc, 0x5c };
    static uint16_t c = 0x42;
    static int i = 0;
    uint16_t t;
    uint8_t x;

    x = state[i];
    t = (uint16_t)x * MULT_LO + c;
    c = t >> 8;
#if MULT_HI
    c += x;
#endif
    x = t & 255;
    state[i] = x;
    if (++i >= sizeof(state))
        i = 0;
    return x;
}