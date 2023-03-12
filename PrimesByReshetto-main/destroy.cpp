#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>

int main() {
    char *shm;
    const char *filepath = "file.txt";

    // creating the key
    key_t key = ftok(filepath, 'R');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // getting the key
    int shmid = shmget(key, 0, 0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    // marking the segment to be destroyed
    int marked = shmctl(shmid, IPC_RMID, nullptr);
    if (marked == -1) {
        perror("shmctl");
        exit(1);
    }

    // Удаляем временный регулярный файл
    int rmv = remove(filepath);
    if (rmv == -1) {
        perror("romove");
        exit(1);
    }

    return 0;
}
