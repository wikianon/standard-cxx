#include <fstream>
#include <sstream>
#include <iostream>
//http://www.dreamincode.net/forums/topic/39880-various-problems-validation-file-editing/


struct Contact{
 std::string name;
 std::string address;
 std::string HomeNumber;
 std::string WorkNumber;
 std::string MobileNumber;
 std::string email;
 std::string dobYear;
 std::string dobMonth;
 std::string dobDay;
 std::string age;
};

const int MAX = 101;
Contact listContact[MAX];
int ContactCounter;

int checkEmail(Contact c)
{
 c.email.find("@");
 if (c.email.find("@")>c.email.length())
  return 0;
 else return 1;
}

/*
bool checkNumber(std::string &s) {
   std::istringstream ssIn(s);
   int n;
   if (ssIn >> n) {
   std::cout << n << "\n";
   // we got a number, feed it back
   std::stringstream ssOut;
   ssOut << n;
   s = ssOut.str();   
   return true;
   } else { 
   s.clear();
   }
   return false;
}

void loadNumber(std::string &s, const std::string &label, const int requiredSize)
{
 while(true)
  {
   std::cout << endl << "Enter " << label << "(" << requiredSize << " digits): ";
   getline (std::cin, s);
   
   if(checkNumber(s))
    { 
     if(s.length()==requiredSize) { break; }
    }
    std::cout << "Invalid " << label << ".\n";
  }
}

void loadString(std::string &s, const std::string &label)
{
 while(true)
  {
   std::cout << endl << "Enter " << label << ": ";
   getline (std::cin, s);
   
   if(s != "")break;
   std::cout << "Invalid " << label << ".\n";
  }
}


Contact newContact()
{
 Contact c;
 
 loadString(c.name, "Name");
 loadString(c.address, "Address");
 loadString(c.HomeNumber, "Home Number");
 loadString(c.WorkNumber, "Work Number");
 loadString(c.MobileNumber, "Mobile Number");
 loadEmail(c.email, "Email Address");
 loadNumber(c.dobYear, "year of birth", 4);
 loadNumber(c.dobMonth, "month of birth", 2);
 loadNumber(c.dobDay, "day of birth", 2);
 return c;
}
*/

Contact newContact()
{
 Contact c;
 do{
    std::cout <<"\n\tEnter name: ";
    getline(std::cin, c.name);
    
    if(c.name == "")std::cout << "Invalid Name.\n";
   }while(c.name == "");
 
 do{
    std::cout << "\n\tEnter address: ";
    getline(std::cin, c.address);
    
    if(c.address == "")std::cout << "Invalid Address.\n";
   }while (c.address == "");
 
 do{
    std::cout << "\n\tEnter home number: ";
    getline(std::cin, c.HomeNumber);
    
    if(c.HomeNumber == "")std::cout << "Invalid Number.\n";
   }while (c.HomeNumber == "");
 
 do{
    std::cout << "\n\tEnter work number: ";
    getline(std::cin, c.WorkNumber);
    
    if(c.WorkNumber == "")std::cout << "Invalid Number.\n";
   }while (c.WorkNumber == "");
 
 do{
    std::cout << "\n\tEnter mobile number: ";
    getline(std::cin, c.MobileNumber );
    
    if(c.MobileNumber == "")
    std::cout << "Invalid Number.\n";
   }while(c.MobileNumber == "");
 
 do{
    std::cout << "\n\tEnter email: ";
    getline(std::cin, c.email);
    
    if(checkEmail(c)==0)std::cout << "Invalid Email Address.\n";
   }while(checkEmail(c) == 0);
 
 do{
    std::cout << "\n\tEnter year of birth(4 digits): ";
    getline(std::cin, c.dobYear);
   }while(c.dobYear == "");
 
 do{
    std::cout << "\n\tEnter month of birth(2 digits): ";
    getline(std::cin, c.dobMonth);
   }while(c.dobMonth == "");
 
 do{
    std::cout << "\n\tEnter day of birth(2 digits): ";
    getline(std::cin, c.dobDay);
   }while(c.dobDay == "");

  return c;
}

