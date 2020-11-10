#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#pragma omp declare target
int * arr;
#pragma omp end declare target

void move_data_to_device(int N)
{
  #pragma omp target enter data map(to: arr[0:N])
}

void move_data_to_host(int N)
{
  #pragma omp target exit data map(from: arr[0:N])
}

int main(void)
{
  int N = 5;

  // Allocate and initialize host array
  size_t sz = N * sizeof(int);
  arr = (int *) malloc(sz);
  for( int i = 0; i < N; i++ )
  {
    arr[i] = i;
  }

  move_data_to_device(N);

  printf("launching kernel\n");

  // Execute device kernel
  #pragma omp target teams distribute parallel for
  for( int i = 0; i < N; i++)
  {
    arr[i] *= 2;
  }
  
  printf("kernel finished\n");

  move_data_to_host(N);

  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6 8\n");
  printf("Actual   Result: ");
  for( int i = 0; i < N; i++)
    printf("%d ", arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[4] != 8 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}
