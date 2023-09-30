#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#define PERMS 0666

typedef struct msgbuf_t
{
    long mtype;
    char mtext[200];
} msgbuf;

int main()
{
    msgbuf buf;
    int msgqid;
    int len;
    key_t key;
    pid_t cid;
    pid_t gcid;
    pid_t pid = getpid();
    pid_t ppid = getppid();

    if ((key = ftok("server.c", 85)) == -1)
    {
        perror("ftok");
        exit(1);
    }
    if ((msgqid = msgget(key, PERMS | IPC_CREAT)) == -1)
    {
        perror("Failed to assign Message queue identifier");
        exit(1);
    }

    // buf.mtype = 1; // we dont really care in this case
    //(fgets(buf.mtext, sizeof (buf.mtext), stdin) != "4")

    while (true)
    {

        printf("Server is waiting for the Client\n");

        if (msgrcv(msgqid, &buf, sizeof(buf), -500, 0) == -1)
        {
            perror("msgrcv");
            exit(1);
        }

        if (buf.mtype == 500)
            break;

        if ((cid = fork()) == -1)
            ;
        {
            // perror("FORK");
            // printf("Error Fork not created !!!!!!");
        }

        if (cid == 0)
        {

            printf("buf %s \n", buf.mtext);
            len = strlen(buf.mtext);
            char choice;
            choice = buf.mtext[0];
            char filename[50];

            switch (choice)
            {
            case '1':

                strcpy(buf.mtext, "hello");
                buf.mtype += 500;
                if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                {
                    perror("Failed to send message");
                    exit(1);
                }
                break;
            case '2':

                strcpy(filename, buf.mtext + 1);
                char buffer2[100];
                int pipe_fd2[2];
                if (pipe(pipe_fd2) == -1)
                {
                    perror("Pipe creation failed");
                    exit(EXIT_FAILURE);
                }
                printf("Pipe created");
                if ((gcid = fork()) == -1)
                    ;
                {
                    // perror("FORK");
                    // printf("Error Fork not created !!!!!!");
                }
                printf("fork created");
                if (gcid == 0)
                { // GrandChild

                    close(pipe_fd2[0]);
                    // printf("Grandchild [%d]",getpid());
                    dup2(pipe_fd2[1], 1);

                    execlp("find", "find", ".", "-name", filename, NULL);
                    close(pipe_fd2[1]);
                    exit(0);
                }
                else // child
                {
                    wait(NULL);
                    // sleep(1);
                    close(pipe_fd2[1]);

                    // printf("[Client] Reading pipe for task 2");
                    ssize_t bytes_read = read(pipe_fd2[0], buffer2, sizeof(buffer2));

                    if (bytes_read == -1)
                    {
                        perror("Read from pipe failed");
                        exit(EXIT_FAILURE);
                    }

                    // printf("child [%d]",getpid());
                    buffer2[bytes_read] = '\0';

                    close(pipe_fd2[0]);

                    strcpy(buf.mtext, buffer2);

                    buf.mtype += 500;
                    // printf("[Client] Sending message for task 2");
                    if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
                    // printf("Message sent");
                    break;
                }

                break;
            case '3':

                strcpy(filename, buf.mtext + 1);
                char buffer3[100];
                int pipe_fd3[2];
                if (pipe(pipe_fd3) == -1)
                {
                    perror("Pipe creation failed");
                    exit(EXIT_FAILURE);
                }

                if ((gcid = fork()) == -1)
                    ;
                {
                    // perror("FORK");
                    // printf("Error Fork not created !!!!!!");
                }

                if (gcid == 0)
                { // Child

                    close(pipe_fd3[0]);
                    dup2(pipe_fd3[1], 1);
                    execlp("wc", "wc", "-w", filename, NULL);
                    close(pipe_fd3[1]);
                    exit(0);
                }
                else // Grandchild
                {
                    wait(NULL);
                    close(pipe_fd3[1]);

                    ssize_t bytes_read = read(pipe_fd3[0], buffer3, sizeof(buffer3));

                    if (bytes_read == -1)
                    {
                        perror("Read from pipe failed");
                        exit(EXIT_FAILURE);
                    }

                    buffer3[bytes_read] = '\0';

                    close(pipe_fd3[0]);

                    strcpy(buf.mtext, buffer3);

                    buf.mtype += 500;
                    if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
                    {
                        perror("Failed to send message");
                        exit(1);
                    }
                    break;
                }

                break;
            }
            exit(0);
        }

        // else
        //{

        //}
    }
    printf("heyy");
    while (wait(NULL) > 0)
        ;
    if (msgctl(msgqid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        exit(1);
    }
    printf("sing with us");
    return 0;
}

// memset(message.msg_text, 0, sizeof(message.msg_text));
// remove newnile at end if it exists
// if (buf.mtext[len - 1] == '\n')
// buf.mtext[len - 1] = '\0';

// if (msgsnd(msqid, &buf, len, 0) == -1) // +1 for '\0'
// perror("msgsnd");