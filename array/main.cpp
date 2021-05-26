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
  arr[i] = i;
} 
#pragma omp end declare target

int main(void)
{
  // Set to zero
  for( int i = 0; i < len; i++ )
    arr[i] = 0;

  #pragma omp target update to(arr)

  // Map and fill with values on device
  #pragma omp target teams distribute parallel for
  for( int i = 0; i < len; i++)
  {
    // Calling foo appears to have no effect
    foo(i);

    // If we uncomment this to inline the contents of foo, it works correctly.
    //arr[i] = i;
  }
  
  #pragma omp target update from(arr)
  
  // Print expected and actual arrays on host
  printf("Expected Result: 0 1 2 3 4 5\n");
  printf("Actual   Result: ");
  for( int i = 0; i < len; i++)
    printf("%d ", arr[i]);
  printf("\n");
  
  // Return non-zero error code if we failed
  if( arr[5] != 5 )
  {
    printf("Error!\n");
    return 1;
  }

  return 0;
}
