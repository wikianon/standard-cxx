#include <cstring>
#include <iostream>

/*
 Faremos a sobrecarga dos seguintes operadores:

 +  -> para concatenação de strings;
 [] -> para retornar um caractere em determinado índice na string;
 *  -> para "clonar" a mesma cadeia de caracteres determinado número de vezes.
 
 https://www.vivaolinux.com.br/dica/Sobrecarga-de-Operadores-em-C/
 */

class String
{
        // cadeia de caracteres encapsulada pela classe
        const char* caracteres;

    public:

        //construtor que recebe uma cadeia de caracteres
        String(const char* caracteres)
        {
            this->caracteres = caracteres;
        }

        //construtor que não recebe parâmetros
        String()
        {
            this->caracteres = "";
        }

        //operador de concatenação
        String& operator + (const String& s)
        {
            int tamS1 = strlen(this->caracteres);
            int tamS2 = strlen(s.caracteres);
            char* caracteres = new char[tamS1 + tamS2];
            strcpy(caracteres, this->caracteres);
            strcat(caracteres, s.caracteres);
            String* nova = new String(caracteres);
            return *nova;
        }

        //operador para retornar caracter em determinado índice
        char operator [] (int indice)
        {
            return this->caracteres[indice];
        }

        //operador para 'clonar' a string
        String& operator * (int x)
        {
            int tam = strlen(this->caracteres) * x;
            char* caracteres = new char[tam];

            for(int i = 0; i < x; i++)
            {
                strcat(caracteres, this->caracteres);
            }

            String* nova = new String(caracteres);
            return *nova;

        }

        //método para mostrar os caracteres da string
        void mostra()
        {
            std::cout << this->caracteres;
        }

};

int main()
{
    String s1 = "sobrecarga ";
    String s2 = "de ";
    String s3 = "operador";
    String s4 = s1 + s2 + s3;
    s4.mostra();
    
    //A letra é é a 12º letra do vetor.
    char c = s4[12];

    std::cout << "\n" << c;
    
    s4 = s2 * 10;
    std::cout << "\n";
    // imprime a s2 10 vezes
    s4.mostra();
    std::cout<<"\n\n";
    return 0;
}