#include <iostream>
     
struct Mamifero
{
 Mamifero(){}
 ~Mamifero(){}
 //void somMamifero() const // Polimorfism no Efect
 virtual void somMamifero() const //Polimorfism Efect
  {
   std::cout<<"\n\tSom de mamifero.\n";
  }
       
 void menu1();
 void menu2();
 void menu3();
};
     
//The struct is a public class by default does not need to use public equal in class
struct Boi: Mamifero
{
 Boi(){}
 ~Boi(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tMuu ..! Muu..!!\n";
  }
};
     
struct Gato: Mamifero
{
 Gato(){}
 ~Gato(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tMiAu ..! MiAu..!!\n";
  }
};
     
struct Porco: Mamifero
{
 Porco(){}
 ~Porco(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tOinc ..! Oinc..!!\n";
  }
};
     
struct Cachorro: Mamifero
{
 Cachorro(){}
 ~Cachorro(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tAu ..! Au..!!\n";
  }
};
    
void Mamifero::menu1()
{
 Mamifero* mamPtr[4];
       
 Boi b;
 Gato g;
 Porco p;
 Cachorro c;
 
 //Polymorphism parametric
 mamPtr[0] = &b;
 mamPtr[1] = &g;
 mamPtr[2] = &p;
 mamPtr[3] = &c;
       
 for(int i=0; i<4;i++)mamPtr[i]->somMamifero();
}
     
void Mamifero::menu2()
{
 Mamifero* mamPtr;
 int op;
 
 while(op != 5)
  {
   std::cout << "\n\t(1) Boi"
                "\n\t(2) Gato"
                "\n\t(3) Porco"
                "\n\t(4) Cachorro"
                "\n\t(5) Sair"
                "\n\tDigite: ";
   
   std::cin >> op;
   
   switch(op)
    {
     case 1:
      mamPtr = new Boi();
      mamPtr->somMamifero();
     break;
        
     case 2:
      mamPtr = new Gato();
      mamPtr->somMamifero();
     break;
        
     case 3:
      mamPtr = new Porco();
      mamPtr->somMamifero();
     break;
       
     case 4:
      mamPtr = new Cachorro();
      mamPtr->somMamifero();
     break;
       
     case 5:
      std::cout<<"\n\tGood Bye ...!!!\n\n";
     break;
       
     default:
      std::cout<<"\n\tOpção Inválida ..!!!\n";
     break; 
    }
  }
}
     
void Mamifero::menu3()
{
 Mamifero* mamPtr[4];
 Boi b;
 Gato g;
 Porco p;
 Cachorro c;
 int op;
       
 while(op != 5)
  {
   std::cout << "\n\t(1) Boi"
                "\n\t(2) Gato"
                "\n\t(3) Porco"
                "\n\t(4) Cachorro"
                "\n\t(5) Sair"
                "\n\tDigite: ";
   std::cin >> op;
   
   switch(op)
    {
     case 1:
      mamPtr[0] = &b;
      mamPtr[0]->somMamifero();
     break;
         
     case 2:
      mamPtr[1] = &g;
      mamPtr[1]->somMamifero();
     break;
         
     case 3:
      mamPtr[2] = &p;
      mamPtr[2]->somMamifero();
     break;
          
     case 4:
      mamPtr[3] = &c;
      mamPtr[3]->somMamifero();
     break;
         
     case 5:
      std::cout<<"\n\tGood Bye ..!!!\n\n";
     break;
         
     default:
      std::cout<<"\n\tOpção Inválida ..!!!\n";
    }
  }
}

int main()
{
 Mamifero *m = new Mamifero();
 
 //(*m).menu1(); //OK
 
 //(*m).menu2(); //OK
 
 (*m).menu3(); //OK
 
 return 0;    
}