// g++ -o shell shell.cpp -O3 -Wall -Wpedantic -std=c++11
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

using namespace std;

std::vector<std::string> split_string(const string& input, const char delimiter) 
{
 std::stringstream ss {input};
 std::vector<std::string> result;
    
 for(std::string buffer; getline(ss, buffer, delimiter);) 
  {
   result.push_back(move(buffer));
  }
 return result;
}

int launch_execute(const std::vector<std::string>& command)
{
 // handle builtins
 if(command.size() == 0) return 1;
 else
 if(command[0] == "exit") return 0;
 else
 if(command[0] == "cd")
  {
   if(command.size() == 1 || command.size() > 2)
    {
     std::cerr << "shell: expected 1 argument to \"cd\"\n";
    }
    else
    {
     if(chdir(command[1].c_str()) != 0)
      {
       perror("shell");
      }
    }   
   return 1;
  }  
  
  // cast vector<string> to const char**
  const char **argv = new const char* [command.size()+1];
    
  for(unsigned j = 0;  j < command.size();  ++j)
   {
    argv [j] = command[j].c_str();
   }
    
  argv[command.size()] = NULL;
    
  // fork
  int status = 0;
  pid_t pid = fork();
  if(pid == 0)
   {
    // Child process
    if(execvp(argv[0], (char **)argv) == -1)
     {
      perror("shell");
     }
    delete[] argv;
    exit(EXIT_FAILURE);  
   }
   else 
   if(pid < 0)
    {
     // Error forking
     perror("shell");   
    }
    else 
    {
     // Parent process
     do{
        waitpid(pid, &status, WUNTRACED);
       }while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
  delete[] argv;
 return 1;
}

int main()
{
 int status = 0;
 std::string line = "";
    
 do{
    std::cout << "shell> ";
    getline(cin, line);
    std::vector<std::string> args = split_string(line, ' ');
        
    status = launch_execute(args);
        
   }while (status);
   
 return 0;
}