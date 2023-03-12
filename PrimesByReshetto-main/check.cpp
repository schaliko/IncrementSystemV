#include <iostream>
#include <vector>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(1);
    }

    int N = std::stoi(argv[1]);

    // creating the key
    key_t key = ftok("file.txt", 'R');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // geting identity
    int shmid = shmget(key, sizeof(int), 0666);
    if (shmid == -1) {
        perror("shmget");
	exit(1);
    }

    // attachinh the segment
    int *shm = (int*) shmat(shmid, nullptr, 0);
    if (shm == (int*) -1) {
        perror("shmat");
        exit(1);
    }

    // using Reshetto for checking the N
//    std::vector<int> primes;
//    std::vector<bool> is_prime(N + 1, true);
//    for (int i = 2; i <= N; i++) {
//        if (is_prime[i]) {
//            primes.push_back(i);
//            for (int j = i * i; j <= N; j += i) {
//                is_prime[j] = false;
//            }
//        }
//   }


    // check if the number is prime
    if (N < 2 || N > 300) {
        std::cerr << "Number out of range" << std::endl;
    } else if (shm[N-1] == 0) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
    }

    // unlinking
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(1);
    }

    return 0;
}
