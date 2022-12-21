#include <ctime>
#include <cstdio>
 
int main(void)
{
    time_t currentTime = 0; // Amount of seconds since 1 jan 1970
    tm *timeInfo;
    unsigned dia = 0;
    unsigned mes = 0;
    unsigned ano = 0;
    const char *diaSemana[] = { "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado" };
    const char *mesAno[] = { "Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho", "Julho","Agosto", "Setembro", "Outubro", "Novembro", "Dezembro" };
 
    /**
     *
     * Calculo de uma data pré-definida
     *
     */
 
    /**
     * Leitura de uma data para verificar o dia da semana
     */
    printf("\n\tDia: ");
    fscanf(stdin, " %d", &dia);
    printf("\n\tMes: ");
    fscanf(stdin, " %d", &mes);
    printf("\n\tAno: ");
    fscanf(stdin, " %d", &ano);
    
    /**
     * Pega a hora atual e converte em uma estrutura tm
     */
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    
    /**
     * Atualiza os valores dia, mes e ano da estrutura tm
     */
    timeInfo->tm_mday = dia;
    timeInfo->tm_mon  = mes - 1; /* Mes vai de 0 a 11 */
    timeInfo->tm_year = ano - 1900; /* Ano inicial é 1900 */
    
    /**
     * Atualizar o dia da semana (tm_wday) e dia do ano (tm_yday), de acordo com os atualizados anteriormente
     */
    mktime(timeInfo);
    
    /**
     * Imprime o dia da semana
     */
    printf(
           "\n\t%02u/%s/%04u foi %s e o dia do ano foi %02d.",
           timeInfo->tm_mday, mesAno[timeInfo->tm_mon], timeInfo->tm_year + 1900, diaSemana[timeInfo->tm_wday], timeInfo->tm_yday + 1
          );
    
    /**
     *
     * Calculo da data atual
     *
     */
    
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    
    printf(
           "\n\n\tData atual: %02d/%s/%04d - Hora: %02d:%02d:%02d (%s)\n\n",
           timeInfo->tm_mday, mesAno[timeInfo->tm_mon], timeInfo->tm_year+1900, timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec, diaSemana[timeInfo->tm_wday]
          );
 
    return 0;
}