#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void my_sigact_func(int signum, siginfo_t *info, void *data)
{
    printf("signum: %d encountered\n", signum);
}

int main(void)
{
    sigset_t sigsus, oldset;
    struct sigaction my_sigaction, oldact;
    int i, rc;

    my_sigaction.sa_sigaction = my_sigact_func;
    my_sigaction.sa_flags = SA_SIGINFO|SA_RESETHAND;

    rc = sigfillset(&sigsus);
    rc = sigprocmask(SIG_SETMASK, &sigsus, &oldset);
    my_sigaction.sa_mask = sigsus;

    for(i=1; i<=64; i++)
    {
        sigaction(i, &my_sigaction, &oldact);
    }

    while(1);

    return rc;
}
