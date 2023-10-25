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

void merge(int* arr, int* tmp, int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;


    // Copy data to temporary arrays L[] and R[]
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary arrays back into arr[]
    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int* arr, int* tmp, int left, int right, int numThreads) {
  if (left < right) {
      int mid = left + (right - left) / 2;

      if (numThreads == 1) {
          // Serial merge sort if there's only one thread.
          mergeSort(arr, tmp, left, mid, numThreads);
          mergeSort(arr, tmp, mid + 1, right, numThreads);
          merge(arr, tmp, left, mid, right);

      } else {
          // Parallel merge sort using OpenMP tasks.
          #pragma omp task shared(arr, tmp) // if (numThreads > 1)
            mergeSort(arr, tmp, left, mid, numThreads / 2);
          
          #pragma omp task shared(arr, tmp) // if (numThreads > 1)
            mergeSort(arr, tmp, mid + 1, right, numThreads / 2);
          
          #pragma omp taskwait
            merge(arr, tmp, left, mid, right);
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
  #pragma omp parallel num_threads(numThreads)
    {
      #pragma omp single nowait
        mergeSort(arr, tmp, 0, n, numThreads);
    }
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;
  //Print the total execution time (in sec) to the error stream
  std::cerr<<elapased_seconds.count()<<std::endl;
  //printArray(arr,n);
  checkMergeSortResult (arr, n);


  delete[] arr;

  return 0;
}
