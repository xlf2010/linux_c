main: test_server.o test_client.o
	cc -g test_server.o -o test_server.e -lpthread
	cc -g test_client.o -o test_client.e -lpthread

teset_server.o: teset_server.c
	cc -c -g teset_server.c -o test_server.o

teset_client.o: teset_client.c
	cc -c -g teset_client.c -o teset_client.o

clean:
	@echo "cleaning project"
	-rm *.e *.o
	@echo "clean project finished"
