#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <cstring>
#include <chrono>


using namespace std;

#ifdef __cplusplus
extern "C" {
#endif
  void generatePrefixSumData (int* arr, size_t n);
  void checkPrefixSumResult (int* arr, size_t n);
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

  if (argc < 3) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }


  int n = atoi(argv[1]);
  int nbThreads = atoi(argv[2]);
  int * arr = new int [n];
  generatePrefixSumData (arr, n);
  // for(int i = 0; i < n; i++){
  //   arr[i] = i;
  // }

  int * pr = new int [n+1];

  //insert prefix sum code here
  int* suma = new int[nbThreads];
  suma[0] = 0;

  int size_chunk = n / nbThreads;
  int rem = n % nbThreads;

  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  #pragma omp parallel num_threads(nbThreads)
    {
        int id = omp_get_thread_num();
        int start = id * size_chunk;
        int end = start + size_chunk;
        if (id == nbThreads - 1) {
            end += rem;
        }

        int partial_sum = 0;
        for (int i = start; i < end; i++) {
            partial_sum += arr[i];
            pr[i + 1] = partial_sum;
        }
        suma[id + 1] = partial_sum;

        #pragma omp barrier
        int sum = 0;
        for(int i = 0; i < id + 1; i++) {
            sum += suma[i];
        }
        for (int i = start; i < end; i++) {
            pr[i + 1] += sum;
        }

        #pragma omp barrier
    }
  
  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;
  //Print the total execution time (in sec) to the error stream
  cerr<<elapased_seconds.count()<<std::endl;

  checkPrefixSumResult(pr, n);

  delete[] arr;
  delete[] suma;

  return 0;
}
