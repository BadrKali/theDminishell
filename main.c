#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *env[])
{
    char *avr[] = {"ls", "-la", NULL};
    char *avr2[] = {"echo", "main", NULL};
    int fds[2];

    pipe(fds);

    if (fork() == 0)
    {
        dup2(fds[1], 1);
        close(fds[0]);
        close(fds[1]);
        execve("/bin/ls", avr, env);
    }
    dup2(fds[0], 0);
    close(fds[0]);
        close(fds[1]);
    execve("/usr/bin/grep", avr2, env);
    
}