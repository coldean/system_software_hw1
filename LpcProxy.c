#include "LpcProxy.h"
#include "Lpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0600

void Init(void) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);
}

int OpenFile(char *path, int flags) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    long cpid = getpid();

    LpcRequest lpcRequest;
    LpcArg lpcArg0, lpcArg1;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    lpcArg0.argSize = sizeof(path);
    strcpy(lpcArg0.argData, path);
    lpcArg1.argSize = 4;
    // strcpy(lpcArg1.argData, flags);
    lpcArg1.argData[0] = flags;
    sprintf(lpcArg1.argData, "%d", flags);

    lpcRequest.pid = cpid;
    lpcRequest.service = LPC_OPEN_FILE;
    lpcRequest.numArg = 2;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;

    //printf("\n\nlpcArg0's data : %s, %ld\n\n", lpcRequest.lpcArgs[0].argData,
     //      lpcRequest.pid);

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);
    //printf("sended open\n");
    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));
    //printf("waiting\n");

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), cpid, 0);
    //printf("%c\n", lpcResponse.responseData[0]);
    int a = atoi(lpcResponse.responseData);
    //printf("open seccess %d\n", a);
    //fflush(stdout);
    return a;
}

int ReadFile(int fd, void *pBuf, int size) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0, lpcArg1;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    lpcArg0.argSize = 4;
    // strcpy(lpcArg0.argData, fd);
    lpcArg0.argData[0] = fd;
    sprintf(lpcArg0.argData, "%d", fd);
    lpcArg1.argSize = 4;
    // strcpy(lpcArg1.argData, size);
    lpcArg1.argData[0] = size;
    sprintf(lpcArg1.argData, "%d", size);

    //printf("ssize : %c, sssize : %s", lpcArg0.argData[0], lpcArg1.argData);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_READ_FILE;
    lpcRequest.numArg = 2;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;

    //printf("size: %c", lpcRequest.lpcArgs[1].argData[0]);

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);

    //printf("lpcResponse.responseData : %s\n", lpcResponse.responseData);
    strcpy(pBuf, lpcResponse.responseData);

    //printf("read succecss, %d\n", lpcResponse.responseSize);
    return lpcResponse.responseSize;
}

int WriteFile(int fd, void *pBuf, int size) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0, lpcArg1, lpcArg2;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    lpcArg0.argSize = 4;
    lpcArg0.argData[0] = fd;
    sprintf(lpcArg0.argData, "%d", fd);
    lpcArg1.argSize = sizeof(pBuf);
    strcpy(lpcArg1.argData, pBuf);
    lpcArg2.argSize = 4;
    lpcArg2.argData[0] = size;
    sprintf(lpcArg2.argData, "%d", size);

    //printf("bufsize : %d, string : %s\n", lpcArg1.argSize, lpcArg0.argData);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_WRITE_FILE;
    lpcRequest.numArg = 3;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;
    lpcRequest.lpcArgs[2] = lpcArg2;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);
    return lpcResponse.responseSize;
}

off_t SeekFile(int fd, off_t offset, int whence) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0, lpcArg1, lpcArg2;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    //printf("fd : %d\n", fd);

    lpcArg0.argSize = 4;
    lpcArg0.argData[0] = fd;
    sprintf(lpcArg0.argData, "%d", fd);
    lpcArg1.argSize = sizeof(off_t);
    lpcArg1.argData[0] = offset;
    sprintf(lpcArg1.argData, "%ld", offset);
    lpcArg2.argSize = 4;
    lpcArg2.argData[0] = whence;
    sprintf(lpcArg2.argData, "%d", whence);

    //printf("%s, %d\n", lpcArg0.argData, lpcArg1.argSize);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_SEEK_FILE;
    lpcRequest.numArg = 3;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;
    lpcRequest.lpcArgs[2] = lpcArg2;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);
    return lpcResponse.responseSize;
}

int CloseFile(int fd) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    lpcArg0.argSize = 4;
    lpcArg0.argData[0] = fd;
    sprintf(lpcArg0.argData, "%d", fd);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_CLOSE_FILE;
    lpcRequest.numArg = 1;
    lpcRequest.lpcArgs[0] = lpcArg0;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);

    return atoi(lpcResponse.responseData);
}

int MakeDirectory(char *path, int mode) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0, lpcArg1;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

   // printf("mode: %d\n", mode);

    lpcArg0.argSize = sizeof(path);
    strcpy(lpcArg0.argData, path);
    lpcArg1.argSize = 4;
    lpcArg1.argData[0] = mode;
    sprintf(lpcArg1.argData, "%d", mode);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_MAKE_DIRECTORY;
    lpcRequest.numArg = 2;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);
    //printf("sended messege\n");

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);
    //printf("receieved messege");
    return atoi(lpcResponse.responseData);
}

int RemoveDirectory(char *path) {
    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);

    LpcRequest lpcRequest;
    LpcArg lpcArg0;

    memset(&lpcRequest, 0x00, sizeof(lpcRequest));

    lpcArg0.argSize = sizeof(path);
    strcpy(lpcArg0.argData, path);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_REMOVE_DIRECTORY;
    lpcRequest.numArg = 1;
    lpcRequest.lpcArgs[0] = lpcArg0;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);
    //printf("sended messege\n");

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);

    return atoi(lpcResponse.responseData);
}
