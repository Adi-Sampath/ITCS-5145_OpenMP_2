#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include <omp.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }

  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int numThreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];
  int * tmp = new int [n];

  generateMergeSortData (arr, n);
  //printArray(arr,n);

  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  //insert sorting code here.

  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;
  //Print the total execution time (in sec) to the error stream
  cerr<<elapased_seconds.count()<<std::endl;
  //printArray(arr,n);
  checkMergeSortResult (arr, n);


  delete[] arr;

  return 0;
}
