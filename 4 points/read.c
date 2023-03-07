// Разработать программу, вычисляющую число вхождений различных цифр в заданной ASCII-строке.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>

const int size = 5000;
int *numbers;

void sys_err (char *msg) {
  puts (msg);
  exit (1);
}

void checkNumber(char c) {
    if ('0' <= c && c <= '9') {
        int a = c - '0';
        numbers[a]++;
    }
}

int main(int argc, char *argv[]) {
    int fd, fd1;
    ssize_t read_bytes;
    size_t size1;
    pid_t readf, count, out;
    char *share;
    int *share_i, *share_numb;
    int shm_id, shm_id_finish, shm_id_numb;

    shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);
    shm_id_finish = shmget (0x2FA, getpagesize(), 0666 | IPC_CREAT);
    shm_id_numb = shmget (0x2FB, getpagesize(), 0666 | IPC_CREAT);
    if (shm_id < 0 || shm_id_finish < 0 || shm_id_numb < 0) {
        perror("shmget()");
        exit(1);
    }

    share = (char *)shmat(shm_id, 0, 0);
    share_i = (int *)shmat(shm_id_finish, 0, 0);
    numbers = (int *)shmat(shm_id_numb, 0, 0);
    
    if (share == NULL || share_i == NULL || numbers == NULL) {
        perror("shmat()");
        exit(2);
    }

    readf = fork();
    if (readf == 0) {
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            printf("Can\'t open reading file\n");
            exit(-1);
        }
        do {
            read_bytes = read(fd, share, size);
            if (read_bytes == -1) {
                printf("Can\'t write this file\n");
                exit(-1);
            }
            *share_i = read_bytes;
        } while (read_bytes == size);

        if (close(fd) < 0) {
            printf("Can\'t close reading file\n");
        }
        exit(1);
    } else if (readf > 0) {
        count = fork();
        if (count == 0) {
            waitpid(readf, NULL, 0);
            for (int i = 0; i < *share_i; ++i) {
                checkNumber(share[i]);
            }
            exit(2);
        } else if (count > 0) {
            waitpid(readf, NULL, 0);
            waitpid(count, NULL, 0);
            out = fork();
            if (out == 0) {
                waitpid(count, NULL, 0);
                if ((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                    printf("Can\'t open writing file\n");
                    exit(-1);
                }

                char buffer [5000];
                for (int i = 0; i < 10; ++i) {
                    int n = sprintf (buffer, "the number of %d is %d\n", i, numbers[i]);
                    size1 = write(fd1, buffer, n);
                }
                
                memset (numbers, 0, 40);
                if (close(fd1) < 0) {
                    printf("Can\'t close writing file\n");
                }
                exit(3);
            } else if (out > 0) {
                exit(4);
            } else {
                printf("Can't create a child\n");
                exit(-1);
            }
        } else {
            printf("Can't create a child\n");
            exit(-1);
        }
    } else {
        printf("Can't create a child\n");
        exit(-1);
    }

    shmdt (share);
    shmdt (share_i);
    shmdt (numbers);

    if (shmctl (shm_id, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        sys_err ("eraser: shared memory remove error");
    } else if (shmctl (shm_id_finish, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        sys_err ("eraser: shared memory remove error");
    } else if(shmctl (shm_id_numb, IPC_RMID, (struct shmid_ds *) 0) < 0) {
        sys_err ("eraser: shared memory remove error");
    }

    return 0;
}