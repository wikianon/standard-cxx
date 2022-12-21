#include <cctype>
#include <fstream>
#include <iostream>

static std::string getFileType(const char *name)
{
 std::string filename = name;
 int pos = filename.rfind('.');
 
 if(pos != std::string::npos)
  {
   std::string suffix = filename.substr(pos);
   std::string result;
     
   for(std::size_t i=0; i < suffix.size(); i++)
    {
     result += (char)tolower(suffix[i]);
    }
   return result;
  }
  else
 return std::string("");
}

std::string extractExtension(std::string file_name)
{
 std::string::size_type pos = file_name.find_last_of('.');
 return (pos == std::string::npos)? file_name : file_name.substr(pos);
}

std::string fileSize1(const char* file)
{
 std::ifstream input(file, std::ios::in | std::ios::binary);
 
 long begin = input.tellg();
 input.seekg(0, std::ios::end);
 
 long end = input.tellg();
 input.close();
 
 return std::to_string(end-begin);
}

std::streampos fileSize2(const char* filePath)
{
 std::ifstream file(filePath, std::ios::binary);
 
 std::streampos fsize = file.tellg();
 file.seekg(0, std::ios::end);
 fsize = (file.tellg() - fsize);
 file.close();
 
 return fsize;
}

std::ifstream::pos_type fileSize3(const char* filename)
{
 std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
 return in.tellg(); 
}

int fileSize4(std::string filename) // path to file
{
 FILE *p_file = NULL;
 
 p_file = fopen(filename.c_str(),"rb");
 fseek(p_file,0,SEEK_END);
 
 int size = ftell(p_file);
 fclose(p_file);
 
 return size;
}

long fileSize5(FILE *arquivo)
{
 // guarda o estado ante de chamar a função fseek
 long posicaoAtual = ftell(arquivo);
 
 // guarda tamanho do arquivo
 long tamanho;
 
 // calcula o tamanho
 fseek(arquivo, 0, SEEK_END);
 tamanho = ftell(arquivo);
 
 // recupera o estado antigo do arquivo
 fseek(arquivo, posicaoAtual, SEEK_SET);
 
 return tamanho;
}

int main()
{
  std::cout<<"\n\tFile extensions\n\t"
           <<extractExtension("input.txt")
           <<"\n\t"
           <<getFileType("input.txt")
           <<"\n\tFileSize:\n\t"
           <<fileSize1("filetype.cxx")<<" bytes\n\t"
           <<fileSize2("filetype.cxx")<<" bytes\n\t"
           <<fileSize3("filetype.cxx")<<" bytes\n\t"
           <<fileSize4("filetype.cxx")<<" bytes\n\n";
  return 0;
}
/*
 Output:
 File extension: .txt
 
 */
