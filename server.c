#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#define PERMS 0666

struct msg_buffer
{
    long mtype;
    char mtext[200];
};
typedef struct msg_buffer msgbuf;

int main()
{
    int msgqid;
    key_t key;
    int len;
    pid_t cid;
    pid_t pid = getpid();
    pid_t ppid = getppid();

    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("Pipe creation failed\n");
        exit(EXIT_FAILURE);
    }
    if ((key = ftok("server.c", 85)) == -1)
    {
        perror("ftok returned -1\n");
        exit(1);
    }
    if ((msgqid = msgget(key, PERMS | IPC_CREAT)) == -1)
    {
        perror("Failed to assign Message queue identifier\n");
        exit(1);
    }

    printf("Server is waiting for the Client\n");
    while (true)
    {
        if (ppid != getppid())
        {
            exit(0);
        }

        if ((cid = fork()) == -1)
        {
            perror("FORK");
        }

        if (cid == 0)
        {
            msgbuf buf;
            if (msgrcv(msgqid, &buf, sizeof(buf), 0, 0) == -1)
            {
                perror("Couldn't receive message from msgque\n");
                exit(1);
            }
            len = strlen(buf.mtext);
            char choice = buf.mtext[0];
            char filename[50];
            switch (choice)
            {
            case '1':
                buf.mtext = "Hello";
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                {
                    perror("Failed to send message");
                    exit(1);
                }
                break;
            case '2':
                strncpy(filename, buf.mtext + 1, len - 1);
                filename[len - 1] = '\0';

                close(pipe_fd[0]);
                dup2(pipe_fd[1], 1);
                dup2(pipe_fd[1], 2);
                close(pipe_fd[1]);

                // exec
                break;
            }
        case '3':
            strncpy(filename, buf.mtext + 1, len - 1);
            filename[len - 1] = '\0';

            close(pipe_fd[0]);
            dup2(pipe_fd[1], 1);
            dup2(pipe_fd[1], 2);
            close(pipe_fd[1]);

            // exec
            break;
        }
    }
}