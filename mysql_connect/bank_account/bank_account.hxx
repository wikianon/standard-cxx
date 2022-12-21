#ifndef BANKACCOUNT_HXX
#define BANKACCOUNT_HXX

#include <string>
#include <cstring>






struct BankAccount
{
 static const int MAX_SIZE = 30;
 BankAccount();    
 ~BankAccount();
 BankAccount(int, std::string, std::string, double);
 
 void setAccountNumber(int);
 void setLastName(std::string);
 void setFirstName(std::string);
 void setBalance(double);
     
 int getAccountNumber() const;
 std::string getFirstName() const;
 std::string getLastName() const;
 double getBalance() const;
 
 private:
  int accountNumber;
  char firstName[MAX_SIZE];
  char lastName[MAX_SIZE];
  double balance;
};








#endif   // BANKACCOUNT_HXX