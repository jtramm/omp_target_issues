#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <algorithm>

double run_empty(){
  // Time empty kernel
  double start = omp_get_wtime();
  #pragma omp target
  {}
  double stop = omp_get_wtime();
  return (stop - start)*1000;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Provide dimension of 2D array as first argument...\n");
    return -1;
  }
  size_t len = atoi(argv[1]);

  // Allocate 2D matrix of size len x len
  double **arr = (double  **)malloc(len * sizeof(double*));
  for (int i = 0; i < len; i++) {
    arr[i] = (double *) calloc(len, sizeof(double));
  }

  // Avoid JIT time
  run_empty();

  printf("Empty Kernel Runtimes Before Data Mapping:\n");
  for (int i = 0; i < 5; i++) {
    double ms0 = run_empty();
    printf("  %.5lf ms\n", ms0);
  }

  // Map 2D matrix to device
  #pragma omp target enter data map(to : arr[:len])
  for (int i = 0; i < len; i++) {
    #pragma omp target enter data map(to : arr[i][:len])
  }

  printf("Empty Kernel Runtimes After Data Mapping:\n");
  for (int i = 0; i < 5; i++) {
    double ms1 = run_empty();
    printf("  %.5lf ms\n", ms1);
  }

  // Unmap 2D matrix
  for (int i = 0; i < len; i++) {
    #pragma omp target exit data map(delete : arr[i][:len])
  }
  #pragma omp target exit data map(delete : arr[:len])

  printf("Empty Kernel Runtimes After Unmapping:\n");
  for (int i = 0; i < 5; i++) {
    double ms2 = run_empty();
    printf("  %.5lf ms\n", ms2);
  }

  return 0;
}
