# detect OS
OSUPPER = $(shell uname -s 2>/dev/null | tr [:lower:] [:upper:])
OSLOWER = $(shell uname -s 2>/dev/null | tr [:upper:] [:lower:])

all:
#	gcc -g -lm *.c -o pvbandsim
	gcc -O3 -lm *.c -o pvbandsim
clean:
	rm -fr pvbandsim.exe
