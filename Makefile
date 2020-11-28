all : client server

client: testcase.o LpcProxy.o
	gcc -o client testcase.o LpcProxy.o

testcase.o: testcase.c
	gcc -c -o testcase.o testcase.c

LpcProxy.o: LpcProxy.c
	gcc -c -o LpcProxy.o LpcProxy.c

server: Server.o LpcStub.o
	gcc -o server Server.o LpcStub.o

Server.o: Server.c
	gcc -c -o Server.o Server.c

LpcStub.o: LpcStub.c
	gcc -c -o LpcStub.o LpcStub.c

clean:
	rm -f *.o
