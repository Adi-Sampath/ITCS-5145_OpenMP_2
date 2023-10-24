CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


UTILDIR=util


all: prefixsum_seq mergesort_seq


# archives

libgen.a: gen_lib.o
	ar rcs libgen.a gen_lib.o

prefixsum_seq: prefixsum_seq.o
	$(LD) $(LDFLAGS) prefixsum_seq.o $(ARCHIVES) -o prefixsum_seq

mergesort_seq: mergesort_seq.o
	$(LD) $(LDFLAGS) mergesort_seq.o $(ARCHIVES) -o mergesort_seq

# run benchmarks
bench: prefixsum_seq mergesort_seq
	./queue.sh

clean:
	-rm *.o
	-rm prefixsum_seq mergesort_seq

distclean:
	-rm *.sh.*


.PHONY: prefixsum_seq mergesort_seq
