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

int main(int argc, char *argv[]) {
    char fd, fd1;
    int mem[2], num[2];
    ssize_t read_bytes;
    size_t size1;
    pid_t readf, count, out;
    char share[size + 1];
    int status = 0;
    int numbers[10] = {0};

    pipe(mem);
    pipe(num);
    readf = fork();
    if (readf == 0) {
        close(mem[0]);
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
        
        write(mem[1], share, size);

        if (close(fd) < 0) {
            printf("Can\'t close reading file\n");
        }
        exit(0);
    } else if (readf > 0) {

        count = fork();

        if (count == 0) {
            wait(&status);
            close(mem[1]);

            int nbytes = read(mem[0], share, sizeof(share));

            for (int i = 0; i < strlen(share); ++i) {
                char c = share[i];
                if ('0' <= c && c <= '9') {
                    int a = c - '0';
                    numbers[a]++;
                }
            }

            close(num[0]);
            char buffer [5000];

            for (int i = 0; i < 10; ++i) {
                int n = sprintf (buffer, "the number of %d is %d\n", i, numbers[i]);
                size1 = write(num[1], buffer, n);
            }

            exit(0);
        } else if (count > 0) {

            out = fork();

            if (out == 0) {
                close(num[1]);
                int nbytes = read(num[0], share, sizeof(share));

                if ((fd1 = open(argv[2], O_WRONLY | O_CREAT, 0666)) < 0) {
                    printf("Can\'t open writing file\n");
                    exit(-1);
                }

                size1 = write(fd1, share, strlen(share));
                
                memset (numbers, 0, 40);
                if (close(fd1) < 0) {
                    printf("Can\'t close writing file\n");
                }
                exit(0);
            } else if (out > 0) {
                exit(0);
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

    return 0;
}