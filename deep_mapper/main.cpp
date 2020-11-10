#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

typedef struct{
  int val;
  int * arr;
} MyObject;

#pragma omp declare target
MyObject * object_arr;
#pragma omp end declare target

void move_data_to_device(int N)
{
  #pragma omp target enter data map(to: object_arr[:N])
  for( int i = 0; i < N; i++ )
  {
    #pragma omp target enter data map(to: object_arr[i].arr[:N])
  }
}

void move_data_to_host(int N)
{
  #pragma omp target exit data map(from: object_arr[:N])
  for( int i = 0; i < N; i++ )
  {
    #pragma omp target exit data map(from: object_arr[i].arr[:N])
  }
}

int main(void)
{
  int N = 5;

  // Allocate and initialize host array
  object_arr = (MyObject *) malloc(N * sizeof(MyObject));
  for( int i = 0; i < N; i++ )
  {
    object_arr[i].val = i;
    object_arr[i].arr = (int *) malloc(N * sizeof(int));
    for( int j = 0; j < N; j++ )
    {
      object_arr[i].arr[j] = i;
    }
  }

  move_data_to_device(N);

  // Execute device kernel
  #pragma omp target teams distribute parallel for
  for( int i = 0; i < N; i++)
  {
    object_arr[i].val *= 2;
    for( int j = 0; j < N; j++ )
    {
      object_arr[i].arr[j] *= 2;
    }
  }

  move_data_to_host(N);

  // Print expected and actual arrays on host
  printf("Primitive Value Check:\n");
  printf("\tExpected Result: 0 2 4 6 8\n");
  printf("\tActual   Result: ");
  for( int i = 0; i < N; i++)
    printf("%d ", object_arr[i].val);
  printf("\n");

  printf("Array Values Check:\n");
  printf("Expected Result:\n");
  for( int i = 0; i < N; i++)
  {
    printf("\t");
    for( int j = 0; j < N; j++)
    {
      printf("%d ", i*2);
    }
    printf("\n");
  }
  printf("Actual Result:\n");
  for( int i = 0; i < N; i++)
  {
    printf("\t");
    for( int j = 0; j < N; j++)
    {
      printf("%d ", object_arr[i].arr[j]);
    }
    printf("\n");
  }
  
  // Return non-zero error code if we failed
  if( object_arr[4].val != 8 )
  {
    printf("Error!\n");
    return 1;
  }
  
  if( object_arr[4].arr[4] != 8 )
  {
    printf("Error!\n");
    return 2;
  }

  return 0;
}
