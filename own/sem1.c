#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include "semun.h"
#include<sys/sem.h>


//create  the semaphore function what we use
static int set_semvalue(void);
static void del_semvalue(void);
static int semaphore_p(void);
static int semaphore_v(void);

static int sem_id;

int main(int argc, char *argv[])
{

    char *op_char = "process 1";
    int i, pause_time;
    
    sem_id = semget((key_t)123, 1, 0666 | IPC_CREAT);
    srand((unsigned int)getpid());

    if (sem_id == -1)
    {
        fprintf(stderr, "ID ERROR\n");
        exit(EXIT_FAILURE);
    }

    if (argc > 1)
    {
        if (!(set_semvalue()))
        {
            fprintf(stderr, "ERROR SET SEMAPHORE VALUE\n");
            exit(EXIT_FAILURE);
        }
        op_char = "process 2";
        sleep(3);
    }

    for (i = 0; i < 5; i++)
    {
        if (!(semaphore_p()))
            exit(EXIT_FAILURE);
        printf("%s  ", op_char);
        fflush(stdout);

        pause_time = (rand() % 3);
        sleep(pause_time);

        printf("%s\n", op_char);
        fflush(stdout);

        if (!(semaphore_v()))
            exit(EXIT_FAILURE);

        pause_time = (rand() % 2);
        sleep(pause_time);
    }

    printf("\n %d getpid...\n", getpid());

    if (argc > 1)
    {
        sleep(5);
        del_semvalue();
    }
    exit(EXIT_SUCCESS);
}

static int set_semvalue(void)
{
    union semun sem_union;
    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
        return (0);
    return (1);
}

static void del_semvalue(void)
{
    union semun sem_union;
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "DEL ERROR\n");
}

static int semaphore_p(void)
{
    //set struct sembuf sem_b for semop
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; //for p option
    sem_b.sem_flg = SEM_UNDO;

    if (semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "SEMAPHORE P ERROR\n");
        return (0);
    }
    return (1);
}

static int semaphore_v(void)
{
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; //for v option
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        fprintf(stderr, "SEMAPHORE V ERROR\n");
        return (0);
    }
    return (1);
}