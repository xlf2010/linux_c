server: test_server.o
	cc -Wall -g test_server.o -o test_server.e -lpthread

client:test_client.o
	cc -Wall -g test_client.o -o test_client.e -lpthread

teset_server.o: teset_server.c
	cc -Wall -c -g teset_server.c -o test_server.o

teset_client.o: teset_client.c
	cc -Wall -c -g teset_client.c -o teset_client.o

epoll:test_epoll.o
	cc -Wall -g test_epoll.o -o test_epoll.e

test_epoll.o:test_epoll.c
	cc -Wall -c -g test_epoll.c -o test_epoll.o

clean:
	@echo "cleaning project"
	-rm *.e *.o
	@echo "clean project finished"
