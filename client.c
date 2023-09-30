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

    int client_id;
    printf("Enter CLient-ID: ");
    scanf("%d", &client_id);

    if ((key = ftok("server.c", 85)) == -1)
    {
        perror("ftok returned -1");
        exit(1);
    }

    // change perms|ipc_creat to perms
    if ((msgqid = msgget(key, PERMS)) == -1)
    {
        perror("Failed to assign Message queue identifier");
        exit(1);
    }

    buf.mtype = client_id;
    while (true)
    {
        int choice;
        // strcpy(output, "");
        printf("\n\n1. Enter 1 to contact the Ping Server\n2. Enter 2 to contact the File Search Server\n3. Enter 3 to contact the File Word Count Server\n4. Enter 4 if this Client wishes to exit\n");
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 4)
        {
            break;
        }

        char filename[50];

        switch (choice)
        {
        case 1:
            // buf.mtext="";
            strcpy(buf.mtext, "1");
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message");
                exit(1);
            }

            // printf("[Client %d] 1 message sent\n", client_id);

            if (msgrcv(msgqid, &buf, sizeof(buf), buf.mtype + 500, 0) == -1) // we add 500 to distinguish direction of msg
            {
                perror("msgrcv");
                exit(1);
            }
            // if(buf.mtype == client_id)
            //{
            // output=buf.mtext;
            printf("%s", buf.mtext);
            //}
            break;
        case 2:
            // buf.mtext="";
            printf("\nEnter file name: ");
            scanf("%s", filename);
            strcpy(buf.mtext, "2");
            strcat(buf.mtext, filename);
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message");
                exit(1);
            }
            // printf("[Client %d] 2 message sent\n", client_id);
            sleep(1);
            if (msgrcv(msgqid, &buf, sizeof(buf), buf.mtype + 500, 0) == -1) // we add 500 to distinguish direction of msg
            {
                perror("msgrcv");
                buf.mtype -= 500;
                exit(1);
            }

            else
            {
                printf("%s", buf.mtext);
            }
            buf.mtype -= 500;

            break;
        case 3:
            // buf.mtext="";
            printf("\nEnter file name: ");
            scanf("%s", filename);
            strcpy(buf.mtext, "3");
            strcat(buf.mtext, filename);
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message");
                exit(1);
            }
            // printf("[Client %d] 3 message sent\n", client_id);
            sleep(1);

            if (msgrcv(msgqid, &buf, sizeof(buf), buf.mtype + 500, 0) == -1) // we add 500 to distinguish direction of msg
            {
                perror("msgrcv");
                exit(1);
            }

            // if(buf.mtype == client_id)
            //{
            // output=buf.mtext;
            buf.mtype -= 500;
            printf("%c", buf.mtext[0]);
            //}
            break;

        default:
            printf("\nInvalid choice! try again \n");
            break;
        }
    }
    return 0;
}