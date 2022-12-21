/*
	make_image.cpp
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int print_usage(void);
int create_new_filename(char**, char*);
int make_file(char**);
int cleanup(char**);

///////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
   char* filenames[5];
   int err=0;

   if(argc < 2)
   {
      print_usage();
      return(-1);
   }

   if(create_new_filename(filenames,argv[1]) != 0)
   {
      cleanup(filenames);
      return(-1);
   }

   err=make_file(filenames);

   cleanup(filenames);

   return(err);
}


///////////////////////////////////////////////////////////////////////

int print_usage(void)
{
   printf("makeimage usage:\n"
          "\tmakeimage image.gif\n"
          "\tmakeimage image.jpg\n");

   return(0);
}

///////////////////////////////////////////////////////////////////////

int cleanup(char** filename)
{
   int x=0;

   for(x=0;x<5;x++)
   {
      if(filename[x] != NULL)
      {
         free(filename[x]);
         filename[x]=NULL;
      }
   }
} 

///////////////////////////////////////////////////////////////////////

int create_new_filename(char** filenames,char* old_filename)
{
   char* tmp_ptr=NULL;
   size_t name_size=0;
   int x=0;

   if(old_filename == NULL)
      return(-1);

   tmp_ptr=strrchr(old_filename,'.');

   if(tmp_ptr == NULL)
   {
      printf("File must end in .gif or .jpg\n");
      return(-1);
   }

   if(strncmp(tmp_ptr,".gif",sizeof(".gif")-1) != 0 &&
      strncmp(tmp_ptr,".jpg",sizeof(".jpg")-1) != 0 )
   {
      printf("File must end in .gif or .jpg\n");
      return(-1);
   }   

   
   name_size=tmp_ptr-old_filename+1;

   filenames[0]=strdup(old_filename);
   filenames[1]=(char*) malloc(name_size+sizeof(".cpp")-1);
   filenames[2]=(char*) malloc(name_size+sizeof(".h")-1);
   filenames[3]=(char*) malloc(name_size);
   filenames[4]=strdup(tmp_ptr+1);

   for(x=0;x<5;x++)
   {
      if(filenames[x] == NULL)
      {
         printf("out of memory");
         return(-1);
      }
   }

   for(x=0;x<name_size;x++)
      filenames[3][x]='\0';

   strncpy(filenames[3],old_filename,name_size-1);

   strcpy(filenames[1],filenames[3]);
   strcpy(filenames[2],filenames[3]);

   strcat(filenames[1],".cpp");
   strcat(filenames[2],".h");

   return(0);
}

///////////////////////////////////////////////////////////////////////

int make_file(char** filenames)
{
   FILE* file_read=NULL;
   FILE* file_write=NULL;

   size_t image_size=0;
   int x=0;
   int c=0;

   file_read=fopen(filenames[0],"r");

   if(file_read == NULL)
   {
      printf("error opening file %s\n",filenames[0]);
      return(-1);
   }

   file_write=fopen(filenames[1],"w");

   if(file_write == NULL)
   {
      printf("error opening file %s for writing\n",filenames[1]);
      return(-1);
   }



   fprintf(file_write,"/*\n\t%s\n*/\n\n\n",filenames[1]);
   fprintf(file_write,"#include <stddef.h>\n\n");

   fprintf(file_write,"const char %s_%s[] = {",filenames[3],filenames[4]);

   do
   {
      c=getc(file_read);

      if(c != EOF)
      {
         if(image_size != 0)
            fprintf(file_write,", ");

         if(image_size%10 == 0)
            fprintf(file_write,"\n   ");

         fprintf(file_write,"0x%02x",c);

         image_size++;
      }
   }
   while(c != EOF);

   fprintf(file_write,"\n};\n\n");

   fprintf(file_write,"const size_t %s_%s_size = %u;\n\n",
			filenames[3],filenames[4],image_size);

   fclose(file_read);
   fclose(file_write);

   file_write=fopen(filenames[2],"w");

   if(file_write == NULL)
   {
      printf("could not open %s for writing\n",filenames[2]);
      return(-1);
   }

   fprintf(file_write,"/*\n\t%s\n*/\n\n",filenames[2]);
   fprintf(file_write,"extern const char* %s_%s;\n",
		filenames[3],filenames[4]);
   fprintf(file_write,"extern const size_t %s_%s_size;\n\n",
		filenames[3],filenames[4]);

   fclose(file_write);

   return(0);
}

///////////////////////////////////////////////////////////////////////



