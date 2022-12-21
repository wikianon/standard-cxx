#include <cmath>
#include <iostream>

struct DiffDate
{
 int d, m, y;
};

int daysTill(int);
int daysDiff(DiffDate, DiffDate);

int main()
{
 DiffDate d1, d2;
 
 printf("Digite a primeira data: "); 
 scanf("%d/%d/%d",&d1.d,&d1.m,&d1.y);
   
 printf("Digite a segunda data: ");
 scanf("%d/%d/%d",&d2.d,&d2.m,&d2.y);
 
 printf("%d\n",daysDiff(d1,d2));
 
 return 0; 
}

int daysTill(int month)
{
 int days=0;
 
 switch(month)
  {
   case 1: days=0; break;
   
   case 2: days=31; break;
   
   case 3: days=59; break;
   
   case 4: days=90; break;
   
   case 5: days=120; break;
   
   case 6: days=151; break;
   
   case 7: days=181; break;
   
   case 8: days=212; break;
   
   case 9: days=243; break;
   
   case 10: days=273; break;
   
   case 11: days=304; break;
   
   case 12: days=334; break;
  }
 return days;
}

int daysDiff(DiffDate d1, DiffDate d2)
{
 int dd1=0, dd2=0, y, yref;
 
 yref=(d1.y<d2.y)?d1.y:d2.y;
  for(y=yref;y<d1.y;y++)
  if(__isleap(y))
  dd1++;
  
  if(__isleap(d1.y) && d1.m>2) dd1++;
  dd1+=daysTill(d1.m)+d1.d+(d1.y-yref)*365;
  
  for(y=yref;y<d2.y;y++)
  if(__isleap(y))
  dd2++;
  if(__isleap(y) && d2.m>2) dd2++;
  dd2+=daysTill(d2.m)+d2.d+(d2.y-yref)*365;
  return abs(dd2-dd1);
}