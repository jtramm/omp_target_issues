#include <assert.h>
#include <omp.h>
#include <stdio.h>

struct Obj {
  int len = 1;
  int size(){return len;}
};

int main(void)
{
  Obj A;

  printf("Launching test...\n");

  #pragma omp target teams distribute parallel for
  for( int i = 0; i < A.size(); i++ )
  {
  }
  
  printf("Success\n");

  return 0;
}
