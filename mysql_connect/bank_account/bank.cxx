#include <sstream>
#include <iomanip>     
#include <cstdlib>
#include <iostream>
#include <mysql/mysql.h>
//https://www.codeguru.com/cpp/data/database-programming-with-cc.html
#include "bank_account.hxx"
#include "bank_transaction.hxx"

enum { Print=1, New, WithDraw, Deposit, Close, End };

int mainMenu()
{
 std::cout << "\n\tMenu Options"
           << "\n\t1 - Print All Account"
           << "\n\t2 - Open New Account"
           << "\n\t3 - Withdraw"
           << "\n\t4 - Deposit"
           << "\n\t5 - Close Account"
           << "\n\t6 - End Transaction"
           << "\n\tChoice: "; 
 int ch;
 std::cin >> ch;
 return ch;
}

int main(int argc, char** argv)
{
 int acno;
 int choice;
 double bal;
 std::string fname;
 std::string lname;
 
 BankTransaction* bt = new BankTransaction("localhost", "root", "passwd123", "mybank");
 
 while(1)
  {
   choice = mainMenu();
   
    switch(choice)
     {
      case Print:
       bt->printAllAccounts();
      break;
      
      case New:
       std::cout << "\n\tEnter account no, first name,last name, balance: ";
       
       std::cin >> acno >> fname >> lname >> bal;
       
             if(acno < 1)
             {
              std::cout << "\n\tInvalid account number.\n";
                break;
             }
             bt->createAccount(new BankAccount(acno, fname, lname, bal));
             break;
      
      case WithDraw:
             std::cout << "\n\tEnter account no, amount to withdraw ";
             std::cin >> acno >> bal;
                  
             if(bal < 0)
              {
               std::cout << "\n\tInvalid amount.\n";
                break;
              }
              
             bt->withdraw(acno, bal);
             break;
                  
      case Deposit:
             std::cout << "\n\tEnter account no, amount to deposit? ";
             std::cin >> acno >> bal;
             if(bal < 0)
              {
               std::cout << "\n\tInvalid amount.\n";
               break;
              }
             bt->deposit(acno, bal);
             break;
      case Close:
             std::cout << "\n\tEnter account no to close account? ";
             std::cin >> acno;
             bt->closeAccount(acno);
             break;
           
      case End:
            std::cout<<"\n\tGood Bye.!!!\n";
            return 0;    
            break;
          
          default:
            std::cerr << "\n\tInvalid choice!\n";
            break;
     }
  }
 return 0;
}