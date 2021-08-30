#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}

void find(char *path, char *filename)
{
    char buf[512], *p;

    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    //printf("path is %s\n", path);
    //printf("begin find\n");
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        //printf("a sub file is %s\n", buf);
        //printf("p is [%s], len is %d\n", p, strlen(p));
        if (!strcmp(p, "."))
            continue;
        if (!strcmp(p, ".."))
            continue;

        switch (st.type)
        {
        case T_FILE:
            if (strcmp(filename, p) == 0)
                printf("%s\n", buf);
            break;

        case T_DIR:
            if (strcmp(filename, p) == 0)
                printf("%s\n", buf);
            //printf("%s is a dir!\n", buf);
            if (strlen(buf) + 1 + DIRSIZ + 1 > sizeof buf)
            {
                printf("find: path too long\n");
                break;
            }
            find(buf, filename);
            break;
        }
    }

    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        fprintf(2, "Usage: find [path] [filename]\n");
        exit(0);
    }

    find(argv[1], argv[2]);

    exit(0);
}
