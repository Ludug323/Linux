/* Here's the receiver program. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>


struct my_msg_st {
    long int my_msg_type;
    char some_text[BUFSIZ];
};

int main()
{
    int running = 1,i;
    int msgid_1, msgid_2;
    int num = 0;
    struct my_msg_st some_data;
    long int msg_to_receive = 0;

/* First, we set up the message queue. */

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

/* Then the messages are retrieved from the queue, until an end message is encountered.
 Lastly, the message queue is deleted. */

    while(running) {
        int count = 0;
        if (msgrcv(msgid_1, (void *)&some_data, BUFSIZ,
                   msg_to_receive, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        some_data.my_msg_type = 2;
        printf("You wrote: %s", some_data.some_text);
        num = atoi(some_data.some_text);
        if(num >0){
            for(i=1;i<num;i++){
                if(num %i == 0){
                    count +=1;
                }
            }        
            if(count == 1){
                sprintf(some_data.some_text,"%d 是質數",num);
                printf("%s\n",some_data.some_text);
            }else{   
                sprintf(some_data.some_text,"%d 不是質數",num);
                printf("%s\n",some_data.some_text);
            }
        }  
        if (msgsnd(msgid_2, (void *)&some_data, BUFSIZ, 0) == -1) {
            fprintf(stderr, "msgsnd failed\n");
            exit(EXIT_FAILURE);
        }
        if (strncmp(some_data.some_text, "-1", 2) == 0) {
            running = 0;
        }
    } 
        
    if (msgctl(msgid_1, IPC_RMID, 0) == -1) {
            fprintf(stderr, "msgctl(IPC_RMID) failed\n");
            exit(EXIT_FAILURE);
    }    
    exit(EXIT_SUCCESS);
}
