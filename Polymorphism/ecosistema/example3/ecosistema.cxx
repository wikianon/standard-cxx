#include <vector>
#include <iostream>

class mamifero
{
 public: //Class is private by default then it is necessary to use public so that other classes or structs have access to the members
  mamifero(){}
  ~mamifero(){}
  virtual mamifero *reproduz() = 0; // Pure virtual constructor (and abstract, to enforce reimplementation in each derived class).
  virtual std::string nome() = 0;
  virtual std::string som() = 0;
};

//Class is private by default so it is necessary to use public
class cachorro: public mamifero
{
 public:
  cachorro(){}
  ~cachorro(){}
  cachorro *reproduz() override{ return new cachorro; }
  std::string nome() override{ return "cachorro"; }
  std::string som() override{ return "latido"; }
};

class gato: public mamifero
{
 public:
  gato(){}
  ~gato(){}
  gato *reproduz() override{ return new gato; }
  std::string nome() override{ return "gato"; }
  std::string som() override{ return "miado"; }
};

class homem: public mamifero
{
 public:
  homem(){}
  ~homem(){}
  homem *reproduz() override{ return new homem; }
  std::string nome() override{ return "homem"; }
  std::string som() override{ return "fala"; }
};

class cavalo: public mamifero
{
 public:
  cavalo(){}
  ~cavalo(){}
  cavalo *reproduz() override{ return new cavalo; }
  std::string nome() override{ return "cavalo"; }
  std::string som() override{ return "relincho"; }
};

class cabra: public mamifero
{
 public:
  cabra(){}
  ~cabra(){}
  cabra *reproduz() override{ return new cabra; }
  std::string nome() override{ return "cabra"; }
  std::string som() override{ return "berro"; }
};

class leao: public mamifero
{
 public:
  leao(){}
  ~leao(){}
  leao *reproduz() override { return new leao; }
  std::string nome() override{ return "leao"; }
  std::string som() override{ return "rugido"; }
};

class boi: public mamifero
{
 public:
  boi(){}
  ~boi(){}
  boi *reproduz() override { return new boi; }
  std::string nome() override{ return "boi"; }
  std::string som() override{ return "sturro"; }
};

std::vector<mamifero *> opcoes
{
 new cachorro,  
 new gato,  
 new homem,
 new cavalo,
 new cabra,
 new leao,
 new boi
};

std::vector<mamifero *> ecossistema;

mamifero *menu(void)
{
 char op;
 int n=0;
 std::cout << "\n\tDiga qual animal vai se reproduzir: ";
 for(const auto &animal: opcoes)
 std::cout << "\n\t" << n++ << ": " << animal->nome()<< "";
 std::cout << "\n\t--> ";
 std::cin >> n;
 std::cin.ignore(1, '\n');
 
 if(n >= 0 && n < opcoes.size())return opcoes[n]->reproduz();
 else
 {
  std::cout << "\n\tEcosistema não encontrado.\n\tDeseja continuar? (s)im ou n(ão): ";
  std::cin >> op;
  if(op == 's' || op == 'S')
  return menu();
  else
  std::cout << "\n\tGoodbye!\n\n";
  return 0;
 }
}

mamifero *getDetails(void)
{
 std::string resposta;
 
 do{
    std::cout << "\n\tO ecossistema tem " << ecossistema.size() << " mamífero(s).\n";
    if(ecossistema.size() >= 0)
    for(const auto &animal: ecossistema)
    std::cout << "\tUm " << animal->nome() << ", que emite " << animal->som() << ".\n";   
    std::cout << "\n\tQuer inserir um novo mamífero no ecossistema?\n\tDigite [s]-Sim ou [n]-Não: ";
    getline(std::cin, resposta);
    std::cout << std::endl;
    
    if(resposta == "s" || resposta == "S")
    ecossistema.push_back(menu());
    else
    if(resposta == "n" || resposta == "N")break;
    else
    if(resposta != "s" || resposta != "S")
    std::cout<<"\n\tOpção Inválida\n";  
   
   }while(resposta != "n" || resposta != "N");
  
   if(ecossistema.size() > 0)
    {
     std::cout << "\n\tAo final, o ecossistema tinha " << ecossistema.size() << " mamífero(s).\n";
     for(const auto &animal: ecossistema)
     std::cout << "\tUm " << animal->nome() << ", que emite " << animal->som() << ".\n";
     std::cout << "\n\tGoodbye!\n\n";
    }
   else
   std::cout << "\n\tAo final, o ecossistema tem " << ecossistema.size() << " mamífero.\n\n";
 return 0;
}

int main()
{
 getDetails();
 return 0;
}