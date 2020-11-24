#include <stdio.h>
#include <assert.h>
#include <omp.h>

#pragma omp requires unified_shared_memory

typedef struct{
  int * val;
} Object;

int main(void)
{
  Object obj;

  obj.val = (int *) omp_target_alloc(sizeof(int), omp_get_default_device());

  obj.val[0] = 1;

  #pragma omp target
  {
    obj.val[0] *= 2;
  }

  assert(obj.val[0] == 2);

  return 0;
}
