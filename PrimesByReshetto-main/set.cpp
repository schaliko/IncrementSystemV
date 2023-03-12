#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>

const int N = 300;

int main()
{
    // creating the key
    key_t key = ftok("file.txt", 'R');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // getting the identity
    int shmid = shmget(key, sizeof(int),  0666);
    if (shmid == -1) {
        if (errno == EEXIST) {
            std::cerr << "Segment already exists" << std::endl;
        } else {
            perror("shmget");
        }
        exit(1);
    }

    // attach the segment
    int *numbers = (int*) shmat(shmid, NULL, 0);
    if (numbers == (int*)-1) {
        perror("shmat");
        exit(1);
    }

    // filling with nulls
    for (int i = 0; i < N; i++) {
        numbers[i] = 0;
    }

    // creating resheto
    for (int i = 2; i * i <= N; i++) {
        if (numbers[i-1] == 0) {
            for (int j = i*i; j <= N; j += i) {
                numbers[j-1] = 1;
            }
        }
    }

    // unlink
    if (shmdt(numbers) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
