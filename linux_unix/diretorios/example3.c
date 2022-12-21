#include <ftw.h>
#include <stdio.h>
#include <unistd.h>

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
} 

int main()
{
 rmrf("/home/wiki/Workspace/GitHub/standard-cxx/linux_unix/diretorios/new_dir");
 
 return 0;
}