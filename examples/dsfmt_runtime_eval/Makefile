CC = g++
CFLAGS = -Wall -O3 -std=c++11 -DDSFMT_MEXP=19937 -DHAVE_SSE2
LDFLAGS = -lvolk_sfmt
LDPATH =
INCPATH =

all:
	$(CC) $(CFLAGS) runtime.cc ../dsfmt_reference_impl/dSFMT.c -o runtime $(LDFLAGS) $(LDPATH) $(INCPATH)

clean:
	rm -f runtime
