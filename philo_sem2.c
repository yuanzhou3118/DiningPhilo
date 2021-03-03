#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>

// gcc -std=c99 -Wall -pthread -lrt -o philo_sem2 philo_sem2.c

#define NUM_PHILO 5
#define EOL "\033[0m \n"

#define RIGHT(_n) ((_n + 1) % NUM_PHILO)
#define LEFT(_n) (_n)

#define SHARED_THREADS 0
#define SHARED_PROCS   1

pthread_t philosopher[NUM_PHILO];
sem_t chopstick[NUM_PHILO];
char colors[NUM_PHILO][10] = {{0}};
int times_eating[NUM_PHILO] = {0};

void *eat_or_think(void *);
void signal_handler(int);

void
signal_handler(int sig)
{
    int i = 0;

    for (i = 0; i < NUM_PHILO; i++){
        pthread_cancel(philosopher[i]);
    }
}

void *
eat_or_think(void *arg)
{
    int n = (int) ((long) arg);

    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    for(;;) {
        printf("%sPhilosopher %d is thinking" EOL, colors[n], n);

        if ((n % 2) == 0) {
            printf("%sPhilosopher %d wants chopstick %d" EOL, colors[n], n, RIGHT(n));
            sem_wait(&chopstick[RIGHT(n)]);
            printf(" %s+ Philosopher %d got chopstick %d" EOL, colors[n], n, RIGHT(n));
            printf("   %sPhilosopher %d wants chopstick %d" EOL, colors[n], n, LEFT(n));
            sem_wait(&chopstick[LEFT(n)]);
            printf("     %s+ Philosopher %d got chopstick %d" EOL, colors[n], n, LEFT(n));
        } else {
            printf("%sPhilosopher %d wants chopstick %d" EOL, colors[n], n, LEFT(n));
            sem_wait(&chopstick[LEFT(n)]);
            printf(" %s+ Philosopher %d got chopstick %d" EOL, colors[n], n, LEFT(n));
            printf("   %sPhilosopher %d wants chopstick %d" EOL, colors[n], n, RIGHT(n));
            sem_wait(&chopstick[RIGHT(n)]);
            printf("     %s+ Philosopher %d got chopstick %d" EOL, colors[n], n, RIGHT(n));
        }

        printf("        %sPhilosopher %d is eating" EOL, colors[n], n);
        sleep(1);
        times_eating[n]++;
        printf("        %sPhilosopher %d done eating" EOL, colors[n], n);

        printf("     %s- Philosopher %d set down chopstick %d" EOL, colors[n], n, RIGHT(n));
        sem_post(&chopstick[RIGHT(n)]);
        printf(" %s- Philosopher %d set down chopstick %d" EOL, colors[n], n, LEFT(n));
        sem_post(&chopstick[LEFT(n)]);
    }
    pthread_exit(NULL);
}

int
main(int argc, const char *argv[])
{
    int i = 0;

    signal(SIGINT, signal_handler);
    for(i = 0; i < NUM_PHILO; i++) {
        sem_init(&chopstick[i], SHARED_THREADS, 1);
        sprintf(colors[i], "\033[%dm", i + 31);
    }
    for(i = 0; i < NUM_PHILO; i++) {
        pthread_create(&philosopher[i], NULL, eat_or_think, (void *)(size_t) i);
    }
    for(i = 0; i < NUM_PHILO; ++i) {
        pthread_join(philosopher[i], NULL);
    }
    printf("\n");
    for(i = 0; i < NUM_PHILO; ++i) {
        printf("%sPhilosopher %d ate %d times" EOL, colors[i], i, times_eating[i]);
    }
    pthread_exit(NULL);
    return EXIT_SUCCESS;
}