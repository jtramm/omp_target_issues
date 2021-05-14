#include <assert.h>
#include "global.h"

void foo(void) {
  A.value *= 2;
  A.array[0] *= 2;
}

int main(void)
{
  A.value = 1;
  A.array = new int[1];
  A.array[0] = 1;

  #pragma omp target update to(A)
  #pragma omp target enter data map (to: A.array[:1])
  
  #pragma omp target
  {
    foo();
  }

  #pragma omp target update from(A)
  #pragma omp target exit data map (from: A.array[:1])

  assert(A.value == 2);
  assert(A.array[0] == 2);
}
