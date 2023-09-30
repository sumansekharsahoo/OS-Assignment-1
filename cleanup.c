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
#include <stddef.h>
#define PERMS 0666

struct msg_buffer
{
    long mtype;
    char mtext[200];
};

typedef struct msg_buffer msgbuf;

int main()
{
    msgbuf buf;
    int msgqid;
    int len;
    // char output[200]
    key_t key;

    if ((key = ftok("server.c", 85)) == -1)
    {
        perror("ftok returned -1");
        exit(1);
    }

    // change perms|ipc to perms
    if ((msgqid = msgget(key, PERMS)) == -1)
    {
        perror("Failed to assign Message queue identifier");
        exit(1);
    }

    buf.mtype = 500;
    while (true)
    {
        int choice;
        // strcpy(output, "");
        printf("\n\nYes(1) or No(0)");
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        int flag = 0;

        switch (choice)
        {
        case 1:
            // buf.mtext="";
            strcpy(buf.mtext, "destory");
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message");
                exit(1);
            }
            flag = 1;

            break;
        case 0:

            break;
        }

        if (flag == 1)
        {
            break;
        }
    }
    return 0;
}