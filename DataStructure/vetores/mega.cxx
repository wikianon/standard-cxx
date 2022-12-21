//https://www.vivaolinux.com.br/script/Mega-Sena

#include <iostream>

/*Algoritmo que simula um jogo da Mega-Sena. Primeiro, armazena 6 números
digitados pelo usuário. Depois, faz um sorteio e armazena 6 números em outro
vetor. Por último, compara os dois vetores e verifica quantos números 
são iguais.*/

// Prototipação
int main();
void mostrar_num();
void ordenar_num();
void sorteio();
void ordernar_sort();
void mostrar_sort();
void result();
void menu_final();

// Variáveis globais
int i, num[6], sort[6], j, aux;



int main()
{
    int igual;
    
    std::cout << "\n\n--------------------\n"
                 "MEGA SENA\n"
                 "--------------------\n\n";
    
    do
    {
          for (i=0,j=1;i<6;i++,j++)
          {
              std::cout << "\n";
              do
              {
                   std::cout << "Digite o " << j << " numero: ";
                   std::cin >> num[i];
                   
                   if (num[i]>60)
                   {
                      std::cout << "Voce digitou um numero maior que 60. Digite novamente.\n";
                   }
                   
              } while (num[i]>60); // verifica se os numeros sao maiores que 60
          } // fim do for
          
          igual=0;
        
           for (i=0;i<6;i++) // verifica se o usuario digitou numeros iguais
           {
               for (j=i+1;j<=5;j++)
               {
                   if (num[i]==num[j])
                   {
                      igual++;
                   }
               } // fim do for
           } // fim do for
               
           if (igual>0) // 
              std::cout << "\nATENCAO! Voce digitou numeros iguais. Digite a sequencia novamente.\n";   
           
     } while (igual>0); // verifica se os numeros são iguais.
    
    mostrar_num();

} // fim do int main



void mostrar_num()
{
     ordenar_num(); // chama o modulo qie ordena o vetor.
     
     std::cout << "\n\nVoce escolheu os seguintes numeros: \n\n";
     
     for (i=0;i<6;i++)
    {
        std::cout << num[i] << "   ";
    }
    
    std::cout << "\n\nAperte qualquer tecla para efetuar o sorteio...";
    
    getchar();
    
    sorteio();
    
} // fim do void mostrar_num



void ordenar_num()
{
     // ordenar o vetor
    for (i=0;i<6;i++)
    {
        for (j=i+1;j<=5;j++)
        {
            if (num[j] < num[i])
            {
               aux=num[i];
               num[i]=num[j];
               num[j]=aux;
            }
        }
    }
} // fim do void ordenar_num





void sorteio()
{
     int igual;
     
     srand (time(0));
     
     do
     {
           
           for (i=0;i<6;i++) // for que sorteia os números
           {
               sort[i]=rand()%60; // ou sorteio[i]=random(60)
           }
     
           igual=0;
        
           for (i=0;i<6;i++) // for que verifica se foram sorteados 2 números iguais
           {
               for (j=i+1;j<=5;j++)
               {
                   if (sort[i]==sort[j])
                   {
                      igual++;
                   }
               } // fim do for
           } // fim do for
           
     } while (igual>0); // Se números iguais foram sorteados, o sorteio será ralizado novamente
     mostrar_sort();
     
} // fim do void sorteio



void ordenar_sort()
{
     // ordenar o vetor
    for (i=0;i<6;i++)
    {
        for (j=i+1;j<=5;j++)
        {
            if (sort[j] < sort[i])
            {
               aux=sort[i];
               sort[i]=sort[j];
               sort[j]=aux;
            }
        }
    }
}



void mostrar_sort()
{
     ordenar_sort();
     
     std::cout << "\n\nNumeros sorteados:  \n";
     
     for (i=0;i<6;i++) // Mostrar os números sorteados
     {
         std::cout << sort[i] << "   ";
     }
     
     std::cout << "\n\nAperte qualquer tecla para ver o resultado...\n\n";
     
     getchar();
     
     result();
} // fim do void mostrar_sort




void result()
{
     int iguais[6], acertos=0, k=0;
     
     for (i=0;i<6;i++) // controla o vetor dos números do usuário
     {
         for (j=0;j<6;j++) // controla o vetor dos números sorteados
         {
             if (num[i]==sort[j]) // verifica se os numeros são iguais
             {
                 iguais[k]=num[i]; //se forem iguais, copia os numeros iguais para um outro vetor
                 acertos++;
                 k++;
                 j=5;
             }
             
             if (num[i] < sort[j])
             {
                 j=5;
             }
         } // fim do for 
     } // fim do for
     
     std::cout << "Voce teve " << acertos <<  " acerto(s): ";
     
     for (i=0;i<acertos;i++) // mostrar apenas os numeros iguais
     {
         std::cout << iguais[i] << "   ";
         //getchar();
     }
     
     // getchar();
     
     menu_final();
     
} //fim do void result



void menu_final()
{
     char opcao;
     int a;
     
     std::cout << "\n\n\n1 - Digitar outra sequencia\n";
     std::cout << "2 - SAIR\n\n";
     
     do
     {
     a=0;
     
     std::cout << "Digite a sua opcao: ";
     std::cin >> opcao;
     
     
          switch (opcao)
          {
                 case'1': main();
                          break;
                 case'2': break;
                 default : std::cout << "Opcao invalida.";
                           a++;
          }
     } while (a=1);//Verifica se o usuário digitou uma opção válida ou não
     
     getchar();
     
} //fim do void menu_final
