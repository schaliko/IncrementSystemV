#include <iostream>
#include <cstring>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

#define SHM_SIZE 300

int main() {
    char *shm;

    // creating the key from the filename
    key_t key = ftok("file.txt", 'R');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // get identity from file's key
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL | 0666);
    if (shmid == -1) {
	if(errno != EEXIST){
		std::cerr << "shmget: share memory segment already exists" << std::endl;
	}else{
        	perror("shmget");
	}
	exit(1);
    }

    // shared memory attach
    shm = (char *)shmat(shmid, nullptr, 0);
    if (shm == (char *) -1) {
        perror("shmat");
        exit(1);
    }

    // filling with nulls
    for (int i = 0; i < SHM_SIZE; i++) {
        shm[i] = 0;
    }

    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