void writeContacts(Contact &c, std::ofstream &os)
{
 os << "\nContact Data..: " << ContactCounter
    << "\nName..........: " << c.name
    << "\nAddress.......: " << c.address
    << "\nHome Number...: " << c.HomeNumber
    << "\nWork Number...: " << c.WorkNumber
    << "\nMobile Number.: " << c.MobileNumber
    << "\nEmail.........: " << c.email
    << "\nDay...........: " << c.dobDay
    << "\nMont..........: " << c.dobMonth
    << "\nYear..........: " << c.dobYear;
}

void readContacts(Contact &c, std::ifstream &is)
{
 getline(is, c.name);
 getline(is, c.address);
 getline(is, c.HomeNumber);
 getline(is, c.WorkNumber);
 getline(is, c.MobileNumber);
 getline(is, c.email);
 getline(is, c.dobYear);
 getline(is, c.dobMonth);
 getline(is, c.dobDay);
}

void addContact()
{
 std::cout << "\n\tInput new contact data: ";
 
 listContact[ContactCounter] = newContact();
 ContactCounter++;
}

void display(Contact &c)
{
 std::cout << "\n\tContact Data...: " << ContactCounter
           << "\n\tName...........: " << c.name
           << "\n\tAddress........: " << c.address
           << "\n\tHome Number....: " << c.HomeNumber
           << "\n\tWork Number....: " << c.WorkNumber
           << "\n\tMobile Number..: " << c.MobileNumber
           << "\n\tEmail..........: " << c.email
           << "\n\tDate of Birth..: " << c.dobDay <<"/"<< c.dobMonth <<"/"<< c.dobYear
           << "\n";
}

void displayContact()
{
 int c;
 do{
    std::cout << "\n\tEnter number of contact: ";
    
    std::cin >> c;
    
   }while(c<0 || c>=ContactCounter);
   
 display(listContact[c]);
}

void displayAll()
{
 for(int i=0; i<ContactCounter; i++)
 display(listContact[i]);
}

/*void editContact()
{
 int c;
 do{
    std::cout << "Enter number of contact: ";
    std::cin >> c;
   }while(c<0 || c>=ContactCounter);
 newContact();
}*/

void readFile()
{
 try{
     std::ifstream inFile("Cadastros.txt", std::ios::in);
     
     inFile >> ContactCounter;
     inFile.ignore();
     
     for(int i=0; i<ContactCounter; i++)
     readContacts(listContact[i], inFile);
     //display(listContact[i]);
     
     inFile.close();
     std::cout << "\n\tContact List Loaded.\n";
    }
    catch(std::exception e)
    {
     std::cout << "\n\t<-|Error|-> Could not read file.\n";
    }
}

void writeFile()
{
 try{
     std::ofstream outFile("Cadastros.txt", std::ios::app);
     if(outFile.is_open())
      {
       for(int i=0; i<ContactCounter; i++)
       writeContacts(listContact[i], outFile);
       outFile<<"\n########################################################################################################";
      }
      outFile.close();
     std::cout << "\n\tContact List Saved.\n";
    } 
    catch(std::exception e)
    {
     std::cout << "\n\t<-|Error|-> Could not read file.\n";
    }
}

void doMenu()
{
   int choice;
   int ContactNumber;
   do{
      std::cout << "\n\tEnter your selection."
                   "\n\t1. Load Contacts List"
                   "\n\t2. Save Contacts List"
                   "\n\t3. Add a Contact"
                   "\n\t4. Display Contact(s) Details"
                   "\n\t5. Display All Contact(s) Details"
                   "\n\t6. Quit"
                   "\n\tEnter: ";
      //std::cout << "6. Edit Contact(s) Details" << endl;
   std::cin >> choice;
   std::cin.ignore();
   switch(choice)
   {
   case 1: readFile();
    break;
   case 2: writeFile();
    break;
   case 3: addContact();
    break;
   case 4: displayContact(); 
    break;
   case 5: displayAll();
    break;
   /*case 6: editContact();
    break;*/
   }
   } while(choice != 6);
}

int main()
{
 readFile();
 doMenu();
}