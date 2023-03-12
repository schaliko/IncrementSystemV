#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    key_t key;
    int semid, semval;
    struct sembuf semops;
    union semun semarg;

    // create a unique key for the semaphore set
    if ((key = ftok(".", 'S')) == -1) {
        error("ftok");
    }

    // create a semaphore set with 1 semaphore
    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) == -1) {
        error("semget");
    }

    // initialize the semaphore value to 0
    semarg.val = 0;
    if (semctl(semid, 0, SETVAL, semarg) == -1) {
        error("semctl");
    }

    // acquire the semaphore
    semops.sem_num = 0;
    semops.sem_op = -1;
    semops.sem_flg = 0;
    if (semop(semid, &semops, 1) == -1) {
        error("semop");
    }

    // release the semaphore
    semops.sem_num = 0;
    semops.sem_op = 1;
    semops.sem_flg = 0;
    if (semop(semid, &semops, 1) == -1) {
        error("semop");
    }

    // remove the semaphore set
    if (semctl(semid, 0, IPC_RMID, semarg) == -1) {
        error("semctl");
    }

    return 0;
}
