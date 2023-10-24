#include <iostream>
#include <omp.h>
#include <chrono>
#include <cstdlib>

#ifdef __cplusplus
extern "C" {
#endif
void generateMergeSortData(int* arr, size_t n);
void checkMergeSortResult(int* arr, size_t n);
#ifdef __cplusplus
}
#endif

void merge(int* arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
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

    delete[] L;
    delete[] R;
}

void parallelMergeSort(int* arr, int n, int numThreads) {
    int curr_size;

    for (curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        #pragma omp parallel for num_threads(numThreads)
        for (int left = 0; left < n - 1; left += 2 * curr_size) {
            int mid = std::min(left + curr_size - 1, n - 1);
            int right = std::min(left + 2 * curr_size - 1, n - 1);
            merge(arr, left, mid, right);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <n> <nbthreads>" << std::endl;
        return -1;
    }

    int n = atoi(argv[1]);
    int numThreads = atoi(argv[2]);

    // Set the number of OpenMP threads
    omp_set_num_threads(numThreads);

    // Get arr data
    int* arr = new int[n];
    generateMergeSortData(arr, n);

   // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

    // Perform parallel merge sort
    parallelMergeSort(arr, n, numThreads);

  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  // Print the total execution time (in sec) to the error stream
  std::cerr << elapsed_seconds.count() << std::endl;

    // Check the result
    checkMergeSortResult(arr, n);

    delete[] arr;

    return 0;
}
