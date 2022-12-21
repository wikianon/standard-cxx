///libfunc.c
 
// Test code for fprintf time stamp  
// This will add  File name, Line number & Time stamp on stderr
// [Baskar]   time stamp test
 
 
#include <stdio.h>
#include <string.h> 
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
 
#define DATA_SIZE   1024
#define DATE_TIME_SIZE 256
#define ERROR  -1
 
 
 
//************************ Method Header ****************************
// Method Name        : fprintf [SJM]
// Purpose            : Wrapper for time stamp impl.
// https://cboard.cprogramming.com/c-programming/100490-creating-our-own-fprintf-function-our-library.html
// Author             : Baskar
// Exceptions Thrown  : None
//*******************************************************************
 
int  fprintf(FILE *fp,const char *format,...)
{
        int l_nNumOfChars=0;
        va_list args;
        va_start(args, format);
        //If it is Standard error or Standard out………. proceed further
        if(fp == stderr || fp == stdout )
        {
             char l_carrTmpData[DATA_SIZE]={0};
             char l_carrTmpDateTime[DATE_TIME_SIZE]={0};
             char l_carrTmpDataAndTime[DATA_SIZE + DATE_TIME_SIZE]={0};
             
             struct timeval timeNalue;
             if( gettimeofday(&timeNalue, NULL) != ERROR )
             {
                struct tm *st_pTimeSt = localtime(&timeNalue.tv_sec);
             
                 if(st_pTimeSt)
                 {
                    snprintf(l_carrTmpDateTime,DATE_TIME_SIZE, "%02d/%02d/%04d %02d:%02d:%02d:%02d",
                        st_pTimeSt->tm_mday, st_pTimeSt->tm_mon+1, st_pTimeSt->tm_year+1900,st_pTimeSt->tm_hour, st_pTimeSt->tm_min, st_pTimeSt->tm_sec,(timeNalue.tv_usec/1000));
                 }
                 
             }
             vsnprintf(l_carrTmpData, DATA_SIZE, format, args);
 
            if(l_carrTmpData[0] == '\n' )
            {
                l_carrTmpData[0]=' ';
                snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"\n%s-%s",l_carrTmpDateTime,l_carrTmpData );
 
            }
            else
            {
                snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"%s - %s",l_carrTmpDateTime,l_carrTmpData );
            }
 
            l_nNumOfChars = fwrite(l_carrTmpDataAndTime,1,strlen(l_carrTmpDataAndTime),stderr);
 
        }
        else // else write to mentioned file
        {
             
            l_nNumOfChars = vfprintf(fp, format, args);
             
        }
        va_end(args);
        return l_nNumOfChars;
}       
//************************ Method Header ****************************
// Method Name        : printf [SJM]
// Purpose            : Wrapper for time stamp impl.
//                      
//                      
// Author             : Baskar
// Exceptions Thrown  : None
//*******************************************************************
int printf (const  char *format, ...)
{
        int l_nNumOfChars=0;
        char l_carrTmpData[DATA_SIZE]={0};
        char l_carrTmpDateTime[DATE_TIME_SIZE]={0};
        char l_carrTmpDataAndTime[DATA_SIZE + DATE_TIME_SIZE]={0};
        struct timeval timeNalue;
 
        va_list args;
        va_start(args, format);
         
        if( gettimeofday(&timeNalue, NULL) != ERROR )
        {
            struct tm *st_pTimeSt = localtime(&timeNalue.tv_sec);
            if(st_pTimeSt)
            {
                snprintf(l_carrTmpDateTime,DATE_TIME_SIZE, "%02d/%02d/%04d %02d:%02d:%02d:%02d",
                        st_pTimeSt->tm_mday, st_pTimeSt->tm_mon+1, st_pTimeSt->tm_year+1900,st_pTimeSt->tm_hour, st_pTimeSt->tm_min, st_pTimeSt->tm_sec,(timeNalue.tv_usec/1000));
            }
 
        }
        vsnprintf(l_carrTmpData, DATA_SIZE, format, args);
        va_end(args);
 
        if(l_carrTmpData[0] == '\n' )
        {
            l_carrTmpData[0]=' ';
            snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"\n%s - %s",l_carrTmpDateTime,l_carrTmpData );
 
        }
        else
        {
            snprintf(l_carrTmpDataAndTime,DATA_SIZE + DATE_TIME_SIZE,"%s - %s",l_carrTmpDateTime,l_carrTmpData );
        }
         
 
         
        l_nNumOfChars = fwrite(l_carrTmpDataAndTime,1,strlen(l_carrTmpDataAndTime),stderr);
 
        return  l_nNumOfChars;
}
 
 
/// Add required functions.../. 
