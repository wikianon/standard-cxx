/* Termpaper Code */
#ifndef _TERMPAPER_CXX
#define _TERMPAPER_CXX

#define this (*this)

#include <TermPaper/include/termpaper.hxx>

TermPaper::TermPaper( )
{
  fName = "";
  lName = "";
  subject = "";
  letterGrade = 'F';
}

std::string  TermPaper::getFName( )
{
  return this.fName;
}

std::string TermPaper::getLName( )
{
  return this.lName;
}

std::string TermPaper::getSubject( )
{
  return this.subject;
}

char TermPaper::getLetterGrade( )
{
  return this.letterGrade;
} 

void TermPaper::setFName(std::string fN)
{
   this.fName = fN;
}

void TermPaper::setLName(std::string lN)
{
  this.lName = lN;
}

void TermPaper::setSubject(std::string sub)
{
  this.subject = sub;
}

void TermPaper::setLetterGrade(char grade)
{
  this.letterGrade = grade;
}


void TermPaper::GetDetails()
{
  std::string firstName;
  std::string lastName;
  std::string subject;
  char grade;
  
  std::cout << "\n\n\t  Please Enter\n\n\tFirst Name: ";
  std::cin >> firstName;
  setFName(firstName);
  
  std::cout << "\n\tLast Name: ";
  std::cin >> lastName;
  setLName(lastName);
  
  std::cout << "\n\tSubject: ";
  std::cin >> subject;
  setSubject(subject);
  
  std::cout << "\n\tLetter Grade: ";
  std::cin >> grade;
  setLetterGrade(grade);
}
/*
void TermPaper::GetDetails()
{
 TermPaper *tmp = new TermPaper();
 
  std::cout << "\n\n\t  Please Enter\n\n\tFirst Name: ";
  std::cin >> tmp->fName;
  setFName(tmp->fName);
  
  std::cout << "\n\tLast Name: ";
  std::cin >> tmp->lName;
  setLName(tmp->lName);
  
  std::cout << "\n\tSubject: ";
  std::cin >> tmp->subject;
  setSubject(tmp->subject);
  
  std::cout << "\n\tLetter Grade: ";
  std::cin >> tmp->letterGrade;
  setLetterGrade(tmp->letterGrade);
}

void TermPaper::GetDetails()
{
 TermPaper tmp;
 
  std::cout << "\n\n\t  Please Enter\n\n\tFirst Name: ";
  std::cin >> tmp.fName;
  setFName(tmp.fName);
  
  std::cout << "\n\tLast Name: ";
  std::cin >> tmp.lName;
  setLName(tmp.lName);
  
  std::cout << "\n\tSubject: ";
  std::cin >> tmp.subject;
  setSubject(tmp.subject);
  
  std::cout << "\n\tLetter Grade: ";
  std::cin >> tmp.letterGrade;
  setLetterGrade(tmp.letterGrade);
}
*/

void TermPaper::PrintDetails()
{
  std::cout << "\n\n\tFirst Name..: " <<getFName();
  std::cout << "\n\tLast Name...: " <<getLName();
  std::cout << "\n\tSubject.....: " <<getSubject();
  std::cout << "\n\tGrade.......: " <<getLetterGrade()<<"\n\n";
}


#endif