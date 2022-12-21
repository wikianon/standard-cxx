#include <stdio.h>
/* Descobridor de 1º de Janeiro a partir da data da páscoa */
typedef struct
{
 int dia, mes, ano, cod_semana;
}Data;

Data dia_janeiro(Data data)
{
    Data d; 
    int ano = data.ano, bissexto = 0;
    while(ano % 100 == 0)
        ano /= 100;
    if(ano % 4 == 0)
        bissexto = 1;
    
    d.dia = 1;
    d.mes = 1;
    d.ano = data.ano;

    if(data.mes == 3)
        d.cod_semana = 8 - ((data.dia + 28 + 31 + bissexto) % 7);
    else if(data.mes == 4)
        d.cod_semana = 8 - ((data.dia + 31 + 28 + 31 + bissexto) % 7);
    if(d.cod_semana > 6) d.cod_semana -= 7;
    return d;
}

int main()
{
    Data pascoa;
    pascoa.dia = 1;
    pascoa.mes = 4;
    pascoa.ano = 2018;
    printf("Código do 1º de Janeiro: %d.\n", dia_janeiro(pascoa).cod_semana);
    return 0;
} 
