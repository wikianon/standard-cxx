#include <iostream>
//http://cowboycoder.tech/article/c-co-ban-phan-19-bai-thuc-hanh-tong-hop-chuong-trinh-quan-ly-hoc-sinh
int getIntRange(int low, int high)
{
 int input;
 
 while(1)
  {
   std::cin >> input;
   
   if(input < low || input > high)
    std::cout << "\n\tPlease do not hesitate!\n\tThe range is between: " << low << " e " << high << "\n\tEnter: ";
   else
    break;
  }
 return input;
} 

int main()
{
 int input;
 while(1)
  {
     std::cout << "\n\tNumber 1"
                  "\n\tNumber 2"
                  "\n\tExit   0"
                  "\n\tEscolha: ";
                  
     input = getIntRange(0, 2);
     
     switch(input)
      {
       case 1:
        std::cout << "\n\tNumero: " << input << " pressionado.\n\n";
       break;
       
       case 2:
        std::cout << "\n\tNumero: " << input << " pressionado.\n\n";
       break;
       
       case 0:
        std::cout << "\n\tGood Bye.!!\n\n"; 
        return 0;
      }
   }
 return 0; 
}