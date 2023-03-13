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

const int size = 200;

int main(int argc, char *argv[]) {
    char fd;
    int fd_fifo;
    size_t size1;
    char share[size + 1];
    int numbers[10] = {0};
    char name[] = "aaa.fifo";
    int shm_id;
    int *sharemem;

    (void)umask(0);
    mknod(name, S_IFIFO|0666, 0);

    shm_id = shmget (0x2FF, getpagesize(), 0666 | IPC_CREAT);

    if (shm_id < 0) {
        perror("shmget()");
        exit(1);
    }

    sharemem = (int *)shmat(shm_id, 0, 0);

    if (sharemem == NULL) {
        perror("shmat()");
        exit(2);
    }

    while (*sharemem != 2) {
        if ((fd_fifo = open(name, O_RDWR)) < 0){
            printf("Can\'t open FIFO for reading\n");
            exit(-1);
        }
        while (*sharemem != 0) {
            if (*sharemem == 2) {
                break;
            }
            sleep(1);
        }
        if (*sharemem == 2) {
            break;
        }
        size1 = read(fd_fifo, share, sizeof(share));
        close(fd_fifo);

        for (int i = 0; i < size1; ++i) {
            char c = share[i];
            if ('0' <= c && c <= '9') {
                int a = c - '0';
                numbers[a]++;
            }
        }
        *sharemem = 1;
    }
    
    char buffer [5000];
    if ((fd_fifo = open(name, O_WRONLY|O_TRUNC)) < 0){
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    for (int i = 0; i < 10; ++i) {
        int n = sprintf (buffer, "the number of %d is %d\n", i, numbers[i]);
        size1 = write(fd_fifo, buffer, strlen(buffer));
    }
    close(fd_fifo);

    *sharemem = 3;

    return 0;
}