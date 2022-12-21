#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    FILE *fp;
    char str[31], *pt, *pFim;
    float f1, f2, f3;
    unsigned int memF, memFlivre, memFocupada;


    fp = fopen("/proc/cpuinfo","r");
    if(fp == NULL){
       fprintf(stderr,"Error na abertura de arquivo da cpu");
       exit(-1);
    }
    printf("\n\nInformacoes de CPU da maquina\n");
    while(fgets(str,30,fp)){
        if((strncmp(str,"processor",8)==0) || \
            (strncmp(str,"vendor_id",8)==0) || \
            (strncmp(str,"cpu MHz",6)==0)) {
               printf("%s",str);
        }
    }

    fclose(fp);

    fp = fopen("/proc/loadavg","r");
    if(fp == NULL){
       fprintf(stderr,"Error na abertura de arquivo do load");
       exit(-1);
   }

    printf("\nLoad da CPU");
     while(fgets(str,30,fp)){
       sscanf(str,"%f %f %f",&f1,&f2,&f3);
       printf("\n%.2f %.2f %.2f\n",f1,f2,f3);
     }

     fclose(fp);

     fp = fopen("/proc/meminfo","r");
     if(fp == NULL){
       fprintf(stderr,"Error ao acessar arquivo memoria");
       exit(-1);
     }

      printf("\nMemoria da maquina\n");
      while(fgets(str,30,fp)){
         if(strncmp(str,"MemTotal:",9)==0) {
            pt=strtok(str," ");
            pt=strtok(NULL," ");
            memF=strtol(pt,&pFim,10);
            printf("Fisica: %d [kb]\n",memF);
         }
         if(strncmp(str,"MemFree:",8)==0) {
            pt=strtok(str," ");
            pt=strtok(NULL," ");
            memFlivre=strtol(pt,&pFim,10);
            printf("Fisica livre: %d [kb]\n",memFlivre);
         }
     }
      printf("Ocupada: %d [kb]\n\n", memF-memFlivre);


     fclose(fp);
 }