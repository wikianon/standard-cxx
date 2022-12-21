#include <stdio.h>

int main()
{
 int ano;
 int g=0, c=0, x=0, z=0, e=0, n=0, d=0;
 int dia,dsemana=7,bis;
 int aux_m=0,dia_aux,semana_aux,Lim, modificador;      
 int m1=31,m2=0,m3=31;
 int Mtx[6][7],Mtx2[6][7];
 int i,j,k,l;

 /*PARTE I - Encontrar dia da Páscoa, localizar em que dia da semana cai 1º de Janeiro em um determinado Ano.*/
 
 printf("Digite um ano para gerar o calendario: ");
 scanf("%d",&ano);
 
 // Calculo do dia da Pascoa
 if(ano%100 == 0)
  {
   ano= ano/100;               
  }
 if(ano%4 == 0)
  {
   bis=1;                       
   m2=29;
  }
  else
  {
   bis=0;
   m2=28;
  }
  
  g = (ano%19)+1;
  c = (ano/100)+1;
  x = ((3*c)/4) -12;
  z = (((8*c)+5)/25)-5;
  e = ((11*g)+20+z-x)%30;
  
  if((e == 25 && g > 11) || e == 24)
  {
   e=e+1;
  }
  
  n=44-e;

  if(n < 21)
   {
    n=n+30;
   }
   
  d=((5*ano)/4) - (x+10);
  
  n=(n+7)-((d+n)%7);
  
  if(n > 31)
   {
    dia=n-31;  
    aux_m=4;             
   }
   else
   {
    dia=n; 
    aux_m=3;
   }      
  
  // Calculo para achar 1º de Janeiro e em que dia da semana ira cair.
  do{
     if(dia == 1 && aux_m > 1)
      {
       aux_m-=1;
       if(aux_m == 3)
        {
         dia=m3;
         if(dsemana == 1)
         dsemana=7;
         else                              
         dsemana -= 1;
        }
       
       if(aux_m == 2)
        {
         dia=m2;
         if(dsemana == 1)
         dsemana=7;
         else                              
         dsemana -= 1;
        }
        
       if(aux_m == 1)
        {
         dia=m1;
         if(dsemana == 1)
         dsemana=7;
         else                              
         dsemana -= 1;
        }
      }
      
     if(dia!=1)
      {
       if(dsemana == 1)
        {
         dsemana=7;
         dia = dia-1;
        }
        else
        {
         dia = dia-1;
         dsemana =dsemana-1;
        }
      }
  }while(aux_m != 1 || dia != 1); 

   /* 
    --------------------------------------------------------------------------------------		
    PARTE II - MONTAGEM GRÁFICA
    Aqui serão gerado os dados para entrar no calendário.
    Os dados de entrada para este próximo passo é o Dia = 1 , Dia da Semana e mês = 1;
    ---------------------------------------------------------------------------------------
   */
  
   // Estabelece parâmetros para iniciar o preenchimento da matriz, que em cada ira armazenar 1 mês do ano por vez.
   dia_aux=dia;
   semana_aux=dsemana;  
   Lim= 31;
   
   printf("Calendario do ano %d\n",ano);
   
   do{
      //LOOP auxiliar para  encaixar 1º dia do mês na posição de linha e coluna correta na matriz
      // Se trabalha com 2 Matrizes, preenchendo mêses de 2 em 2 . Ex: Janeiro e Fevereiro , Março e Abril...
      if(semana_aux != 7)
       {
        for(i=0; i<1; i++)
        for(j=0; j<=semana_aux; j++)
        if(j != semana_aux)
         {
          if(aux_m%2 == 0)
          Mtx2[i][j]=0;
          else
          Mtx[i][j]=0;
         }
         else
         {
          if(aux_m%2==0)
          Mtx2[i][j]=dia_aux;
          else
          Mtx[i][j]=dia_aux;
         }
       }
       else
       {
        if(aux_m%2 == 0)
        Mtx2[0][0]=dia_aux;
        else
        Mtx[0][0]=dia_aux;
       }
       
       if(semana_aux!=7)
        {
         semana_aux++;
        }
        else
        {
         semana_aux=0;
         semana_aux++;
        }
       // LOOP de inserção do restante dos dias do mês - OK
       for(i=0; i<6; i++)
        {
         if(i != 0)
         semana_aux=0;
         for(j=semana_aux; j<7; j++)
          {
           if(dia_aux < Lim)
            {
             dia_aux++;
             if(aux_m%2 == 0)
             Mtx2[i][j]=dia_aux;
             else
             Mtx[i][j]=dia_aux;
             
             if(dia_aux == Lim)
              {
               modificador=j;
              }
            }
            else
            {
             if(aux_m%2 == 0)
             Mtx2[i][j]=0;
             else
             Mtx[i][j]=0;
            }
           }
        }
      
        // Montagem Gráfica do Calendário
        if(aux_m%2 == 0)
         {
          if(aux_m == 2)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Janeiro                      |"
                   "|Fevereiro                    |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"
                  );
           }
           
          if(aux_m == 4)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Março                        |"
                   "|Abril                        |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"
                  );
           }
           
          if(aux_m == 6)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Maio                         |"
                   "|Junho                        |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"
                  );
           }
           
          if(aux_m == 8)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Julho                        |"
                   "|Agosto                       |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"
                  );
           }
           
          if(aux_m == 10)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Setembro                     |"
                   "|Outubro                      |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"
                  );
           }
           
          if(aux_m == 12)
           {
            printf(
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "|Novembro                     |"
                   "|Dezembro                     |\n"
                   "|-----------------------------|"
                   "|-----------------------------|\n"
                   "| dom seg ter qua qui sex sab |"
                   "| dom seg ter qua qui sex sab |\n"      
                  );
           }
           
          // Pega os dados que são os dias do mês contido nas Matrizes e imprime na tela.
          for(k=0; k<6; k++)
           {
            printf("|");
            for(l=0; l<7; l++)
             {
              if(Mtx[k][l] == 0)
               {
                printf("    ");
               }
               else
               printf("  %02d",Mtx[k][l]);
             }
             printf(" ||");
            // Segundo for para a matriz 2
            for(l=0; l<7; l++)
             {
              if(Mtx2[k][l] == 0)
              printf("    ");
               else
              printf("  %02d",Mtx2[k][l]);
             }
             printf(" |\n");
           }
         }
         // Modificador de 1º dia 
         dia_aux=1;
         semana_aux = modificador;
         if(semana_aux == 6)
          semana_aux=0;
         else
          semana_aux ++;
         aux_m++;
        
         // Auxiliar na mudança de mês e na quantidade de dias que cada um possue.			
         switch(aux_m)
          {
           case 2:
           if(bis==0)
            Lim=28;
           else
            Lim=29;
           break;
           
           case 3:
            Lim=31;
           break;
              
           case 4:
            Lim=30;
           break;
              
           case 5:
            Lim=31;
           break;
           case 6:
            Lim=30;
           break;
           
           case 7:
            Lim=31;
           break;
           
           case 8:
            Lim=31;
           break;
           
           case 9:
            Lim=30;
           break;
           
           case 10:
            Lim=31;
           break;
           
           case 11:
            Lim=30;
           break;
           
           case 12:
            Lim=31;
           break;
          }
       
     }while(aux_m != 13); // Condição para que imprima somente 12 Mêses.
     printf("|-----------------------------||-----------------------------|\n\n");
  return 0;
}