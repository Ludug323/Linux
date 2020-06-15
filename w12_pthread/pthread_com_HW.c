/*
 *  Solution to Producer Consumer Problem
 *  Using Ptheads, a mutex and condition variables
 *  From Tanenbaum, Modern Operating Systems, 3rd Ed.
 */

/*
    In this version the buffer is a single number.
    The producer is putting numbers into the shared buffer
    (in this case sequentially)
    And the consumer is taking them out.
    If the buffer contains zero, that indicates that the buffer is empty.
    Any other value is valid.
*/

#include <stdio.h>
#include <pthread.h>
#define SIZE (5)
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer = 0;
int count = 0;

void *producer(void *ptr)
{
    int i;
    int num = (long)ptr;
    int start = num * 5;
    int end = start + 5;

    for (i = start + 1; i <= end; i++)
    {
        pthread_mutex_lock(&the_mutex); /* protect buffer */
        while (count != 0)              /* If there is something 
					  in the buffer then wait */
            pthread_cond_wait(&condp, &the_mutex);
        buffer = i;
        //printf("producer = %d\n", buffer[count]);
        count++;
        pthread_cond_signal(&condc);      /* wake up consumer */
        pthread_mutex_unlock(&the_mutex); /* release the buffer */
    }
    pthread_exit(0);
}

void *consumer(void *ptr)
{
    int i;
    int num = (long)ptr;
    int start = num * 5;
    int end = start + 5;
    for (i = start + 1; i <= end; i++)
    {
        pthread_mutex_lock(&the_mutex); /* protect buffer */

        while (count == 0) /* If there is nothing in 
					   the buffer then wait */
            pthread_cond_wait(&condc, &the_mutex);
        printf("ConBuffer:%2d\n", buffer);
        buffer = 0;
        count--;
        pthread_cond_signal(&condp);      /* wake up producer */
        pthread_mutex_unlock(&the_mutex); /* release the buffer */
    }
    pthread_exit(0);
}

int main(int argc, char **argv)
{
    pthread_t pro[3], con[3];
    int i;
    // Initialize the mutex and condition variables
    /* What's the NULL for ??? */
    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&condc, NULL); /* Initialize consumer condition variable */
    pthread_cond_init(&condp, NULL); /* Initialize producer condition variable */

    // Create the threadsx

    for (i = 0; i < 3; i++)
    {
        pthread_create(&con[i], NULL, producer, (void *)(long)i);
        pthread_create(&pro[i], NULL, consumer, (void *)(long)i);
    }
    // Wait for the threads to finish
    // Otherwise main might run to the end
    // and kill the entire process when it exits.
    for (i = 0; i < 3; i++)
    {
        pthread_join(con[i], NULL);
    }
    for (i = 0; i < 3; i++)
        pthread_join(pro[i], NULL);

    // Cleanup -- would happen automatically at end of program
    pthread_mutex_destroy(&the_mutex); /* Free up the_mutex */
    pthread_cond_destroy(&condc);      /* Free up consumer condition variable */
    pthread_cond_destroy(&condp);      /* Free up producer condition variable */
}
