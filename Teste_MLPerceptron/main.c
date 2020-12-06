#include <stdio.h>
#include <stdlib.h>
#include <math.h>



int main()
{
    /* --- VARIAVEIS ---*/
    FILE *arq, *arq2;

    arq = fopen("amostras.txt", "r");

    if (arq == NULL)
    {
        printf("ERRO! O arquivo não foi aberto!\n");
    }
    else
    {
        printf("Arquivo de amostras foi aberto com sucesso!\n");
    }

    unsigned int iseed;
    char c, c_aux, c1, c1_aux;
    int padrao = 0;
    int lin = 0;
    int col = 0;
    int entradas = 256;
    int qtd_neuronios = 100;
    int amostras_por_digito = 5;
    int qtd_digitos = 10;
    int tam_vetor_saida = 10; //10 algarismos
    int amostras = amostras_por_digito * tam_vetor_saida;
    float limiar = 0.0;
    float alfa = 0.001; //taxa aprendizagem
    float erro_tolerado = 0.05;

    int ordem[amostras];
    int cont = 0;

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
    float aux_z_inicial = 0.0;
    float z[1][qtd_neuronios];

    float y_inicial[1][tam_vetor_saida];
    float y[1][tam_vetor_saida];

    float delta_w[tam_vetor_saida][qtd_neuronios];
    float delta_w0[tam_vetor_saida][1];
    float delta_w_transp[qtd_neuronios][tam_vetor_saida];
    float delta_w0_transp[1][tam_vetor_saida];
    float delta_v[qtd_neuronios][entradas];
    float delta_v0[1][qtd_neuronios];
    float delta_v_transp[entradas][qtd_neuronios];
    float delta_v0_transp[qtd_neuronios][1];
    float deltinha[1][qtd_neuronios];
    float deltinha2[qtd_neuronios][1];
    float deltinha_k[tam_vetor_saida][1];
    float deltinha_k_transp[1][tam_vetor_saida];
    float deltinha_in[1][qtd_neuronios];

    float entrada_aux[1][entradas];
    float entrada_testes[qtd_digitos][entradas];
    float h[tam_vetor_saida][1];
    float target_comp[tam_vetor_saida][1];
    float soma = 0.0;
    int ciclo = 0;
    float erro_total = 1000;
    int letra;

    float aleatorio = 0.2;

    float deltaaux= 0;
    float errotemporario=0;
    float mediadeltaaux=0;
    float mediadeltaauxant=0;
    float alfaaux=0.1;
    float theta=0.000000001;
    float dec=0.7;
    float uu=1.01;

    float entrada_teste[entradas];

    for(int n = 0; n < amostras; n++)
    {
        ordem[n] = 0;
    }

    for(int m = 0; m < tam_vetor_saida; m++)
    {
        for(int n = 0; n < amostras_por_digito; n++)
        {
            ordem[cont] = m;
            cont++;
        }
    }

    /* --- ARQUIVO DE AMOSTRAS DE TREINAMENTO ---*/
    float entrada[amostras][entradas];
    float target[tam_vetor_saida][qtd_digitos];

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
            printf("\n\n\n");
        }
        if(c == '-')
        {
            //entrada[lin][col] = -1.0;
            col++;
            c_aux = 0;
            printf("#");
            /*if((col == 15) || (col == 30) || (col == 45) || (col == 60) || (col == 75) || (col == 90)
               || (col == 105) || (col == 120) || (col == 135) || (col == 150) || (col == 165) || (col == 180)
               || (col == 195) || (col == 210) || (col == 225) || (col == 240) || (col == 255))
            {
                printf("\n");
            }*/
            if(col%16 == 0)
            {
               printf("\n");
            }
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
                    printf("-");
                    /*if((col == 15) || (col == 30) || (col == 45) || (col == 60) || (col == 75) || (col == 90)
                        || (col == 105) || (col == 120) || (col == 135) || (col == 150) || (col == 165) || (col == 180)
                        || (col == 195) || (col == 210) || (col == 225) || (col == 240) || (col == 255))
                    {
                        printf("\n");
                    }*/
                    if(col%16 == 0)
                    {
                       printf("\n");
                    }
                }
            }
        }
    }
    printf("Arquivo de amostras foi aberto! Pressione qualquer tecla para continuar...\n");
    getch();
    /* == TESTE ==
    for(int j = 0; j < entradas; j++)
    {
        printf(" %f", entrada[49][j]);
    }
    printf("\n\n");
    /*===========*/

    /* --- PREENCHIMENTO MATRIZ TARGET ---*/
    for(int i = 0; i < tam_vetor_saida; i++)
    {
        for(int j = 0; j < qtd_digitos; j++)
        {
            target[i][j] = -1.0;
        }
    }

    for(int j = 0; j < 10; j++)
    {
        target[j][j] = 1.0;
    }

    /* == TESTE ==*/
    /*for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < tam_vetor_saida; j++)
        {
            printf(" %.2f", target[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");*;

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
        soma = 0.0;

        for(int t = 0; t < tam_vetor_saida; t++)
        {
            y_inicial[0][t] = 0.0;
        }

        for(int j = 0; j < qtd_neuronios; j++)
        {
            z_inicial[0][j] = 0.0;
        }

        for(padrao = 0; padrao < amostras; padrao++)
        {
            for(int j = 0; j < qtd_neuronios; j++)
            {
                for(int t = 0; t < entradas; t++)
                {
                    soma = soma + (entrada[padrao][t] * v_anterior[t][j]);
                    //aux_z_inicial += (entrada[padrao][t] * v_anterior[t][j]);
                   // zin[0][j] = np.dot(x[padrao,:], vanterior[:, j]) + v0anterior[0][j];
                }

                //z_inicial[0][j] = aux_z_inicial + v0_anterior[0][j];
                z_inicial[0][j] = soma + v0_anterior[0][j];
                z[0][j] = tanh(z_inicial[0][j]);
                soma = 0.0;
            }

            //////////////////////////////////////////////////////////////////

            //multiplica_matriz(1, qtd_neuronios, tam_vetor_saida, z, w_anterior, y_inicial);

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                for(int j = 0; j < qtd_neuronios; j++)
                {
                    soma = soma + (z[0][j] * w_anterior[j][t]);
                    //y_inicial[0][t] += (z[0][j] * w_anterior[j][t]);
                }
                y_inicial[0][t] = soma + w0_anterior[0][t];
                y[0][t] = tanhf(y_inicial[0][t]);
                h[t][0] = y[0][t];
                soma = 0.0;
                /*for(int t = 0; t < tam_vetor_saida; t++)
                {
                    y_inicial[0][t] = w0_anterior[0][t] + aux_z_inicial;
                    y[0][t] = tanhf(y_inicial[0][t]);
                    h[t][0] = y[0][t];
                }*/
            }

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                target_comp[t][0] = target[t][ordem[padrao]];
            }
            //aux_z_inicial = 0;
            ////////////////////////////////////////////////////////////////////

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                soma = soma + (powf((target_comp[t][0] - h[t][0]), 2.0));
                //erro_total = erro_total + 0.5 * (powf((target_comp[t][0] - y[0][t]), 2.0));
            }
            erro_total += 0.5 * soma;
            soma = 0.0;

            /* --- OBTER MATRIZES PARA ATUALIZACAO DOS PESOS ---*/
            for(int t = 0; t < tam_vetor_saida; t++)
            {
                deltinha_k[t][0] = (target_comp[t][0] - y[0][t]) * (1 + h[t][0]) * (1 - h[t][0]);
            }

            //multiplica_matriz(tam_vetor_saida, 1, qtd_neuronios, deltinha_k, z, delta_w);

            for(int u = 0; u < qtd_neuronios; u++)
            {
                for(int t = 0; t < tam_vetor_saida; t++)
                {
                    //delta_w[t][u] = alfa * delta_w[t][u];
                    delta_w[t][u] = alfa * deltinha_k[t][0] * z[0][u];
                }
            }

            for(int t = 0; t < tam_vetor_saida; t++)
            {
                delta_w0[t][0] = alfa * deltinha_k[t][0];
            }

            //transpose(tam_vetor_saida, 1, deltinha_k, deltinha_k_transp);
            //transpose(qtd_neuronios, tam_vetor_saida, w_anterior, w_anterior_transp);
            //multiplica_matriz(1, tam_vetor_saida, qtd_neuronios, deltinha_k_transp, w_anterior_transp, deltinha_in);
            for(int u = 0; u < qtd_neuronios; u++)
            {
                //deltinha[0][u] = deltinha_in[0][u] * (1 + z[0][u]) * (1 - z[0][u]);
                for(int t = 0; t < tam_vetor_saida; t++)
                {
                    //deltinha_in[0][u] += (deltinha_k[t][0] * w_anterior[u][t]);
                    soma = soma + (deltinha_k[t][0] * w_anterior[u][t]);
                }
                deltinha_in[0][u] = soma;
                soma = 0.0;
            }

            for(int u = 0; u < qtd_neuronios; u++)
            {
                deltinha[0][u] = deltinha_in[0][u] * (1 + z[0][u]) * (1 - z[0][u]);

            }

            /*for(int m = 0; m < qtd_neuronios; m++)
            {
                deltinha2[m][0] = deltinha[0][m];
            }*/
            //transpose(0, qtd_neuronios, deltinha, deltinha2);

            for(int k = 0; k < entradas; k++)
            {
                entrada_aux[0][k] = entrada[padrao][k];
            }

            //multiplica_matriz(qtd_neuronios, 1, entradas, deltinha2, entrada_aux, delta_v);
            for(int n = 0; n < entradas; n++)
            {
                for(int m = 0; m < qtd_neuronios; m++)
                {
                    //delta_v[m][n] = delta_v[m][n] * alfa;
                    delta_v[m][n] = alfa * deltinha[0][m] * entrada[padrao][n];
                }
            }

            for(int u = 0; u < qtd_neuronios; u++)
            {
                delta_v0[0][u] = alfa * deltinha[0][u];
            }

            /* --- REALIZANDO ATUALIZACAO DOS PESOS ---*/
            //transpose(qtd_neuronios, entradas, delta_v, delta_v_transp);
            for(int m = 0; m < entradas; m++)
            {
                for(int n = 0; n < qtd_neuronios; n++)
                {
                    //v_novo[m][n] = v_anterior[m][n] + delta_v_transp[m][n];
                    v_novo[m][n] = v_anterior[m][n] + delta_v[n][m];
                }
            }

            //transpose(1, qtd_neuronios, delta_v0, delta_v0_transp);
            for(int n = 0; n < qtd_neuronios; n++)
            {
                v0_novo[0][n] = v0_anterior[0][n] + delta_v0[0][n];
            }

            //transpose(tam_vetor_saida, qtd_neuronios, delta_w, delta_w_transp);
            for(int m = 0; m < qtd_neuronios; m++)
            {
                for(int n = 0; n < tam_vetor_saida; n++)
                {
                    //w_novo[m][n] = w_anterior[m][n] + delta_w_transp[m][n];
                    w_novo[m][n] = w_anterior[m][n] + delta_w[n][m];
                }
            }

            //transpose(tam_vetor_saida, 1, delta_w0, delta_w0_transp);
            for(int n = 0; n < tam_vetor_saida; n++)
            {
               //w0_novo[0][n] = w0_anterior[0][n] + delta_w0_transp[0][n];
               w0_novo[0][n] = w0_anterior[0][n] + delta_w0[n][0];
            }

            for(int m = 0; m < entradas; m++)
            {
                for(int n = 0; n < qtd_neuronios; n++)
                {
                    v_anterior[m][n] = v_novo[m][n];
                }
            }

            for(int n = 0; n < qtd_neuronios; n++)
            {
                v0_anterior[0][n] = v0_novo[0][n];
            }

            for(int m = 0; m < qtd_neuronios; m++)
            {
                for(int n = 0; n < tam_vetor_saida; n++)
                {
                   w_anterior[m][n] = w_novo[m][n];
                }
            }

            for(int n = 0; n < tam_vetor_saida; n++)
            {
                w0_anterior[0][n] = w0_novo[0][n];
            }
        }

        ciclo++;

        // Alfa adaptativo.
       if (ciclo>1)
       {
            deltaaux = (erro_total-errotemporario)/erro_total;
            mediadeltaaux = alfaaux*deltaaux + (1-alfaaux)*mediadeltaauxant;
            if (((deltaaux*mediadeltaauxant) < 0)&&(abs(mediadeltaauxant))>theta)
            {
                alfa = dec*alfa;
            }

            else
            {
                alfa = uu*alfa;
            }

            mediadeltaauxant = mediadeltaaux;
            errotemporario = erro_total;
       }


        printf("\nCiclo: %d \tTaxa de Aprendizagem: %f \t    Erro: %.2f", ciclo, alfa, erro_total);
        //printf("\ndeltaaux: %f \tmediadeltaauxant: %f", deltaaux, mediadeltaauxant);
    }
    printf("\nRede Treinada com Sucesso. Pressione qualquer tecla para continuar...\n\n");
    getch();

    //teste manual
    arq2 = fopen("testes.txt", "r");

    if (arq2 == NULL)
    {
        printf("ERRO! O arquivo não foi aberto!\n");
    }
    else
    {
        printf("Arquivo de testes aberto com sucesso!\n");
    }

    for(int i = 0; i < qtd_digitos; i++)
    {
        for(int j = 0; j < entradas; j++)
        {
            entrada_testes[i][j] = -1.0;
        }
    }

    lin = 0;

    while(c1 != EOF)
    {
        c1 = getc(arq2);

        if(c1 == 'p')
        {
            c1_aux = 0;
            lin++;
            col = 0;
            printf("\n\n\n");
        }
        if(c1 == '-')
        {
            col++;
            c1_aux = 0;
            printf("#");
            if(col%16 == 0)
            {
               printf("\n");
            }
        }
        else
        {
            if(c1 == ' ')
            {
                c1_aux = 1;
            }
            else
            {
                if((c1 == '1') && (c1_aux == 1))
                {
                    entrada_testes[lin][col] = 1.0;
                    col++;
                    c1_aux = 0;
                    printf("-");
                    if(col%16 == 0)
                    {
                       printf("\n");
                    }
                }
            }
        }
    }

    printf("\n====================================================\n");
    printf("Digite qual algarismo deseja testar (0 a 9)\n");
    scanf("%i",&letra);
    printf("\n\n\n");

    for(int col = 0; col < entradas; col++)
    {
        entrada_teste[col] = entrada_testes[letra][col];

        if(col%16 == 0)
        {
           printf("\n");
        }

        if(entrada_teste[col] == 1.0)
        {
            printf("#");
        }
        else
        {
            printf(" ");
        }
    }

    printf("\n\n\n");

    for (int m = 0; m < tam_vetor_saida; m++)
    {
        for (int n = 0; n < qtd_neuronios; n++)
        {
            for (int o = 0; o < entradas; o++)
            {
                soma += (entrada_teste[o] * v_anterior[o][n]);
            }
            z_inicial[0][n] = soma + v0_anterior[0][n];
            z[0][n] = tanh(z_inicial[0][n]);
            soma = 0;
        }

        for (int n = 0; n < qtd_neuronios; n++)
        {
            soma += z[0][n] * w_anterior[n][m];
        }
        y_inicial[0][m] = soma + w0_anterior[0][m];
        y[0][m] = tanh(y_inicial[0][m]);
        soma = 0;

        printf("[%f]", y[0][m]);
    }

    printf("\n\n\n");

    for(int j = 0; j < tam_vetor_saida; j++)
    {
        if (y[0][j] >= limiar)
        {
            y[0][j] = 1.0;
        }

        else
        {
            y[0][j] = -1.0;
        }

        printf("[%.0f]", y[0][j]);
    }
    printf("\n\n\n");
    if (y[0][0] == 1.0) { printf("0 reconhecido\n"); }
    if (y[0][1] == 1.0) { printf("1 reconhecido\n"); }
    if (y[0][2] == 1.0) { printf("2 reconhecido\n"); }
    if (y[0][3] == 1.0) { printf("3 reconhecido\n"); }
    if (y[0][4] == 1.0) { printf("4 reconhecido\n"); }
    if (y[0][5] == 1.0) { printf("5 reconhecido\n"); }
    if (y[0][6] == 1.0) { printf("6 reconhecido\n"); }
    if (y[0][7] == 1.0) { printf("7 reconhecido\n"); }
    if (y[0][8] == 1.0) { printf("8 reconhecido\n"); }
    if (y[0][9] == 1.0) { printf("9 reconhecido\n"); }
    if (y[0][0] == y[0][1] == y[0][2] == y[0][3] == y[0][4] == y[0][5] == y[0][6] == y[0][7] == y[0][8] == y[0][9] == -1.0)
    {
        printf("Nenhum reconhecido\n");
    }
    printf("-=-=-=-=-=-=-=-=-=-=-=-\n");
    getch();

    return 0;
}
