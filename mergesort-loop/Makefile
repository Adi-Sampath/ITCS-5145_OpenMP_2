CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


all: mergesort 


mergesort: mergesort-loop.o
	$(LD) $(LDFLAGS) mergesort-loop.o $(ARCHIVES) -o mergesort-loop

test: mergesort
	./test.sh

bench: mergesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm mergesort-loop

distclean:
	-rm *.sh.*


.PHONY:  mergesort-loop
