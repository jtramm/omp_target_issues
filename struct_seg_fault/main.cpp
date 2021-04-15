#include <stdio.h>
#include <stdlib.h>

#include "global.h"

int main(void)
{
  int N = 5;
  obj.arr = (int *) calloc(N, sizeof(int));

  #pragma omp target enter data map(to:obj.arr[:N])

  return 0;
}
