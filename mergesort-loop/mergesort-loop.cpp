#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <omp.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

void merge(int* arr, int l, int m, int r) {
  int i, j, k;
  int n1 = m - l + 1; // size of left subarray
  int n2 =  r - m; // size of right subarray

  /* create temp arrays */
  int* L = new int[n1];
  int* R = new int[n2];

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < n1; i++) {
    L[i] = arr[l + i];
  }
  for (j = 0; j < n2; j++) {
    R[j] = arr[m + 1+ j];
  }

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l; // Initial index of merged subarray
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k++] = L[i++];
    }
    else {
      arr[k++] = R[j++];
    }
  }

}

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

  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int numThreads = atoi(argv[2]);

  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);


  //insert sorting code here.
  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  // parallel merge sort
  int i, j;
  int chunk = n / numThreads;
  int left, mid, right;
  int tid;
  #pragma omp for schedule(static, chunk) private(i, j, left, mid, right, tid)
  for (i = 1; i <= n - 1; i = 2 * i) {
    for (j = 0; j < n - 1; j += 2 * i) {
      left = j;
      mid = j + i - 1;
      right = std::min(j + 2 * i - 1, n - 1);
      merge(arr, left, mid, right);
    }
  }


  //printArray(arr, n);
  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;
  //Print the total execution time (in sec) to the error stream
  std::cerr<<elapased_seconds.count()<<std::endl;
  
  checkMergeSortResult (arr, n);

  delete[] arr;

  return 0;
}
