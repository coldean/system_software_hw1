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
    lpcArg1.argData[0] = flags;
    sprintf(lpcArg1.argData, "%d", flags);

    lpcRequest.pid = cpid;
    lpcRequest.service = LPC_OPEN_FILE;
    lpcRequest.numArg = 2;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);
    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), cpid, 0);
    int a = atoi(lpcResponse.responseData);
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
    lpcArg0.argData[0] = fd;
    sprintf(lpcArg0.argData, "%d", fd);
    lpcArg1.argSize = 4;
    lpcArg1.argData[0] = size;
    sprintf(lpcArg1.argData, "%d", size);

    lpcRequest.pid = getpid();
    lpcRequest.service = LPC_READ_FILE;
    lpcRequest.numArg = 2;
    lpcRequest.lpcArgs[0] = lpcArg0;
    lpcRequest.lpcArgs[1] = lpcArg1;

    msgsnd(msqid_client, &lpcRequest, sizeof(lpcRequest), 0);

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);

    strcpy(pBuf, lpcResponse.responseData);

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
    return atoi(lpcResponse.responseData);
}

off_t SeekFile(int fd, off_t offset, int whence) {
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
    lpcArg1.argSize = sizeof(off_t);
    lpcArg1.argData[0] = offset;
    sprintf(lpcArg1.argData, "%ld", offset);
    lpcArg2.argSize = 4;
    lpcArg2.argData[0] = whence;
    sprintf(lpcArg2.argData, "%d", whence);

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
    int a = atoi(lpcResponse.responseData);
    return (off_t)a;
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

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);
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

    LpcResponse lpcResponse;
    memset(&lpcResponse, 0x00, sizeof(LpcResponse));

    msgrcv(msqid_server, &lpcResponse, sizeof(LpcResponse), getpid(), 0);

    return atoi(lpcResponse.responseData);
}
