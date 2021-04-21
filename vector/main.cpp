#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "global.h"
#include "foo.h"

#include <vector>

#pragma omp declare target
std::vector<int> B;
#pragma omp end declare target

int main(void)
{
  #pragma omp target
  {
    B.push_back(2);
  }

  return 0;
}
