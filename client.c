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
    key_t key;
    char output[200];

    int client_id;
    printf("Enter CLient-ID: ");
    scanf("%d", &client_id);

    if ((key = ftok("server.c", 85)) == -1)
    {
        perror("ftok returned -1\n");
        exit(1);
    }

    if ((msgqid = msgget(key, PERMS)) == -1)
    {
        perror("Failed to assign Message queue identifier\n");
        exit(1);
    }

    while (true)
    {
        int choice;
        printf("\n\n1. Enter 1 to contact the Ping Server\n2. Enter 2 to contact the File Search Server\n3. Enter 3 to contact the File Word Count Server\n4. Enter 4 if this Client wishes to exit");
        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 4)
        {
            break;
        }

        char filename[50];
        buf.mtype = client_id;
        switch (choice)
        {
        case 1:
            strcpy(buf.mtext, "1");
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message\n");
                exit(1);
            }
            sleep(1);
            if (msgrcv(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("msgrcv returned -1\n");
                exit(1);
            }
            if (buf.mtype == client_id)
            {
                strcpy(output, buf.mtext);
                printf("%s\n", output);
            }
            break;
        case 2:
            printf("\nEnter file name: ");
            scanf("%s", filename);
            strcpy(buf.mtext, "2");
            strcat(buf.mtext, filename);
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message\n");
                exit(1);
            }
            sleep(1);
            if (msgrcv(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("msgrcv returned -1\n");
                exit(1);
            }
            if (buf.mtype == client_id)
            {
                strcpy(output, buf.mtext);
                printf("%s\n", output);
            }
            break;
        case 3:
            printf("\nEnter file name: ");
            scanf("%s", filename);
            strcpy(buf.mtext, "3");
            strcat(buf.mtext, filename);
            if (msgsnd(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("Failed to send message\n");
                exit(1);
            }
            sleep(1);
            if (msgrcv(msgqid, &buf, sizeof(buf), 0) == -1)
            {
                perror("msgrcv returned -1\n");
                exit(1);
            }
            if (buf.mtype == client_id)
            {
                strcpy(output, buf.mtext);
                printf("%s\n", output);
            }
            break;
        case 4:
            break;
        default:
            printf("\nInvalid choice!\n");
            break;
        }
    }
    return 0;
}