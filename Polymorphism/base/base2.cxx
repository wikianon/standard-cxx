#include <iostream>

//// LIBRARY START
struct Base
{
 Base(){}
 virtual ~Base(){} // Ensures to invoke actual object destructor
 
 // An interface Polymorphism Dinamic
 virtual void DisplayAction() = 0;
};

struct Derived1 : Base
{
 Derived1()
 {
  std::cout << "Derived1 created\n";
 }

 ~Derived1()
  {
   std::cout << "Derived1 destroyed\n";
  }

 void DisplayAction()
 {
  std::cout << "Action from Derived1\n";
 }
};

struct Derived2 : Base
{
 Derived2()
 {
  std::cout << "Derived2 created\n";
 }

 ~Derived2()
  {
   std::cout << "Derived2 destroyed\n";
  }

 void DisplayAction()
 {
  std::cout << "Action from Derived2\n";
 }
};

//// LIBRARY END

struct User
{
 // Creates Drived1
 User() : pBase(0)
 {
  // What if Derived2 is required? - Add an if-else ladder (see next sample)
  pBase = new Derived1();
 }

 ~User()
 {
  if( pBase )
  {
   delete pBase;
   pBase = 0;
  }
 }

 // Delegates to actual object
 void Action()
 {
  pBase->DisplayAction();
 }

 private:
 Base *pBase;
};

int main()
{
 User *user = new User();

 // Need Derived1 functionality only
 user->Action();

 delete user;
}