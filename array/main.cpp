#include <stdio.h>
#include <stdlib.h>
#include <array>

#define len 6

#pragma omp declare target
std::array<int, len> arr;
#pragma omp end declare target

#pragma omp declare target
void foo(int i)
{
  arr[i] *= 2;
} 
#pragma omp end declare target

int main(void)
{
  for( int i = 0; i < len; i++ )
    arr[i] = i;

  #pragma omp target update to(arr)

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < len; i++ )
  {
    foo(i);
  }
  
  #pragma omp target update from(arr)
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 2 4 6 8 10\n");
  printf("Actual   Result: ");
  for( int i = 0; i < len; i++ )
    printf("%d ", arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[5] != 10 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}
