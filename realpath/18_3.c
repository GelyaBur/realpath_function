#include <libgen.h>
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/dir.h>
#include <ctype.h>
#include <limits.h>

#define _BSD_SOURCE
#define _XOPEN_SOURCE_EXTENDED
#define MAX_ARR 20

char*realpath_impl(char *path, char *resolved_path)
{
    int fd, fd2;
    struct stat sb;
    struct stat sb2;
    char *buf1, *buf2, *dirn, *basen;
    char buf[PATH_MAX];
    char *p;
    DIR *dirp;
    struct dirent *dp;
    ssize_t nbytes;
    fd = open(".", O_RDONLY);
    printf("cwd_begin = %s\n", path);
    if (lstat(path, &sb) == -1)
        errExit("lstat");
    dirn = strdup(path);
    basen = strdup(path);
    buf1 = dirname(dirn);
    buf2 = basename(basen);
    if (S_ISLNK(sb.st_mode)) {
        nbytes = readlink(path, buf, PATH_MAX-1);
        if (nbytes == -1)
            errExit("readlink");
        sprintf(resolved_path, "%s/%s", buf1, buf);
        if (stat(resolved_path, &sb2) == -1)
            errExit("stat");
        if (S_ISREG(sb2.st_mode)) {
            p = buf;
            chdir(buf1);
            path = getcwd(NULL, 0);
            sprintf(resolved_path, "%s/%s", path, p);
        } else if (S_ISDIR(sb2.st_mode)) {
            chdir(path);
            path = getcwd(NULL, 0);
            sprintf(resolved_path, "%s", path);
        }
    } else if (S_ISDIR(sb.st_mode)){
        chdir(path);
        path = getcwd(NULL, 0);
        sprintf(resolved_path, "%s/%s", path, p);
    } else if(S_ISREG(sb.st_mode)) {
        p = buf2;
        chdir(buf1);
        path = getcwd(NULL, 0);
        sprintf(resolved_path, "%s/%s", path, p);
    }
    fchdir(fd);
    close(fd);
    return resolved_path;
}
int main()
{
    char resolved_path[PATH_MAX];
    printf("%s\n", realpath_impl("/home/gelya/src/Linux_C/./link_tee", resolved_path));
}
