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
    pid_t readf, count, out;
    char share[size + 1];
    int numbers[10] = {0};
    char    name[] = "aaa.fifo";

    (void)umask(0);
    mknod(name, S_IFIFO|0666, 0);

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

        count = fork();

        if (count > 0) {
            waitpid(readf, NULL, 0);
            if ((fd_fifo = open(name, O_RDWR)) < 0){
                printf("Can\'t open FIFO for reading\n");
                exit(-1);
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
            exit(0);
        } else if (count == 0) {
            exit(0);
        } else {
            printf("Can't create a child\n");
            exit(-1);
        }
    } else if (readf > 0) {
        waitpid(readf, NULL, 0);
        waitpid(count, NULL, 0);
        if ((fd_fifo = open(name, O_RDONLY)) < 0){
            printf("Can\'t open FIFO for reading\n");
            exit(-1);
        }
        size1 = read(fd_fifo, share, sizeof(share));

        if ((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
            printf("Can\'t open writing file\n");
            exit(-1);
        }

        size1 = write(fd1, share, strlen(share));
        
        memset (numbers, 0, 40);
        if (close(fd1) < 0) {
            printf("Can\'t close writing file\n");
        }
        close(fd_fifo);
        exit(0);
    } else {
        printf("Can't create a child\n");
        exit(-1);
    }

    return 0;
}