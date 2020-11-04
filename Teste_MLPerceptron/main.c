#include <stdio.h>
#include <stdlib.h>

int main()
{
    /* --- VARIAVEIS ---*/
    FILE *arq;

    arq = fopen("amostras.txt", "r");

    if (arq == NULL)
    {
        printf("ERRO! O arquivo não foi aberto!\n");
    }
    else
    {
        printf("O arquivo foi aberto com sucesso!\n");
    }

    unsigned int iseed;
    char c, c_aux;
    int lin = 0;
    int col = 0;
    int entradas = 256;
    int qtd_neuronios = 200;
    int amostras_por_digito = 5;
    int tam_vetor_saida = 10; //10 algarismos
    int amostras = amostras_por_digito * tam_vetor_saida;
    float limiar = 0.0;
    float alfa = 0.005; //taxa aprendizagem
    float erro_tolerado = 0.5;

    float v_anterior[entradas][qtd_neuronios];
    float v0_anterior[1][qtd_neuronios];
    float w_anterior[qtd_neuronios]{tam_vetor_saida];
    float w0_anterior[1][tam_vetor_saida];

    float aleatorio = 0.2;

    /* --- ARQUIVO DE AMOSTRAS DE TREINAMENTO ---*/
    float entrada[amostras][entradas];
    float target[10][tam_vetor_saida];

    for(int i = 0; i < amostras; i++)
    {
        for(int j = 0; j < entradas; j++)
        {
            entrada[i][j] = -1.0;
        }
    }


    while(c != EOF)
    {
        c = getc(arq);

        if(c == 'p')
        {
            c_aux = 0;
            lin++;
            col = 0;
        }
        if(c == '-')
        {
            //entrada[lin][col] = -1.0;
            col++;
            c_aux = 0;
        }
        else
        {
            if(c == ' ')
            {
                c_aux = 1;
            }
            else
            {
                if((c == '1') && (c_aux == 1))
                {
                    entrada[lin][col] = 1.0;
                    col++;
                    c_aux = 0;
                }
            }
        }
    }

    /* == TESTE ==*/
    for(int j = 0; j < entradas; j++)
    {
        printf(" %f", entrada[49][j]);
    }
    printf("\n\n");
    /*===========*/

    /* --- PREENCHIMENTO MATRIZ TARGET ---*/
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < tam_vetor_saida; j++)
        {
            target[i][j] = -1.0;
        }
    }

    for(int j = 0; j < 10; j++)
    {
        target[j][j] = 1.0;
    }

    /* == TESTE ==*/
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < tam_vetor_saida; j++)
        {
            printf(" %.2f", target[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

    /* --- GERANDO PESOS SINAPTICOS ALEATORIAMENTE ---*/
    for(int i = 0; i < entradas; i++)
    {
        for(int j = 0; j < qtd_neuronios; j++)
        {
            iseed = (unsigned int)time(NULL);
            v_anterior[i][j] = (aleatorio*2) * (float)rand() / (float)RAND_MAX - aleatorio;
        }
    }

    for(int j = 0; j < qtd_neuronios; j++)
    {
        iseed = (unsigned int)time(NULL);
        v0_anterior[0][j] = (aleatorio*2) * (float)rand() / (float)RAND_MAX - aleatorio;
    }

    for(int i = 0; i < qtd_neuronios; i++)
    {
        for(int j = 0; j < tam_vetor_saida; j++)
        {
            iseed = (unsigned int)time(NULL);
            w_anterior[i][j] = (aleatorio*2) * (float)rand() / (float)RAND_MAX - aleatorio;
        }
    }

    for(int j = 0; j < tam_vetor_saida; j++)
    {
        iseed = (unsigned int)time(NULL);
        w0_anterior[0][j] = (aleatorio*2) * (float)rand() / (float)RAND_MAX - aleatorio;
    }

    /* --- MATRIZES DE ATUALIZACAO DE PESOS E VALORES DE SAIDA DA REDE ---*/


    return 0;
}
