CFLAGS   = -I./backchannel_functions -o2 -Wall

all: backchannel

backchannel: backchannel.c backchannel.h
	@echo Making backchannel...
	gcc backchannel.c $(CFLAGS) -o backchannel
	@echo Tidying up...
	rm -rf *~

clean:
	@echo Cleaning up...
	rm -rf *~ *.o backchannel *tmpdatafile*
