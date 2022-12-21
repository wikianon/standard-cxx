#include <iostream>

#define elif else if

class Mamifero
{
 public:
 Mamifero(){}
 ~Mamifero(){}
 //void somMamifero() const // Error:
 virtual void somMamifero() const  //Polimorfism Efect
  {
   std::cout<<"\n\tSom de mamifero.\n";
  }
       
 void menu();
};
     
class Boi: public Mamifero
{
 public:
 Boi(){}
 ~Boi(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tMuu ..! Muu..!!\n";
  }
};
     
class Gato: public Mamifero
{
 public:
 Gato(){}
 ~Gato(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tMiAu ..! MiAu..!!\n";
  }
};
     
class Porco: public Mamifero
{
 public:
 Porco(){}
 ~Porco(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tOinc ..! Oinc..!!\n";
  }
};

class Cachorro: public Mamifero
{
 public:
 Cachorro(){}
 ~Cachorro(){}
 
 void somMamifero() const
  {
   std::cout<<"\n\tAu ..! Au..!!\n";
  }
};
    
void emitirSom(Mamifero* animais)
{
 /*
  Em C++ não temos a palavra-chave instanceof,
  mas podemos usar a função typeid().
  se o argumento dessa função for uma referência ou um ponteiro 
  referenciado para uma classe polimórfica, 
  ela retornará um type_info correspondendo ao 
  tipo do objeto em tempo de execução.
 */      
 
 if(typeid(*animais) == typeid(Boi))
  {
   Boi *boi = dynamic_cast<Boi *>(animais);
   boi->somMamifero();
  }
 elif(typeid(*animais) == typeid(Gato))
  {
   Gato *gato = dynamic_cast<Gato *>(animais);
   gato->somMamifero();
  }
 elif(typeid(*animais) == typeid(Porco))
  {
   Porco *porco = dynamic_cast<Porco *>(animais);
   porco->somMamifero();
  }
 elif(typeid(*animais) == typeid(Cachorro))
  {
   Cachorro *cachorro = dynamic_cast<Cachorro *>(animais);
   cachorro->somMamifero();
  }   
}

void Mamifero::menu()
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
      emitirSom(mamPtr);  
     break;
        
     case 2:
      mamPtr = new Gato();
      emitirSom(mamPtr);
     break;
        
     case 3:
      mamPtr = new Porco();
      emitirSom(mamPtr);
     break;
        
     case 4:
      mamPtr = new Cachorro();
      emitirSom(mamPtr);
     break;
          
     case 5:
      std::cout<<"\n\tGood Bye ...!!!\n\n";
     break;
        
     default:
      std::cout<<"\n\tOpção Inválida ..!!!\n";
    }
  }
}
    
int main()
{
 Mamifero *m = new Mamifero();
 
 (*m).menu();
}