RM	:= rm -f
CC	:= gcc
FLG	:= -Wall -pedantic -Wextra -Wpointer-arith 
FLG += -Wshadow -Wfloat-equal -Waggregate-return

all : travelMonitor Monitor
	@echo "> linking finished"
	@echo "Run: ./travelMonitor -m <numMonitors> -b <bufferSize> -s <sizeOfBloom> -i <input_dir>"
	@$(RM) *.o

clean :
	@$(RM) travelMonitor Monitor 
	@echo "all cleaned"

bloom.o: src/bloom.c headers/bloom.h
	@$(CC) -c $(FLG) src/bloom.c

pipes.o: src/pipes.c headers/pipes.h
	@$(CC) -c $(FLG) src/pipes.c

lists.o: src/lists.c headers/lists.h
	@$(CC) -c $(FLG) src/lists.c

travelMonitor: bloom.o lists.o pipes.o
	@$(CC) -o travelMonitor src/travelMonitor.c bloom.o lists.o pipes.o
	@echo "- created travelMonitor"

Monitor: bloom.o lists.o pipes.o
	@$(CC) -o Monitor src/Monitor.c bloom.o lists.o pipes.o
	@echo "- created Monitor"