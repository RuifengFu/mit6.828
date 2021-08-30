#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{

    char buf[512];
    char *exec_argv[32];
    int i = 0;
    for (i = 1; i < argc; i++)
    {
        exec_argv[i - 1] = argv[i];
    }
    exec_argv[argc] = 0;
    i = 0;
    while (1)
    {
        gets(buf, 512);
        if (!strlen(buf))break;
        int pid = fork();
        if (pid == 0)
        {
            buf[strlen(buf)-1] = 0;
            exec_argv[argc - 1] = buf;
            exec(argv[1], exec_argv);
        } else if (pid < 0) {
            fprintf(2, "fork failed\n");
            exit(1);
        } else {
            wait((int *)0);
        }
    }

    exit(0);
}
