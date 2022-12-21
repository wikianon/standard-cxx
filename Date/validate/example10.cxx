#include <iostream>

constexpr unsigned int str2int(const char* str, int h = 0)
{
 // not my code but can't remember where I got it from
 return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}

std::string current_date(const std::string& format)
{
 tm dtm;
 char temp[32] = "";
 time_t timer = time(NULL);
  
 switch(str2int(format.c_str()))
  {
   case str2int("yyyy-mm-dd"):
    strftime(temp, sizeof(temp) - 1, "%Y-%m-%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd-mm-yyyy"):
    strftime(temp, sizeof(temp) - 1, "%d-%m-%Y", localtime_r(&timer, &dtm));
   break;
   
   case str2int("yy-mm-dd"):
    strftime(temp, sizeof(temp) - 1, "%y-%m-%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd-mm-yy"):
    strftime(temp, sizeof(temp) - 1, "%d-%m-%y", localtime_r(&timer, &dtm));
   break;
      
   case str2int("yyyy.mm.dd"):
    strftime(temp, sizeof(temp) - 1, "%Y.%m.%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd.mm.yyyy"):
    strftime(temp, sizeof(temp) - 1, "%d.%m.%Y", localtime_r(&timer, &dtm));
   break;
   
   case str2int("yy.mm.dd"):
    strftime(temp, sizeof(temp) - 1, "%y.%m.%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd.mm.yy"):
    strftime(temp, sizeof(temp) - 1, "%d.%m.%y", localtime_r(&timer, &dtm));
   break;   
   
   case str2int("yyyy/mm/dd"):
    strftime(temp, sizeof(temp) - 1, "%Y/%m/%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd/mm/yyyy"):
    strftime(temp, sizeof(temp) - 1, "%d/%m/%Y", localtime_r(&timer, &dtm));
   break;
   
   case str2int("yy/mm/dd"):
    strftime(temp, sizeof(temp) - 1, "%y/%m/%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("dd/mm/yy"):
    strftime(temp, sizeof(temp) - 1, "%d/%m/%y", localtime_r(&timer, &dtm));
   break;
   
   case str2int("yyyymmdd"):
    strftime(temp, sizeof(temp) - 1, "%Y%m%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("ddmmyyyy"):
    strftime(temp, sizeof(temp) - 1, "%d%m%Y", localtime_r(&timer, &dtm));
   break;
   
   case str2int("yymmdd"):
    strftime(temp, sizeof(temp) - 1, "%y%m%d", localtime_r(&timer, &dtm));
   break;
   
   case str2int("ddmmyy"):
    strftime(temp, sizeof(temp) - 1, "%d%m%y", localtime_r(&timer, &dtm));
   break;
   
   default:
    std::cout << "\n\tInvalid!\n\n";
   break;
  }
  
 return temp;
}

std::string current_time(const std::string& format)
{
 tm dtm;
 char temp[32] = "";
 time_t timer = time(NULL);
   
 switch(str2int(format.c_str()))
  {
   case str2int("HH:MM:SS"):
    strftime(temp, sizeof(temp) - 1, "%H:%M:%S", localtime_r(&timer, &dtm));
   break;
   
   case str2int("HH:MM"):
    strftime(temp, sizeof(temp) - 1, "%H:%M", localtime_r(&timer, &dtm));
   break;
   
   case str2int("HHMMSS"):
    strftime(temp, sizeof(temp) - 1, "%H%M%S", localtime_r(&timer, &dtm));
   break;
   
   case str2int("HHMM"):
    strftime(temp, sizeof(temp) - 1, "%H%M", localtime_r(&timer, &dtm));
   break;
   
   default:
    std::cout << "\n\tInvalid!\n\n";
   break;
  }
  
 return temp;
}

std::string exp_date(int days, int hours, int minutes, int seconds)
{
 tm dtm;
 char date[256] = "";
 time_t t = time(NULL) + seconds + 60 * (minutes + 60 * (hours + 24 * days));
  
 strftime(date, sizeof(date), "%A, %d-%b-%Y %H:%M:%S GMT", gmtime_r(&t, &dtm));
 
 return date;
} 

int main()
{
 std::cout<<"\n\t"<<current_date("yyyy-mm-dd")<<" "<<current_time("HH:MM:SS")<<"\n\t"<<exp_date(34,12,35,59)<<"\n\n";
 
 return 0;
}