CFLAGS   = -I./backchannel_functions -I./utils -o2 -Wall

all: lib backchannel

backchannel: backchannel.c backchannel.h lin_eq_solver.o
	@echo Making backchannel...
	gcc backchannel.c $(CFLAGS) -o backchannel lin_eq_solver.o
	@echo Tidying up...
	rm -rf *~
	
lib: utils/lin_eq_solver.c utils/lin_eq_solver.h
	@echo Making libraries...
	gcc -I./lin_eq_solver -o2 -c -o lin_eq_solver.o utils/lin_eq_solver.c

clean:
	@echo Cleaning up...
	rm -rf *~ *.o backchannel *tmpdatafile*
