#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>

#include <message/message.h>
#include <argument/argument.h>

int main(int argc, char **argv)
{
    Argument::ListeMap liste;
    int largc;
    char **largv;
    struct passwd *pwd;
    char *va[1024];
    int i;

    liste["locale"] = Argument::Liste("-locale", 'c', 1, DEF_LOCALE);

#include "rexec.inc"

    largc = argc;
    largv = argv;

    Argument a(&liste, *argv);
    a.scan(--largc, ++largv);

    pwd = getpwuid(getuid());

    setuid( 0 );
    setgroups(0, NULL);
    setgid( 0 );

   if ( chdir((char*)((std::string)a["projectroot"]).c_str()) < 0)
    {
        fprintf(stderr, "can't change to projectroot\n");
        exit (-1);
    }

    va[0] = new char[strlen(largv[0]) + 40];
    strcpy( (char *)va[0], (std::string("exec/system/shell/") + *largv).c_str());

    va[1] = (char*)"-locale";
    va[2] = (char*)((std::string)a["locale"]).c_str();
    va[3] = (char*)"-project";
    va[4] = (char*)((std::string)a["project"]).c_str();
    va[5] = (char*)"-user";
    va[6] = pwd->pw_name;

    for ( i = 1 ; i != 1000 && largv[i] != NULL && largv[i+1] != NULL; i++, i++ )
    {
        va[i+6] = new char[strlen(largv[i]) + 4];
        strcpy((char *)va[i+6], ( std::string("-va") + largv[i]).c_str());
        va[i+7] = largv[i+1];
    }
    va[i + 6] = NULL;

    execv( (std::string("exec/system/shell/") + *largv).c_str(), va);
    fprintf(stderr, "command not found <%s>\n", (((std::string)a["projectroot"]) + "/exec/system/shell/" + *largv).c_str());
    fflush(stderr);
    exit(-1);
}
