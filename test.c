#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    /*
    int fd = open("./text.txt", O_CREAT | O_RDWR);
    char *msg = "hi";
    char *msg2 = "plusssssssssssssssssss";
    write(fd, (char *)msg, 2);
    lseek(fd, 7, SEEK_CUR);
    write(fd, (char *)msg2, 15);
    close(fd);
    */
    // mkdir("one", 0777);
    int a = 0777;
    printf("%d", a);
}
