#include<stdio.h>
#include<conio.h>
#include <time.h>
#include <stdlib.h>
int main()
{
   float entrada[64][2];//64 valores e dois padrões (A e B).
   int opc = 0;
   float limiar = 0;
   float w[64]; //Pesos - um para cada entrada.
   float target[2];
   float b;
   float alfa = 0.001;
   float yLiq =0;
   int contIni = 0;
   int contCiclo=0;
   float y;
   int lin, col;
   lin = col = 0;
   int condErro=1;
   float teste;
   target[0] = 1;
   target[1] = -1;
   for(lin = 0; lin<64;lin++)
   {
      srand (time(NULL));
      w[lin]=(rand() % 100);
      w[lin] = w[lin]/100;
   }
   srand (time(NULL));
   b=(rand() % 100);
   b = b/100;

   //Inserção dos valores de entrada da rede
   while(contIni<64)
   {
      entrada[contIni][0] = entrada[contIni][1] = -1;
	  contIni++;
   }
   //Inserção de dados da letra A (entrada).
   entrada[3][0] = entrada[10][0] = entrada[12][0] = entrada[18][0] = entrada[20][0] = 1;
   entrada[25][0] = entrada[29][0] = entrada[33][0] = entrada[34][0] = entrada[35][0] = 1;
   entrada[36][0] = entrada[37][0] = entrada[41][0] = entrada[45][0] = entrada[49][0] = 1;
   entrada[53][0] = entrada[57][0] = entrada[61][0] = 1;
   //Inserção de dados da letra B(entrada).
   entrada[9][1] = entrada[10][1] = entrada[11][1] = entrada[17][1] = entrada[19][1] = 1;
   entrada[25][1] = entrada[26][1] = entrada[33][1] = entrada[34][1] = entrada[11][1] = 1;
   entrada[43][1] = entrada[49][1] = entrada[51][1] = entrada[57][1] = entrada[58][1] = 1;
   entrada[59][1] = 1;

   while(opc != 3)
   {
   	  printf("\n\n ************ Programa Perceptron: Letras A e B ************");
      printf("\n\n Digite 1 para treinar a rede");
      printf("\n Digite 2 para operar");
      printf("\n Digite 3 para Sair\n ->");
      scanf("%i",&opc);
      if(opc ==1)
      {
         while(condErro == 1)
         {//Condição de parada.
            condErro = 0;
            col = 0;
            while(col<2)
            {
               yLiq = 0;
               lin = 0;
			   while(lin<64)
			   {//Verificação de um peso para cada entrada.
                   yLiq = yLiq + (entrada[lin][col]*w[lin]);
				   lin++;
			   }
			   yLiq = yLiq + b;
               if(yLiq >= limiar)
               {
                  y = 1;
               }else{
                  y=-1;
               }
               if(y != target[col])
               {//Será necessário atualizar os pesos, pois o peso ainda não gera a saída.
                  condErro = 1;
                  lin = 0;
                  while(lin<64)
			      {
                     w[lin] = w[lin]+ (alfa* target[col]*entrada[lin][col]);
				     lin++;
			      }
			      b = b + (alfa * target[col]);
               }
               col++;
            }
           contCiclo++;
         }
         printf("\n Rede treinada - Num ciclos: %i",contCiclo);
      }
      if(opc ==2)
      {
         	printf("\n\t --->Testando a rede treinada");
         	for(col =0;col<2;col++)
         	{
         		teste = 0;
	            for(lin = 0; lin < 64; lin ++)
	            {//Somatório dos pesos.
		            teste= teste + (entrada[lin][col]*w[lin]);
		        }
	            teste = teste + b;//Finalização com Bias.
	            printf("\n Saida %f: ",teste);
	            if(teste >=0)
	            {//Fronteira para separar os dois padrões.
	            	printf(" Y = %.5f - Letra A recebida");
	            }else{
	            	printf(" Y = %.5f - Letra B recebida");
	            }

         	}
      }
  }
  return(0);
}
