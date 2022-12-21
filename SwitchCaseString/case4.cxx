#include <map>
#include <iostream>

enum StringValue { evStringValue1=1, evStringValue2, evStringValue3, evEnd };

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> mapStringValues;

// Intialization
void initialize()
{
 mapStringValues["First Value"]  = evStringValue1;
 mapStringValues["Second Value"] = evStringValue2;
 mapStringValues["Third Value"]  = evStringValue3;
 mapStringValues["end"] = evEnd;
 
 const std::string strings[4]={"First Value","Second Value","Third Value","end"};
 
 std::cout << "\n\tmapStringValues contain\n\n";
         for(int st = evStringValue1; st<=evEnd; st++)
          {
           StringValue str = static_cast<StringValue>(st);
           std::cout<<"\t"<<strings[str-1]<<"\n";   
          }    
}

int main(int argc, char* argv[])
{
 // User input
 static char szInput[15];
 
 // Inicializa map strings
 initialize();
  
 // Enquanto imput nao for igual a end
 while(1) 
  {
    // Get the user's input
    std::cout << "\n\tPlease enter any of the\n\tstrings above: ";
    std::cout.flush();
    std::cin.getline(szInput, sizeof(szInput));
    
    // Switch on strings
    switch(mapStringValues[szInput])
     {
      case evStringValue1:
            std::cout << "\n\tDetected the first valid string.\n";
      break;
      
      case evStringValue2:
            std::cout << "\n\tDetected the second valid string.\n";
      break;
      
      case evStringValue3:
            std::cout << "\n\tDetected the third valid string.\n";
      break;
      
      case evEnd:
            std::cout << "\n\tDetected program end command. "
                      << "\n\tProgramm will be stopped.\n\n";
      return 0;
      break;
      
      default:
            std::cout << "\n\t\"" << szInput 
                      << "\" is an invalid string."
                      << "\n\tmapStringValues does not\n\tcontain "
                      << mapStringValues.size()
                      << " entries.\n";
            initialize();
      break;
     }
  }
 return 0;
}