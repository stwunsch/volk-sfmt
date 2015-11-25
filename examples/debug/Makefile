CC = g++
CFLAGS = -Wall -O3 -std=c++11 -DDSFMT_MEXP=19937#-DHAVE_SSE2
LDFLAGS =
LDPATH =
INCPATH =

all:
	$(CC) $(CFLAGS) debug.cc ../dsfmt_reference_impl/dSFMT.c -o debug $(LDFLAGS) $(LDPATH) $(INCPATH)

clean:
	rm -f debug
