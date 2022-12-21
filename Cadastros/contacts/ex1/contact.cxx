#include <vector>
#include <fstream>
#include <iostream>

struct Contact
{
 std::string m_fName;
 std::string m_lName;
 int m_pNumber;

 Contact(const char* firstName, const char* lastName, int phoneNumber): 
 m_fName(firstName),
 m_lName(lastName),
 m_pNumber(phoneNumber){}
};

class ContactList
{
 std::vector<Contact> m_myContacts;
 
 public:
 ContactList(): m_myContacts(){}
 
 void addContact(Contact contact)
 {
  m_myContacts.push_back(contact);
 }
 
 friend std::ostream& operator << (std::ostream& os , const ContactList& list)
 {
  for(size_t i = 0; i < list.m_myContacts.size(); ++i)
   {
    os << list.m_myContacts[i].m_fName 
       << " " << list.m_myContacts[i].m_lName 
       << " : " << list.m_myContacts[i].m_pNumber 
       << std::endl;
   }
  return os;
 }
};

int main(int argc, char* argv[])
{
 std::ifstream streamReader("Contacts.txt", std::ios::in);

 ContactList myContactList;

 if(!streamReader.good())
 {
  std::cout << "Was not able to read the file\n";
  return 0;
 }

 while(!streamReader.eof())
 {
  std::string firstName("");
  std::string lastName("");
  int phoneNumber = -1;
  
  streamReader >> firstName >> lastName >> phoneNumber;

  Contact contact(firstName.c_str(), lastName.c_str(), phoneNumber);

  myContactList.addContact(contact);
 }
 
 streamReader.close();

 //do your stuff

 //prints to console all contacts
 std::cout << myContactList;

 return 0;
}