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

const int size = 5001;

int main(int argc, char *argv[]) {
    char fd, fd1;
    int fd_fifo;
    ssize_t read_bytes;
    size_t size1;
    char share[size + 1];
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
        share[read_bytes] = '\0';
    } while (read_bytes == size);

    if ((fd_fifo = open(name, O_WRONLY|O_TRUNC)) < 0){
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    size1 = write(fd_fifo, share, read_bytes);

    if (close(fd) < 0) {
        printf("Can\'t close reading file\n");
    }
    close(fd_fifo);

    *sharemem = 0;

    // writing into file
    while (*sharemem != 1) {
        sleep(1);
    }
    if ((fd_fifo = open(name, O_RDONLY)) < 0){
        printf("Can\'t open FIFO for reading\n");
        exit(-1);
    }
    size1 = read(fd_fifo, share, sizeof(share));

    if ((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
        printf("Can\'t open writing file\n");
        exit(-1);
    }

    write(fd1, share, size1);
    
    if (close(fd1) < 0) {
        printf("Can\'t close writing file\n");
    }
    close(fd_fifo);

    shmdt (sharemem);

    return 0;
}