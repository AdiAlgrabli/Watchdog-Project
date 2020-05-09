#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "sched.h"
#include "user_process.h"

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define RESET "\x1B[0m"

#define WD_PROGRAM "./wd" /* the executable file of wd_process (instead a.out) */

/* ADD TO COMPILE LINE: -D_XOPEN_SOURCE=700 -lpthread */

typedef int (*op_func_t)(void *param);
typedef void (*sig_handler_func_t)();

int wd_signal_arrived = 0;
sched_t *sched = NULL;
int dnr_flag = 0;

typedef struct crit_data_s
{
    size_t interval;
    size_t attempts;
    char **argv;
    int argc;
    pid_t pid_to_sig;

} crit_data_t;

void InitCritDataStruct(crit_data_t *crit_data, int argc, char **argv)
{
    crit_data->interval = INTERVAL;
    crit_data->attempts = ATTEMPTS;
    crit_data->argv = argv;
    crit_data->argc = argc;
}

void RecreateWD(crit_data_t *crit_data)
{
    pid_t wd_pid = fork();

    if (0 == wd_pid)
    {
        if (-1 == execv(WD_PROGRAM, crit_data->argv))
        {
            perror(RED "execv" RESET);

            return;
        }
    }

    crit_data->pid_to_sig = wd_pid;
}

int SendSigToWD(void *crit_data)
{
    crit_data_t crit_data_l = *(crit_data_t *)crit_data;

    puts(YEL "\nUser: Sending signal to WD" RESET);    
    kill(crit_data_l.pid_to_sig, SIGUSR1);

    return (1);
}

int CheckWDSig(void *crit_data)
{
    if (wd_signal_arrived)
    {
        wd_signal_arrived = 0;
    }
    else
    {
        puts(RED "User: WD is dead. Recreating WD..." RESET);
        RecreateWD(crit_data);
    }

    return (1);
}

void SetSched(op_func_t send_sig, op_func_t check_sig, void *crit_data)
{
    sched = SchedCreate();
    if (NULL == sched)
    {
        perror(RED "Error in sched create" RESET);

        return;
    }

    SchedAddTask(sched, send_sig, crit_data, ATTEMPTS);
    SchedAddTask(sched, check_sig, crit_data, INTERVAL);
}

void *GuardWD(void *crit_data)
{
    crit_data_t crit_data_l = *(crit_data_t *)crit_data;

    SetSched(SendSigToWD, CheckWDSig, crit_data);
    SchedRun(sched);
    
    puts("\n\nUser: Sending WD signal to DNR. Bye Bye!\n\n");
    kill(crit_data_l.pid_to_sig, SIGUSR2);

    return (NULL);
}

void SigThreadHandler()
{
    wd_signal_arrived = 1;
    puts(YEL "User: Signal was received from WD" RESET);
}

void SetSigactionStruct(struct sigaction *sa, sig_handler_func_t sig_handler, 
                                                                        int sig)
{
    sa->sa_handler = sig_handler;
    sa->sa_flags = 0;
    sigaction(sig, sa, NULL);
}

int IsWDExist()
{
    return (0 != strcmp("0", getenv("WD_CALLER"))); /* if WD_CALLER's val != 0 */
}

void *MMI(size_t interval, size_t attempts, int argc, char *argv[])
{
    struct sigaction sa_thread;
    pthread_t *user_thread = NULL;
    crit_data_t *crit_data = malloc(sizeof(crit_data_t));
    if (NULL == crit_data)
    {
        return (NULL);
    }

    dnr_flag = 0;

    InitCritDataStruct(crit_data, argc, argv);
    setenv("WD_CALLER", "0", 0);

    SetSigactionStruct(&sa_thread, SigThreadHandler, SIGUSR1);

    if (IsWDExist())
    {
        crit_data->pid_to_sig = atoi(getenv("WD_CALLER"));
    }
    else
    {
        crit_data->pid_to_sig = fork();

        if (0 == crit_data->pid_to_sig)
        {
            if (-1 == execv(WD_PROGRAM, argv))
            {
                perror(RED "execv" RESET);

                return (NULL);
            }
        }
    }

    user_thread = malloc(sizeof(pthread_t));
    if (NULL == user_thread)
    {
        return (NULL);
    }

    pthread_create(user_thread, NULL, GuardWD, crit_data);

    return ((void *)user_thread);
}

void DNR(void *thread_id)
{
    puts(RED "\n\nUser has asked to do not resuscitate. Terminating WD.\n\n" RESET);

    dnr_flag = 1;

    SchedStop(sched);
    puts("\n\nStop user scheduler.");

    pthread_join(*(pthread_t *)thread_id, NULL);
    free(thread_id);
    unsetenv("WD_CALLER");

    puts("\n\nDestroy user scheduler.");
    SchedDestroy(sched);
}
