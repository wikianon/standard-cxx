#include <iomanip>
#include <iostream>

int main()
{
    std::time_t now = std::time(nullptr);

    std::string lang[6] = { "pt_BR", "en_US", "en_GB", "de_DE", "ja_JP", "ru_RU" };

    for( int i = 0; i < 6; i++ )
    {
        std::string loc = lang[i] + ".utf8";

        try
        {
            std::cout.imbue(std::locale(loc));
            std::cout << lang[i] << ": " << std::put_time(std::gmtime(&now), "%c") << std::endl;
        }
        catch( std::exception &e )
        {
            std::cout << lang[i] << " nao suportada!" << std::endl;
        }
    }

    return 0;
} 
