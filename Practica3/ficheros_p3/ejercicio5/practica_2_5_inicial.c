#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

int main(void)
{
    int fd1,fd2,i,pos;
    char c;
    char buffer[6];
    
    // pthread_mutex_t mtx;
    // pthread_mutex_destroy(&mtx);
    // pthread_mutex_init(&mtx, NULL);

    fd1 = open("output.txt", O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
        write(fd1, "00000", 5);
        for (i=1; i < 10; i++) {
            pos = lseek(fd1, 0, SEEK_CUR);
            if (fork() == 0) {
                /* Child */
                fd2 = open("output.txt", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                sprintf(buffer, "%d", i*11111);
                lseek(fd2, pos, SEEK_SET);
                write(fd2, buffer, 5);
                close(fd2);
                exit(0);
            } else {
                /* Parent */
                lseek(fd1, 5, SEEK_CUR);
                write(fd1, "00000", 5);
            }
        }

	//wait for all children to finish
    while (wait(NULL) != -1);

    // ?? pthread_mutex_destroy(&mtx);

    lseek(fd1, 0, SEEK_SET);
    // printf("File contents are:\n");
    printf("File contents are: ");
    while (read(fd1, &c, 1) > 0)
        printf("%c", (char) c);
    printf("\n");
    close(fd1);
    exit(0);
}
