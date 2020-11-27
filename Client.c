#include "Lpc.h"
#include "LpcProxy.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

//SeekFile로 자리 바꾼다음 WriteFile로 원래 크기보다 더 큰값 쓰면 에러
int main(void) {

    char buf[11];
    memset(buf, 0x00, 11);
    int fd = OpenFile("text.txt", O_CREAT | O_RDWR);

    //int r = ReadFile(fd, buf, 10);
    //printf("read %s, %d", buf, r);

   // WriteFile(fd, "therealshitclass", 10);

    //SeekFile(fd, 1, SEEK_CUR);

    //WriteFile(fd, "fuck", 5);

    //printf("close : %d", CloseFile(fd));
    MakeDirectory("hi", 0777);
    return 0;
}
