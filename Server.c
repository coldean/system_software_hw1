#include "Lpc.h"
#include "LpcStub.h"
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define PERMS 0600

int id_send = 0;
int id_receive = 0;

void signalHandler(int signum);

int main(void) {

    key_t mykey_client = 1111;
    key_t mykey_server = 2222;
    int msqid_client = msgget(mykey_client, IPC_CREAT | PERMS);
    int msqid_server = msgget(mykey_server, IPC_CREAT | PERMS);
    id_send = msqid_client;
    id_receive = msqid_server;

    long clientPid;
    LpcService lpcService;
    int numArg;
    LpcArg *lpcArgs;

    LpcRequest *lpcRequest;
    lpcRequest = malloc(sizeof(LpcRequest));

    signal(SIGINT, signalHandler);

    while (1) {
        memset(lpcRequest, 0x00, sizeof(LpcRequest));
        // memset(&lpcRequest->numArg, 0x00, sizeof(lpcRequest->numArg));
        msgrcv(msqid_client, lpcRequest, sizeof(LpcRequest), 0, 0);

        clientPid = lpcRequest->pid;
        lpcService = lpcRequest->service;
        numArg = lpcRequest->numArg;
        lpcArgs = lpcRequest->lpcArgs;

        int fd, size, a;
        printf("cilent's pid : %ld\n", clientPid);

        switch (lpcService) {
        case LPC_OPEN_FILE:
            fd = OpenFile(lpcRequest->lpcArgs[0].argData,
                          atoi(lpcRequest->lpcArgs[1].argData), clientPid);
            fflush(stdout);
            break;
        case LPC_READ_FILE:
            a = atoi(lpcRequest->lpcArgs[1].argData);
            size = ReadFile(atoi(lpcRequest->lpcArgs[0].argData),
                            atoi(lpcRequest->lpcArgs[1].argData), clientPid);
            break;
        case LPC_WRITE_FILE:
            WriteFile(atoi(lpcRequest->lpcArgs[0].argData),
                      lpcRequest->lpcArgs[1].argData,
                      atoi(lpcRequest->lpcArgs[2].argData), clientPid);
            break;
        case LPC_SEEK_FILE:
            SeekFile(atoi(lpcRequest->lpcArgs[0].argData),
                     atoi(lpcRequest->lpcArgs[1].argData),
                     atoi(lpcRequest->lpcArgs[2].argData), clientPid);
            break;
        case LPC_CLOSE_FILE:
            CloseFile(atoi(lpcRequest->lpcArgs[0].argData), clientPid);
            break;
        case LPC_MAKE_DIRECTORY:
            MakeDirectory(lpcRequest->lpcArgs[0].argData,
                          atoi(lpcRequest->lpcArgs[1].argData), clientPid);
            break;
        case LPC_REMOVE_DIRECTORY:
            RemoveDirectory(lpcRequest->lpcArgs[0].argData, clientPid);
            break;
        }
    }

    return 0;
}

void signalHandler(int signum) {
    if (signum == SIGINT) {
        msgctl(id_send, IPC_RMID, NULL);
        msgctl(id_receive, IPC_RMID, NULL);
        exit(0);
    }
}
