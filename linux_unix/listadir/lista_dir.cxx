#include <iostream>
#include <dirent.h>
/* ListarDiretorios.cpp:
   - Listar diretorios
   - Criar recursão para acessar subpastas
   - Procurar arquivos via extensão
   - Adicionar arquivos em uma coleção
   - Estruturar o programa em classes
*/
#if defined(__WIN32__) || defined(__WINDOWS__)

//Windows definition
#define IS_FILE 32768
#define IS_DIR 16384

const char *DIRECTORY = "c:\\";

#elif defined(__linux__) || defined(__gnu_linux__)

//Linux definition
#define IS_DIR 0x4
#define IS_FILE 0x8

const char *DIRECTORY = "/home/";

#endif

struct dirent *directory = 0;

DIR *_DIR;

//prototipos
int countFiles(const char *);
inline bool existsFiles(const char *);
void listDir(const char *);
inline bool dirExist(const char *);

int main()
{
 std::cout << std::endl
           << countFiles(DIRECTORY)
           << std::endl
           << dirExist(DIRECTORY)
           << std::endl;
    
 listDir(DIRECTORY);
 return 0;
}

inline bool dirExist(const char *dir)
{
 return ((::_DIR = opendir(dir)) != NULL);
}

void listDir(const char *dir)
{
 ::_DIR = opendir(dir);
 
 while ((directory = readdir(_DIR)) != NULL)
  {
   if(directory->d_type == IS_DIR)
   std::cout << DIRECTORY
             << directory->d_name
             << std::endl;
  };
 
 directory = 0;
 closedir(::_DIR);
 std::cout << std::endl;
}

inline bool existsFiles(const char *dir)
{
 ::_DIR = opendir(dir);
 
 while ((directory = readdir(_DIR)) != NULL)// exists file
  {
   if(directory->d_type == IS_FILE)
    {
     directory = 0;
     closedir(::_DIR);
     return 1;
    }
  };

  directory = 0;
  closedir(::_DIR);
  return 0;
}

int countFiles(const char *dir)
{
 int count = 0;
 ::_DIR = opendir(dir);
 
 while ((directory = readdir(_DIR)) != NULL)
 {
  if (directory->d_type == IS_DIR)
  count++;
 };

 directory = 0;
 closedir(::_DIR);
 return count;
}