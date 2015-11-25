CC = g++
CFLAGS = -Wall -O3 -std=c++11 -DDSFMT_MEXP=19937 -DHAVE_SSE2
LDFLAGS = -lvolk_sfmt
LDPATH =
INCPATH =

all:
	$(CC) $(CFLAGS) dsfmt_test.cc ../dsfmt_reference_impl/dSFMT.c -o dsfmt_test $(LDFLAGS) $(LDPATH) $(INCPATH)

clean:
	rm -f dsfmt_test
