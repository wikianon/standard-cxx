/* TermPaper Header */
#ifndef _SRCMAIN_CXX
#define _SRCMAIN_CXX

#include <termpaper>  /* forma correta de compilar OK */
//#include <TermPaper/src/termpaper.cxx> /* erro ao incluir direto OK */
//#include <TermPaper/include/termpaper.hxx> /* erro ao incluir direto OK */

int main()
{
 TermPaper *trmppr = new TermPaper();
  trmppr->GetDetails();
  trmppr->PrintDetails();
  delete trmppr;
 return 0;
} 

#endif
