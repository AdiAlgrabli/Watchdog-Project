#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "sched.h"
#include "user_process.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define CYN "\x1B[36m"
#define RESET "\x1B[0m"

/* ADD TO COMPILE LINE: -D_XOPEN_SOURCE=700 -lpthread */

typedef int (*op_func_t)(void *param);
typedef void (*sig_handler_func_t)();

int user_signal_arrived = 0;
sched_t *sched = NULL;
int dnr_flag = 0;

typedef struct crit_data_s
{
    size_t interval;
    size_t attempts;
    int argc;
    char **argv;
    pid_t pid_to_sig;

} crit_data_t;

void InitCritDataStruct(crit_data_t *crit_data, int argc, char **argv)
{
    crit_data->interval = INTERVAL;
    crit_data->attempts = ATTEMPTS;
    crit_data->argv = argv;
    crit_data->argc = argc;
}

void RecreateUser(crit_data_t *crit_data)
{
    pid_t child_pid = fork();

    if (0 == child_pid)
    {
        if (-1 == execv(crit_data->argv[0], crit_data->argv))
        {
            perror("execv");

            return;
        }
    }

    printf("WD: New user pid after resuscitate: %d\n", child_pid);
    crit_data->pid_to_sig = child_pid;
}

int SendSigToUser(void *crit_data)
{
    crit_data_t crit_data_l = *(crit_data_t *)crit_data;

    puts(CYN "\nWD: Sending signal to user" RESET);

    kill(crit_data_l.pid_to_sig, SIGUSR1);

    return (1);
}

int CheckUserSig(void *crit_data)
{
    crit_data_t crit_data_l = *(crit_data_t *)crit_data;

    if (user_signal_arrived && dnr_flag == 0)
    {
        user_signal_arrived = 0;
    }
    else
    {
        char buff[50] = {'\0'};
        
        puts(RED "WD: User is dead. Recreating user." RESET);
        
        sprintf(buff, "%d", getpid());

        setenv("WD_CALLER", buff, 1);
        printf("WD: setenv WD_CALLER to %s\n", buff);
        RecreateUser(crit_data);


        /* NEED TO CHECK THESE LINES */
        /* while (!user_signal_arrived);
        setenv("WD_CALLER", "0", 1); */
    }

    return (1);
}


void SetSched(op_func_t send_sig, op_func_t check_sig,
                            void *crit_data, size_t attempts, size_t interval)
{
    sched = SchedCreate();
    if (NULL == sched)
    {
        perror("Error in sched create");

        return;
    }

    SchedAddTask(sched, send_sig, crit_data, ATTEMPTS);
    SchedAddTask(sched, check_sig, crit_data, INTERVAL);
}

void *GuardUser(void *crit_data)
{
    SetSched(SendSigToUser, CheckUserSig, crit_data, ATTEMPTS, INTERVAL);
    if (NULL == sched)
    {
        return (NULL);
    }

    SchedRun(sched);
    SchedDestroy(sched);

    return (NULL);
}

void SigWdHandler()
{
    user_signal_arrived = 1;
    puts(CYN "WD: Signal was received from user" RESET);
}

void SigDNRHandler()
{
    dnr_flag = 1;
    SchedStop(sched);
}

void SetSigactionStruct(struct sigaction *sa, sig_handler_func_t sig_handler, int sig)
{
    sa->sa_handler = sig_handler;
    sa->sa_flags = 0;
    sigaction(sig, sa, NULL);
}

int main(int argc, char *argv[])
{
    crit_data_t crit_data;
    struct sigaction sa_wd;
    struct sigaction sa_wd_dnr;
    
    dnr_flag = 0;
    InitCritDataStruct(&crit_data, argc, argv);


    puts(GRN "WD was successfully created!" RESET);
    printf("WD pid: %d\n", getpid());

    SetSigactionStruct(&sa_wd, SigWdHandler, SIGUSR1);
    SetSigactionStruct(&sa_wd_dnr, SigDNRHandler, SIGUSR2);

    crit_data.pid_to_sig = getppid();

    GuardUser(&crit_data);

    return (0);
}
