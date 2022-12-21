#include <fstream>
#include <sstream>
#include <iostream>
#include "date.hxx"

//http://www.dreamincode.net/forums/topic/39499-2-parts-of-attention-required/
using namespace std;

struct Contact{
	string name;
	string address;
	string HomeNumber;
	string WorkNumber;
	string MobileNumber;
	string email;
	string dobYear;
	string dobMonth;
	string dobDay;
	int age;
};

const int MAX = 101;
Contact listContact[MAX];
int ContactCounter;

int checkEmail (Contact c)
{
	c.email.find("@");
	if (c.email.find("@")>c.email.length())
		return 0;
	else return 1;
}

Contact newContact()
{
	Contact c;
	do {
		cout << endl << "Enter name : ";
	getline (cin, c.name);
	if (c.name == "")
		cout << "Invalid Name." << endl;
	else cout << endl;
	} while(c.name == "");
	
	do {
		cout << endl << "Enter address : ";
	getline (cin, c.address );
	if (c.address == "")
		cout << "Invalid Address." << endl;
	else cout << endl;
	} while (c.address == "");
	
	do {
		cout << endl << "Enter home number : ";
	getline (cin, c.HomeNumber );
	if (c.HomeNumber == "")
		cout << "Invalid Number." << endl;
	else cout << endl;
	} while (c.HomeNumber == "");
	
	do {
		cout << endl << "Enter work number : ";
	getline (cin, c.WorkNumber );
	if (c.WorkNumber == "")
		cout << "Invalid Number." << endl;
	else cout << endl;
	} while (c.WorkNumber == "");
	
	do {
		cout << endl << "Enter mobile number : ";
	getline (cin, c.MobileNumber );
	if (c.MobileNumber == "")
		cout << "Invalid Number." << endl;
	else cout << endl;
	} while(c.MobileNumber == "");
	
	do {
		cout << endl << "Enter email : ";
	getline (cin, c.email );
	if (checkEmail (c)==0)
		cout << "Invalid Email Address." << endl;
	else cout << endl;
	} while (checkEmail (c) ==0);
	
	do {
		cout << endl << "Enter year of birth(4 digits) : ";
	getline (cin, c.dobYear );
	} while (c.dobYear == "");
	
	do {
		cout << endl << "Enter month of birth(2 digits) : ";
	getline (cin, c.dobMonth);
	if (c.name == "")
		cout << "Invalid name." << endl;
	else cout << endl;
	} while(c.dobMonth == "");
	
	do {
		cout << endl << "Enter day of birth(2 digits) : ";
	getline (cin, c.dobDay);
	} while (c.dobDay == "");

	int numdobDay;
	stringstream dobDaystream(c.dobDay);
	dobDaystream >> numdobDay;
	
	int numdobMonth;
	stringstream dobMonthstream(c.dobMonth);
	dobMonthstream >> numdobMonth;

	int numdobYear;
	stringstream dobYearstream(c.dobYear);
	dobYearstream >> numdobYear;


	date today;
	cout << today;
	cout << endl << "The year is " << today.year();
	cout << endl << "The month is " << today.month();
	cout << endl << "The day is " << today.day();
	
	
	c.age = today._year - numdobYear;
	if (numdobMonth > today._month)
	--c.age;
	else if (numdobMonth == today._month && numdobDay > today._day )
	--c.age;
	else if (numdobMonth == today._month && numdobDay == today._day )
	cout << "^_^/> Happy Birthday To You ^_^/>";

 	return c;
}

void writeContacts(Contact &c, ofstream &os)
{
	os << c.name << endl 
		<< c.address << endl 
		<< c.HomeNumber << endl 
		<< c.WorkNumber << endl 
		<< c.MobileNumber << endl 
		<< c.email << endl 
		<< c.dobYear << endl 
		<< c.dobMonth << endl 
		<< c.dobDay << endl;
}

void readContacts(Contact &c, ifstream &is)
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
	cout << endl << "Input new contact data: " << endl;
	int ContactCounter = 0;
	listContact[ContactCounter] = newContact();
	ContactCounter++;
}

void display(Contact &c, ifstream &is)
{
	cout << endl << "Contact Data:" << endl
		<< "Name		  - " << c.name << endl
		<< "Address	   - " << c.address << endl
		<< "Home Number   - " << c.HomeNumber << endl
		<< "Work Number   - " << c.WorkNumber << endl
		<< "Mobile Number - " << c.MobileNumber << endl
		<< "Email		 - " << c.email << endl
		<< "Date of Birth - " << c.dobDay <<"/"<< c.dobMonth <<"/"<< c.dobYear << endl;
}

void displayContact(Contact c)
{
		cout << endl
		<< "Name		  - " << c.name << endl 
		<< "Address	   - " << c.address << endl
		<< "Home Number   - " << c.HomeNumber << endl
		<< "Work Number   - " << c.WorkNumber << endl
		<< "Mobile Number - " << c.MobileNumber << endl
		<< "Email		 - " << c.email << endl
		<< "Date of Birth - " << c.dobDay <<"/"<< c.dobMonth <<"/"<< c.dobYear << endl;
}

void readFile()
{
   try{
	  ifstream inFile("cadastro.txt", ios::in);
	  inFile >> ContactCounter;
	  inFile.ignore();
	  for(int i=0; i<ContactCounter; i++)
	   display(listContact[i], inFile);
	  inFile.close();
   }
   catch(exception e) {
	  cout << endl << "<-|Error|->" << endl << "Could not read file." << endl;
   }
}

void writeFile()
{
	try{
		ofstream outFile("cadastro.txt", ios::ate);
		if(outFile.is_open())
		{
		 outFile << ContactCounter << endl;
		 for(int i=0; i<ContactCounter; i++)
		 writeContacts(listContact[i], outFile);
		}
		outFile.close();
	}
	catch(exception e) {
	  cout << endl << "<-|Error|->" << endl << "Could not read file." << endl;
	}
}

void doMenu()
{
	  int choice;
	  int ContactNumber;
	  do{
		 cout << "\nEnter your selection." << endl;
		 cout << "1. Load Contacts List" << endl;
		 cout << "2. Save Contacts List" << endl;
		 cout << "3. Add a Contact" << endl;
		 cout << "4. Display Contact(s) Details" << endl;
		 cout << "5. Quit" << endl;
		 cin >> choice;
		 cin.ignore();
		 switch(choice)
		 {
		 case 1: readFile();
			 break;
		 case 2: writeFile();
			 break;
		 case 3: newContact();
			 break;
		 /*case 4: cout << "Enter the number of the contact : " << endl;
			 cin << ContactNumber;
			 for (int i; i < MAX; i++)
				displayContact (ContactNumber)
				 else cout << "Contact Not Found." <<endl;*/
		 
		 case 4: 
		   cout << "Enter the number of the contact : ";
                   cin >> ContactNumber;
                   if(ContactCounter > ContactNumber >= 0)
	           displayContact(listContact[ContactNumber]);
                   else
		   cout << "Contact Not Found.\n";
		  break;
		 }
		 
	  } while(choice != 5);
}

int main()
{
	readFile();
	doMenu();
	writeFile();
}