#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{


 int t = uptime();
 int cnt = 100;
 while (cnt) {
     if (t == uptime()) continue;
     cnt--;
     t = uptime();
     printf("uptime is %d     \r", t);
 }
 printf("\n");

  exit(0);
}
