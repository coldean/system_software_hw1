// msgrcv는 server.에서
//여기서는 client에 msgsnd를 해야 함

#include "LpcStub.h"
#include "Lpc.h"
#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define PERMS 0600

void Init(void) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);
}

int OpenFile(char *path, int flag, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);
    int fd = open(path, flag, 0666);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 4;
    lpcResponse.responseData[0] = fd;
    sprintf(lpcResponse.responseData, "%d", fd);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);
    return fd;
}

int ReadFile(int fd, int readCount, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    char buf[LPC_DATA_MAX + 1];
    memset(buf, 0x00, LPC_DATA_MAX + 1);

    int rsize = read(fd, buf, readCount);

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = rsize;
    strcpy(lpcResponse.responseData, buf);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return rsize;
}

int WriteFile(int fd, char *pBuf, int writeCount, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    int wsize = write(fd, pBuf, writeCount);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 4;
    lpcResponse.responseData[0] = wsize;
    sprintf(lpcResponse.responseData, "%d", wsize);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return wsize;
}

off_t SeekFile(int fd, off_t offset, int whence, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    off_t offs = lseek(fd, (off_t)offset, whence);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 8;
    lpcResponse.responseData[0] = offs;
    sprintf(lpcResponse.responseData, "%ld", offs);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return offs;
}

int CloseFile(int fd, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    int k = close(fd);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 4;
    lpcResponse.responseData[0] = k;
    sprintf(lpcResponse.responseData, "%d", k);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return k;
}

int MakeDirectory(char *path, int mode, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    int k = mkdir(path, mode);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 4;
    lpcResponse.responseData[0] = k;
    sprintf(lpcResponse.responseData, "%d", k);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return k;
}

int RemoveDirectory(char *path, int clientPid) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    int k = rmdir(path);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    lpcResponse.pid = clientPid;
    lpcResponse.errorno = 0;
    lpcResponse.responseSize = 4;
    lpcResponse.responseData[0] = k;
    sprintf(lpcResponse.responseData, "%d", k);

    msgsnd(msqid_server, &lpcResponse, sizeof(lpcResponse), 0);

    return k;
}
