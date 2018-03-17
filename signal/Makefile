alarm:test_sig_alarm.o
	gcc -g test_sig_alarm.o -o test_sig_alarm.e
test_sig_alarm.o:test_sig_alarm.c
	gcc -Wall -c -g test_sig_alarm.c -o test_sig_alarm.o

init:test_sig_init.o
	gcc -g test_sig_init.o -o test_sig_init.e
test_sig_init.o:test_sig_init.c
	gcc -Wall -c -g test_sig_init.c -o test_sig_init.o

clean:
	@echo "cleaning project"
	-rm *.e *.o
	@echo "clean project finished"

