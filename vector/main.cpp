#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include <vector>

#pragma omp declare target
std::vector<int> A;
#pragma omp end declare target

int main(void)
{
  A.push_back(1);
  #pragma omp target
  {
    //A.push_back(1);
    //printf("A[0] = %d\n", A[0]);
    printf("hello\n");
  }

  return 0;
}
