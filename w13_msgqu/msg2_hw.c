/* The sender program is very similar to msg1.c. In the main set up, delete the
 msg_to_receive declaration and replace it with buffer[BUFSIZ], remove the message
 queue delete and make the following changes to the running loop.
 We now have a call to msgsnd to send the entered text to the queue. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>

#define MAX_TEXT 512

struct my_msg_st {
    long int my_msg_type;
    char some_text[MAX_TEXT];
};

int main()
{
    int running = 1;
    struct my_msg_st some_data;
    int msgid_1,msgid_2;
    char buffer[BUFSIZ];
    long int msg_to_receive = 0;


    msgid_1 = msgget((key_t)1234, 0666 | IPC_CREAT);
    msgid_2 = msgget((key_t)5678, 0666 | IPC_CREAT);

    if (msgid_1 == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    if (msgid_2 == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
    
    while(running) {
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        some_data.my_msg_type = 1; 
        strcpy(some_data.some_text, buffer);
        if (msgsnd(msgid_1, (void *)&some_data, MAX_TEXT, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        if (strncmp(buffer, "-1", 2) == 0) {
            running = 0;
        }  
    } 

    while(running == 0){
        if (msgrcv(msgid_2, (void *)&some_data, BUFSIZ,
                msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }  
        if (strncmp(some_data.some_text, "-1", 2) == 0) {
            running = 1;
        } 
        printf("%s\n",some_data.some_text);
    }  
    
    if (msgctl(msgid_2, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }   

    exit(EXIT_SUCCESS);
}
