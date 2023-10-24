CFLAGS=-O3 -std=c11 -fPIC -g -fopenmp
CXXFLAGS=-O3 -std=c++11 -fPIC -g -fopenmp
LDFLAGS=-fopenmp
ARCHIVES=../libgen.a 
LD=g++


all: mergesort 


mergesort: mergesort-task.o
	$(LD) $(LDFLAGS) mergesort-task.o $(ARCHIVES) -o mergesort-task

test: mergesort
	./test.sh

bench: mergesort
	./queue.sh

plot:  
	./plot.sh

clean:
	-rm *.o
	-rm mergesort-task

distclean:
	-rm *.sh.*


.PHONY:  mergesort-task
