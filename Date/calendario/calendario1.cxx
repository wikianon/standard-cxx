#include <iostream>
     
const char *months[12]={ "January","February","March","April","May","June","July","August","September","October","November","December" };
     
int month_days[12] = {31, 28, 31, 30, 31, 30, 31 ,31 ,30, 31, 30, 31};

int first_day_year(int year)
{
 int x = (year - 1.)/4.0;
 int y = (year - 1.)/100.;
 int z = (year - 1.)/400.;
     
 int first_day = (year + x - y + z) %7;
     
 return first_day;
}
     
int leapyear(int year)
{
  if(__isleap(year))return month_days[1] = 29; //agora com mês posição 1, que é o segundo
}

int calendar(int month, int year, int first_day)
{
 int i;
 month--;//agora reduz o próprio month, pois em todos os locais que é
 //utilizado daqui para a frente tem de ter 1 a menos
     
 printf("\n%s %d\n\n", months[month], year); //sem -1 aqui, pois já foi reduzido acima
 printf("Sun  Mon  Tue  Wed  Thu  Fri  Sat\n");
     
  //volta ao i=0 que era o que tinha. month também tem agora -1 que veio de cima
 for(i = 0; i < month; i++)
 first_day = ((first_day + month_days[i])%7); 
      
 for(i = 1; i <= 1 + first_day * 5; i++)printf(" ");
     
 for(i = 1; i <= month_days[month]; i++)
  {
   printf("%2d", i);
     
   if((i + first_day)%7 > 0)
    printf("   ");
   else
    printf("\n ");
  }
 printf("\n\n"); 
} 

int main()
{
 int year;
 int month;
 int first_day;
     
 do{
    printf("Enter a month (1 - 12): ");
    scanf("%d", &month);
   }while(month < 1 || month > 12);
     
 do{
    printf("Enter a year (1000 - 3000): ");
    scanf("%d", &year);
   }while(year < 1000 || year > 3000);
   
   first_day = first_day_year(year);
     
   leapyear(year);
   
   calendar(month, year, first_day);
   
 return 0;
}