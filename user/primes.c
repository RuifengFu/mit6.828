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

    int lp[2];
    int rp[2];

    char buf[4];
    pipe(lp);
    int pid = fork();
    if (pid < 0) {
        fprintf(1, "fork failed\n");
        exit(1);
    } else if (pid > 0) {
        close(lp[0]);
        for (int i = 2; i <= 35; i++) {
            memcpy(buf, &i, 4);
            write(lp[1], buf, 4);
            //sfprintf(2, "loop %d\n", i);
        }
        close(lp[1]);
        wait((int *) 0);
        exit(0);
    } else {
        close(lp[1]);
        int n, p;
        int child = 0;
        read(lp[0], buf, 4);
        memcpy(&p, buf, 4);
        fprintf(1, "prime %d\n", p);
        while(read(lp[0], buf, 4)) {
            memcpy(&n, buf, 4);
            if (n % p != 0) {
                if (!child) {
                    pipe(rp);
                    child = fork();
                } 
                if (child < 0) {
                    fprintf(2, "fork failed");
                    exit(1);
                } else if (child > 0) {
                    memcpy(buf, &n, 4);
                    write(rp[1], buf, sizeof(buf));
                } else {
                    
                    lp[0] = rp[0];
                    lp[1] = rp[1];
                    rp[0] = rp[1] = 0;
                    close(lp[1]);
                    read(lp[0], buf, sizeof(buf));
                    memcpy(&p, buf, 4);
                    fprintf(1, "prime %d\n", p);
                    child = 0;
                }
            }
        }
        close(rp[0]);
        close(rp[1]);
        close(lp[0]);
        close(lp[1]);
        wait((int *)0);
        exit(0);

    }
  exit(0);
}
