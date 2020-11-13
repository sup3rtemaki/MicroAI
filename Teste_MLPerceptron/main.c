#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void multiplica_matriz(int r1, int c1, int c2, float A[r1][c1], float B[][c2], float C[r1][c2]);
void transpose(int r1, int c1, float A[r1][c1]);

//Multiplica duas matrizes
void multiplica_matriz(int r1, int c1, int c2, float A[r1][c1], float B[][c2], float C[r1][c2])
{
    for (int i = 0; i < r1; ++i)
    {
      for (int j = 0; j < c2; ++j)
      {
         for (int k = 0; k < c1; ++k)
         {
            C[i][j] += A[i][k] * B[k][j];
         }
      }
   }
}

//Transpoe matriz
void transpose(int r1, int c1, float A[r1][c1], float B[c1][r1])
{
    for (r1 = 0; r1 < m; r1++)
    {
        for (c1 = 0; c1 < n; c1++)
        {
            B[c1][r1] = A[r1][c1];
        }
    }
}

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
    int qtd_digitos = 10;
    int tam_vetor_saida = 10; //10 algarismos
    int amostras = amostras_por_digito * tam_vetor_saida;
    float limiar = 0.0;
    float alfa = 0.005; //taxa aprendizagem
    float erro_tolerado = 0.5;

    float v_anterior[entradas][qtd_neuronios];
    float v0_anterior[1][qtd_neuronios];
    float w_anterior[qtd_neuronios][tam_vetor_saida];
    float w0_anterior[1][tam_vetor_saida];
    float w_anterior_transp[tam_vetor_saida][qtd_neuronios];

    /* --- MATRIZES DE ATUALIZACAO DE PESOS E VALORES DE SAIDA DA REDE ---*/
    float v_novo[entradas][qtd_neuronios];
    float v0_novo[1][qtd_neuronios];

    float w_novo[qtd_neuronios][tam_vetor_saida];
    float w0_novo[1][tam_vetor_saida];

    float z_inicial[1][qtd_neuronios];
    float aux_z_inicial = 0;
    float z[1][qtd_neuronios];

    float y_inicial[1][tam_vetor_saida];
    float y[1][tam_vetor_saida];

    float delta_w[tam_vetor_saida][tam_vetor_saida];
    float delta_w0[tam_vetor_saida][1];
    float delta_v[qtd_neuronios][entradas];
    float delta_v0[1][qtd_neuronios];
    float deltinha[1][qtd_neuronios];
    float deltinha2[qtd_neuronios][1];
    float deltinha_k[tam_vetor_saida][1];
    float deltinha_k_transp[1][tam_vetor_saida];
    float deltinha_in[1][qtd_neuronios];

    float entrada_aux[1][entradas];
    float h[tam_vetor_saida][1];
    float target_comp[tam_vetor_saida][1];
    float soma = 0;
    int ciclo = 0;
    float erro_total = 10000;

    float aleatorio = 0.2;

    /* --- ARQUIVO DE AMOSTRAS DE TREINAMENTO ---*/
    float entrada[amostras][entradas];
    float target[qtd_digitos][tam_vetor_saida];

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

    /* == TESTE ==
    for(int j = 0; j < entradas; j++)
    {
        printf(" %f", entrada[49][j]);
    }
    printf("\n\n");
    /*===========*/

    /* --- PREENCHIMENTO MATRIZ TARGET ---*/
    for(int i = 0; i < qtd_digitos; i++)
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


    /* --- IMPLEMENTACAO DA REDE ---*/
    while(erro_tolerado < erro_total)
    {
        erro_total = 0;
        for(int padrao = 0; padrao < amostras; padrao++)
        {
            for(int j = 0; j < qtd_neuronios; j++)
            {
                for(int t = 0; t < entradas; t++)
                {
                    aux_z_inicial += (entrada[padrao][t] * v_anterior[j][padrao]);
                }

                z_inicial[0][j] = aux_z_inicial + v0_anterior[0][j];
                z[0][j] = tanh(z_inicial[0][j]);
                aux_z_inicial = 0;

                for(int t = 0; t < tam_vetor_saida; t++)
                {
                    aux_z_inicial += (z[0][j] * w_anterior[j][t]);
                }
            }

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                y_inicial[0][t] = aux_z_inicial + w0_anterior[0][t];
                y[0][t] = tanh(y_inicial[0][t]);
                h[t][0] = y[0][t];
                target_comp[t][0] = target[t][ordem[padrao]];
            }
            aux_z_inicial = 0;

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                soma += ((target_comp[t][0] - h[t][0])^2);
            }
            erro_total += 0.5 * soma;
            soma = 0;

            /* --- OBTER MATRIZES PARA ATUALIZACAO DO PESOS ---*/
            for(int t = 0; t < tam_vetor_saida; t++)
            {
                deltinha_k[t][0] = (target_comp[t][0] - h[t][0]) * ((1 + h[t][0]) * (1 - h[t][0]));
            }

            multiplica_matriz(tam_vetor_saida, 1, qtd_neuronios, **deltinha_k, **z, **delta_w);

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                for(int u = 0; u < qtd_neuronios; u++)
                {
                    delta_w[t][u] = alfa * delta_w[t][u];
                }
            }

            for(int u = 0; u < tam_vetor_saida; u++)
            {
                delta_w0[u][0] = alfa * deltinha_k[u][0];
            }

            transpose(tam_vetor_saida, 1, **deltinha_k, **deltinha_k_transp);
            transpose(qtd_neuronios, tam_vetor_saida, **w_anterior, **w_anterior_transp);
            multiplica_matriz(1, tam_vetor_saida, qtd_neuronios, **deltinha_k_transp, **w_anterior_transp, **deltinha_in);

            for(int t = 0; t < qtd_neuronios; t++)
            {
                deltinha[0][t] = deltinha_in[0][t] * (1 + z[0][t]) * (1 - z[0][t]);
            }

            /*for(int m = 0; m < qtd_neuronios; m++)
            {
                deltinha2[m][0] = deltinha[0][m];
            }*/
            transpose(0, qtd_neuronios, **deltinha, **deltinha2);

            for(int k = 0; k < entradas; k++)
            {
                entrada_aux[0][k] = entrada[padrao][k];
            }

            multiplica_matriz(qtd_neuronios, 1, entradas, **deltinha2, **entrada_aux, **delta_v);
            for(int m = 0; m < qtd_neuronios; m++)
            {
                for(int n = 0; n < entradas; n++)
                {
                    delta_v[m][n] = delta_v[m][n] * alpha;
                }
            }

            for(int u = 0; u < qtd_neuronios; u++)
            {
                delta_v0[0][u] = alpha * deltinha[0][u];
            }

        }
    }

    return 0;
}
