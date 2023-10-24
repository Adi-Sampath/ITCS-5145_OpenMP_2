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

void merge(int * arr, int l[], int r[], int l_size, int r_size) {

  int i = 0, j = 0, k = 0;

  while (i < l_size && j < r_size) {
    if (l[i] <= r[j]) {
      arr[k] = l[i];
      i++;
    } else {
        arr[k] = r[j];
        j++;
      }
      k++;
    }
  // Copy any remaining elements from l and r (if any)
  while (i < l_size) {
      arr[k] = l[i];
      i++;
      k++;
  }
  while (j < r_size) {
      arr[k] = r[j];
      j++;
      k++;
  }
}

void parallelMergeSort(int arr[], int n) {

  if(n > 1) {
    int mid = n / 2;
    int *left = arr;
    int *right = arr + mid;

    #pragma omp parallel sections
    {
      #pragma omp section
      {
        parallelMergeSort(left, mid);
      }
      #pragma omp section
      {
        parallelMergeSort(right, n - mid);
      }
    }

    // Merge the two sorted subarrays (sequentially)
    merge(arr, left, right, mid, n - mid);
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

  omp_set_num_threads(numThreads);
  parallelMergeSort(arr, n);
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
