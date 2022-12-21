#include <iostream>

//Structs foward declaration.
struct Boi;
struct Gato;
struct Porco;
struct Cachorro;

struct AnimalVisitor 
{
 AnimalVisitor(){}
 ~AnimalVisitor(){}
 virtual void visit(Boi*) = 0;
 virtual void visit(Gato*) = 0;
 virtual void visit(Porco*) = 0;
 virtual void visit(Cachorro*) = 0;
};

struct Animal 
{
 Animal(){}
 ~Animal(){}
 virtual void accept(AnimalVisitor*) = 0;
};

struct EmissorDeSom: AnimalVisitor 
{
 EmissorDeSom(){}
 ~EmissorDeSom(){}
  
  void visit(Boi *boi)
   {
    std::cout<<"\n\tMuu ..! Muu..!!\n";
   }
    
  void visit(Gato *gato)
   {
    std::cout<<"\n\tMiAu ..! MiAu..!!\n";
   }
    
  void visit(Porco *porco)
   {
    std::cout<<"\n\tOinc ..! Oinc..!!\n";
   }
    
  void visit(Cachorro *cachorro)
   {
    std::cout<<"\n\tAu ..! Au..!!\n";
   }
   
  void menu();
};

struct Boi: Animal
{
 Boi(){}
 ~Boi(){}
  
  void accept(AnimalVisitor *visitor)
  {
   visitor->visit(this);
  }
};
    
struct Gato: Animal 
{
 Gato(){}
 ~Gato(){}
 
 void accept(AnimalVisitor *visitor)
 {
  visitor->visit(this);
 }
};

struct Porco: Animal
{
 Porco(){}
 ~Porco(){}
 
 void accept(AnimalVisitor *visitor)
 {
  visitor->visit(this);
 }
};

struct Cachorro: Animal 
{
 Cachorro(){}
 ~Cachorro(){}
 
 void accept(AnimalVisitor *visitor)
 {
  visitor->visit(this);
 }
};

void emitirSom(Animal *animal)
{
 AnimalVisitor *visitor = new EmissorDeSom();
 animal->accept(visitor);
}

void EmissorDeSom::menu()
{ 
 Animal *an;
 int op;
 
 while(op != 5)
  {
        std::cout<<"\n\t(1) Boi"
                 <<"\n\t(2) Gato"
                 <<"\n\t(3) Porco"
                 <<"\n\t(4) Cachorro"
                 <<"\n\t(5) Sair"
                 <<"\n\tDigite: ";
         std::cin>>op;
   switch(op)
    {
     case 1:
      an = new Boi();
      emitirSom(an);
     break;
         
     case 2:
      an = new Gato();
      emitirSom(an);
     break;
         
     case 3:
      an = new Porco();
      emitirSom(an);
     break;
         
     case 4:
      an = new Cachorro();
      emitirSom(an);
     break;
         
     case 5:
      std::cout<<"\n\tGood Bye ..!!!\n\n";
     break;
         
     default:
      std::cout<<"\n\tOpção Inválida ..!!!\n";
     break;
    }
  }
}

int main()
{
 EmissorDeSom m = *new EmissorDeSom();
  m.menu();
 return 0;
}