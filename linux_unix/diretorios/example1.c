#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


/* recursive mkdir */
int mkdir_p(const char *dir, const mode_t mode)
{
    char tmp[256];
    char *p = NULL;
    struct stat sb;
    size_t len;

    /* copy path */
    strncpy(tmp, dir, sizeof(tmp));
    len = strlen(dir);
    if (len >= sizeof(tmp)) {
        return -1;
    }

    /* remove trailing slash */
    if(tmp[len - 1] == '/') {
        tmp[len - 1] = 0;
    }

    /* recursive mkdir */
    for(p = tmp + 1; *p; p++) {
        if(*p == '/') {
            *p = 0;
            /* test path */
            if (stat(tmp, &sb) != 0) {
                /* path does not exist - create directory */
                if (mkdir(tmp, mode) < 0) {
                    return -1;
                }
            } else if (!S_ISDIR(sb.st_mode)) {
                /* not a directory */
                return -1;
            }
            *p = '/';
        }
    }
    /* test path */
    if (stat(tmp, &sb) != 0) {
        /* path does not exist - create directory */
        if (mkdir(tmp, mode) < 0) {
            return -1;
        }
    } else if (!S_ISDIR(sb.st_mode)) {
        /* not a directory */
        return -1;
    }
    return 0;
}

int main()
{
 mkdir_p("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/mod1", 0777);
 mkdir_p("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/mod2", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
 mkdir_p("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/mod3", S_IRWXU | S_IRWXG | S_IRWXO);
 mkdir_p("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/mod4", ACCESSPERMS);
 mkdir_p("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/mod5", S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
 return 0;
}