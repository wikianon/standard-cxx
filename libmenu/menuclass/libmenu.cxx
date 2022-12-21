#include "libmenu.hxx"

#define MAX 15
/*!
 *
 * @begin @section terms_of_use Terms of Use
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * @end @section terms_of_use Terms of Use
 * 
 * @begin @section author Author
 * 
 * @file       libmenu.cxx
 * @version    0.1
 * @brief      menu const char* e std::string*
 * @author     Jean Zonta
 * @Copyright (C) 2013-2017 Jean Zonta.
 * 
 * @end @section author Author
 *
*/
menu::menu(int opmenu, const char* opcoes[])
{
 if(opmenu > MAX)
 {
  std::cout<<"OPTION LIMITS MAXIMUM SIZE: "<<MAX<<" EXCEEDED OPTION UNSUPORTED: "<<opmenu<<"\n";
  exit(1);
 }
 else
 for(int i = 0; i<opmenu; i++)
 std::cout<<"\n\t["<<i+1<<"]-"<<opcoes[i];
 std::cout<<"\n\tCHOICE: ";
}

menu::menu(int opmenu, int maxlimits, const char* opcoes[])
{
 if(opmenu != maxlimits)
 {
  std::cout<<"OPTION LIMITS: "<<opmenu<<" DOES NOT EQUAL TO MAX: "<<maxlimits<<"\n";
  exit(1);
 }
 else
 for(int i = 0; i<opmenu; i++)
 std::cout<<"\n\t["<<i+1<<"]-"<<opcoes[i];
 std::cout<<"\n\tCHOICE: ";
}

menu::menu(int opmenu, std::string* opcoes)
{
 if(opmenu > MAX)
 {
  std::cout<<"OPTION LIMITS MAXIMUM SIZE: "<<MAX<<" EXCEEDED OPTION UNSUPORTED: "<<opmenu<<"\n";
  exit(1);
 }
 else
 for(int i = 0; i<opmenu; i++)
 std::cout<<"\n\t["<<i+1<<"]-"<<opcoes[i];
 std::cout<<"\n\tCHOICE: ";
}

menu::menu(int opmenu, int maxlimits, std::string* opcoes)
{
 if(opmenu != maxlimits)
 {
  std::cout<<"OPTION LIMITS: "<<opmenu<<" DOES NOT EQUAL TO MAX: "<<maxlimits<<"\n";
  exit(1);
 }
 else
 for(int i = 0; i<opmenu; i++)
 std::cout<<"\n\t["<<i+1<<"]-"<<opcoes[i];
 std::cout<<"\n\tCHOICE: ";
}