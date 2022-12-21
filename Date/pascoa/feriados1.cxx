#include <iostream>

struct Calendario
{
 static bool hrVeraoBrasil(const time_t);
 static time_t Pascoa(int);
 static time_t Carnaval(int);
 static int numDomingosCarnavalFevereiro(int);
 
 protected:
  static int contaDomingos(const time_t);
}; 

bool Calendario::hrVeraoBrasil(const time_t timestamp)
{
 int ret = 0;
 struct tm *pstamp = NULL;
 time_t ltimestamp = timestamp;
 int domingos = 0;
 
 pstamp = gmtime( &ltimestamp );
 domingos = contaDomingos(ltimestamp);

 if( pstamp->tm_mon > 9 || pstamp->tm_mon tm_mon == 9 ) //outubro, mes de inicio do horario de verao
  {
        if( domingos >= 3 )
        {
            ret = 1;
        }
  }
  else
 if( pstamp->tm_mon == 1 ) //fevereiro, mes de termino do horario de verao
  {
   //o termino do horario de verao e o unico que pode variar, devido ao carnaval
   int carnavalfev = numDomingosCarnavalFevereiro(pstamp->tm_year + 1900);
   if( (carnavalfev == 3) && (domingos < 4) )
    {
     ret = 1;
    }
   else
   if(domingos < 3)
    {
     ret = 1;
    }
  }
 return ret;
}

time_t Calendario::Carnaval(int ano)
{
    const time_t SETE_SEMANAS = 4233600; //em segundos

    time_t dpascoa = Pascoa(ano);

    return dpascoa - SETE_SEMANAS; //o carnaval 7 domingos antes da pascoa
}

//retorna em qual domingo de fevereiro cai o carnaval
//se retornar 0, o carnaval e em outro mes
int Calendario::numDomingosCarnavalFevereiro(int ano)
{
    const time_t UMA_SEMANA = 604800; //em segundos
    int contador = 0;

    time_t data = Carnaval(ano);

    tm *ptm = gmtime(&data);

    while(ptm->tm_mon == 1)
    {
        contador++;

        data -= UMA_SEMANA;

        ptm = gmtime(&data);
    }

    return contador;
}

//traducao direta do algoritmo em basic disponivel em:
//http://www.assa.org.au/edm.html#Computer
//valido para o periodo de anos de 1583 a 4099
//comentarios foram preservados como no original, em ingles
time_t Calendario::Pascoa(int ano)
{
    int FirstDig, Remain19, temp; //intermediate results
    int tA, tB, tC, tD, tE; //table A to E results
    int d,m;

    if( ano > 4099 )
    {
        //nao, certamente nao se espera que este codigo esteja em execucao por essas epocas
        //mas um bug da rotina chamadora já é algo muito mais provavel
        perror("Ano deve estar entre 1583 e 4099");
        return 0; 
    }

    FirstDig = ano / 100; //first 2 digits of year
    Remain19 = ano % 19; //remainder of year / 19

    //calculate PFM date
    temp = (FirstDig - 15) / 2 + 202 - 11 * Remain19;

    switch(FirstDig)
    {
        case 21:
        case 24:
        case 25:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 34:
        case 35:
        case 38:
            temp = temp - 1;
            break;
        case 33:
        case 36:
        case 37:
        case 39:
        case 40:
            temp = temp - 2;
                break;
    }

    temp = temp % 30;

    tA = temp + 21; 
    if( temp == 29 ) tA = tA - 1; 
    if (temp == 28 && Remain19 > 10) tA = tA - 1;

    //find the next Sunday 
    tB = (tA - 19) % 7; 

    tC = (40 - FirstDig) % 4; 
    if( tC == 3 ) tC = tC + 1; 
    if( tC > 1 ) tC = tC + 1; 

    temp = ano % 100; 
    tD = (temp + temp / 4) % 7; 

    tE = ((20 - tB - tC - tD) % 7) + 1; 
    d = tA + tE;

    //return the date
    if( d > 31 )
    { 
        d = d - 31; 
        m = 4; 
    }
    else
    {
        m = 3;
    }

    //agora convertemos isto em um timestamp e retornamos  
    tm data, *pdata = &data;

    memset(&data, 0, sizeof(data));

    pdata->tm_mday = d; 
    pdata->tm_mon = m - 1;
    pdata->tm_year = ano - 1900;

    time_t timestamp = mktime(pdata);

    return timestamp;
}