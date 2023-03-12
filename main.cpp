#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#define NUM_INCREMENTS 10000


union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void error(const char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    key_t key1, key2;
    int semid, semval;
    int EXIT_STATUS1, EXIT_STATUS2;
    struct sembuf semops;
    union semun semarg;
    pid_t pid1, pid2;


    // create a unique key for the semaphore set
    if ((key2 = ftok(".", 'N')) == -1) {
        error("ftok");
    }

    // create a shared memory segment
    int shmid = shmget(key2, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        error("shmget");
    }

    //attach the shm
    int* shared_var = (int*) shmat(shmid, NULL, 0);
    if (shared_var == (int*) -1) {
        error("shmat");
    }


    // initilize the shm
    *shared_var = 0;

    // create a unique key for the semaphore set
    if ((key1 = ftok(".", 'S')) == -1) {
        error("ftok");
    }

    // create a semaphore set with 1 semaphore
    if ((semid = semget(key1, 1, 0666 | IPC_CREAT)) == -1) {
        error("semget");
    }

    // initialize the semaphore value to 1
    semarg.val = 1;
    if (semctl(semid, 0, SETVAL, semarg) == -1) {
        error("semctl");
    }

    //create 2 proccesses
    (pid1 = fork()) && (pid2 = fork());

    if(pid1 < 0 || pid2 < 0){
        error("fork");
    }

    if(pid1 > 0 && pid2 > 0){
     
        wait(&EXIT_STATUS1);
        wait(&EXIT_STATUS2);

        std::cout << *shared_var << std::endl;
    
    }else if(pid1 == 0){

                // acquire the semaphore
                semops.sem_num = 0;
                semops.sem_op = -1;
                semops.sem_flg = 0;
                if (semop(semid, &semops, 1) == -1) {
                    error("semop");
                    return 1;
                }

        for (int j = 0; j < NUM_INCREMENTS; j++) {
                (*shared_var)++;

            }

                // release the semaphore
                semops.sem_num = 0;
                semops.sem_op = 1;
                semops.sem_flg = 0;
                if (semop(semid, &semops, 1) == -1) {
                    error("semop");
                    return 1;
                }

            return 0;
        }
    else if(pid2 == 0){
                // acquire the semaphore
                semops.sem_num = 0;
                semops.sem_op = -1;
                semops.sem_flg = 0;
                if (semop(semid, &semops, 1) == -1) {
                    error("semop");
                    return 1;
                }

          for (int j = 0; j < NUM_INCREMENTS; j++) {
                (*shared_var)++;
            }

                // release the semaphore
                semops.sem_num = 0;
                semops.sem_op = 1;
                semops.sem_flg = 0;
                if (semop(semid, &semops, 1) == -1) {
                    error("semop");
                    return 1;
                }

            return 0;
    }

    // detach the shm
    if(shmdt(shared_var) == -1){
        error("shmdt");
    }

    // remove the shm
    if(shmctl(shmid, IPC_RMID, NULL) < 0){
        error("shmctl");
    }
    
    // remove the semaphore set
    if (semctl(semid, 0, IPC_RMID, semarg) == -1) {
        error("semctl");
    }

    return 0;
}
