/* vim: ts=4 sw=4 et
*/
/* The second program is the producer and allows us to enter data for consumers.
 It's very similar to shm1.c and looks like this. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>
#include<sys/sem.h>
#include"semun.h"
#include "shm_com.h"

static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);
static int sem_id;

int main()
{
    int running = 1,i;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    int shmid;
    //char buffer[BUFSIZ];
    

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st) , 0666 | IPC_CREAT);
    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    sem_id = semget((key_t)5678, 1, 0666 | IPC_CREAT);
    if (!set_semvalue()) {
            fprintf(stderr, "Failed to initialize semaphore\n");
            exit(EXIT_FAILURE);
    }
    
    
    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n",(int)shared_memory);
    shared_stuff = (struct shared_use_st *)shared_memory;

    for(i=0;i<10;i++){
        if (!semaphore_p()) exit(EXIT_FAILURE);
        shared_stuff->some_text[i] = 0;
        printf("array[%d] :%d\n",i,shared_stuff->some_text[i]);
        if (!semaphore_v()) exit(EXIT_FAILURE);
    }
    shared_stuff->written_by_you = 1;

    while(running == 1){
        if(!(semaphore_p())) exit(EXIT_FAILURE);
        if(shared_stuff->written_by_you == 2){
            printf("p1 start\n");
            sleep(1);
            for(i=0;i<10;i++){
                if(!(semaphore_v())) exit(EXIT_FAILURE);
                printf("array[%d] :%d\n",i,shared_stuff->some_text[i]);
            }
            running = 2;
        }
        if(!(semaphore_v())) exit(EXIT_FAILURE);
    }
    printf("p1 finish.\n");
    del_semvalue();
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

static int set_semvalue(void)
{
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(void)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* semaphore_p changes the semaphore by -1 (waiting). */

static int semaphore_p(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(void)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}