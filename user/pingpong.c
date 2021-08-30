#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    
    if (argc > 1) {
        fprintf(2, "too much arguments\n");
        exit(1);
    }
    
    
    int p[2], q[2];
    pipe(p);
    pipe(q);

    int pid = fork();
    char buf[20];
    if (pid < 0) {
        fprintf(2, "fork failed");
        exit(1);
    } else if (pid == 0) {
        pid = getpid();
        read(q[0], buf, 20);
        fprintf(1, "%d: received ping\n", pid);
        write(p[1], "child pong\n", 12);
        exit(0);
    } else {

        pid = getpid();
        write(q[1], "ping", 5);
        read(p[0], buf, 20);
        fprintf(1, "%d: received pong\n", pid);
        exit(0);
    }
  
    exit(1);

}
