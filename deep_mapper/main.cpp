#include <stdio.h>
#include <stdlib.h>

typedef struct{
  int val;
  int * arr;
} MyObject;

#pragma omp declare target
MyObject * objects;
#pragma omp end declare target

void move_data_to_device(int N)
{
  #pragma omp target enter data map(to: objects[:N])
  for( int i = 0; i < N; i++ )
  {
    #pragma omp target enter data map(to: objects[i].arr[:N])
  }
}

void move_data_to_host(int N)
{
  for( int i = 0; i < N; i++ )
  {
    #pragma omp target exit data map(from: objects[i].arr[:N])
  }
  #pragma omp target exit data map(from: objects[:N])
}

int main(void)
{
  int N = 5;

  // Allocate and initialize host array
  objects = (MyObject *) malloc(N * sizeof(MyObject));
  for( int i = 0; i < N; i++ )
  {
    objects[i].val = i;
    objects[i].arr = (int *) malloc(N * sizeof(int));
    for( int j = 0; j < N; j++ )
    {
      objects[i].arr[j] = i;
    }
  }

  move_data_to_device(N);

  // Execute device kernel
  #pragma omp target teams distribute parallel for
  for( int i = 0; i < N; i++)
  {
    objects[i].val *= 2;
    for( int j = 0; j < N; j++ )
    {
      objects[i].arr[j] *= 2;
    }
  }

  move_data_to_host(N);

  // Print expected and actual results and check for any errors
  printf("Primitive Value Check:\n");
  printf("\tExpected Result: 0 2 4 6 8\n");
  printf("\tActual   Result: ");
  for( int i = 0; i < N; i++)
    printf("%d ", objects[i].val);
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
      printf("%d ", objects[i].arr[j]);
    }
    printf("\n");
  }
  
  if( objects[4].val != 8 )
  {
    printf("Error!\n");
    return 1;
  }
  
  if( objects[4].arr[4] != 8 )
  {
    printf("Error!\n");
    return 2;
  }

  return 0;
}
