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
    if ((key = ftok("client.c", 85)) == -1)
    {
        perror("ftok returned -1");
        exit(1);
    }

    if ((msgqid = msgget(key, PERMS | IPC_CREAT)) == -1)
    {
        PERROR("Failed to connect to Message queue");
        exit(1);
    }

    while (true)
    {
        msgbuf buf;
        if (msgrcv(msgqid, &buf, sizeof(buf), 0, 0) == -1)
        {
            perror("Couldn't receive message from msgque");
            exit(1);
        }
        char menu = buf.mtext[0];
        switch (menu)
        {
        case '1':
            printf("Server pinged!");
            break;
        case '2':
            // start here
            break;
        }
    }
}