#include <map>
#include <iostream>

enum StringValue { evStringValue1=1, evStringValue2, evStringValue3, evEnd };

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> mapStringValues;

// Intialization
void Initialize()
{
 mapStringValues["First Value"]  = evStringValue1;
 mapStringValues["Second Value"] = evStringValue2;
 mapStringValues["Third Value"]  = evStringValue3;
 mapStringValues["end"] = evEnd;
 
 std::cout << "\n\tmapStringValues contain " 
           << mapStringValues.size() 
           << " entries.\n";
}

int main(int argc, char* argv[])
{
 // User input
 static char szInput[15];

 const std::string strings[3]={"First Value","Second Value","Third Value"};
 
 // Inicializa map strings
 Initialize();
  
 // Enquanto imput nao for igual a end
 while(1) 
  {
    // Get the user's input
    std::cout << "\n\tPlease enter a string or enter end: ";
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
                      << "\n\tmapStringValues does not contain "
                      << mapStringValues.size() 
                      << " entries."
                      << "\n\n\tEnter:\n";
         for(int st = evStringValue1; st<evEnd; st++)
          {
           StringValue str = static_cast<StringValue>(st);
           std::cout<<"\t"<<strings[str-1]<<"\n";   
          }
      break;
     }
  }
 return 0;
} 
