#include <time.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "user_process.h"

int main(int argc, char *argv[], char *envp[])
{
    void *ptr = NULL;
    time_t entry_time = time(NULL);
    int run_time = 30;

    printf("User PID: %d\n\n", getpid());

    ptr = MMI(INTERVAL, ATTEMPTS, argc, argv);

    while (time(NULL) - entry_time < run_time);
    DNR(ptr);

    return (0);
}
