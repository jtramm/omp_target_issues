#include "foo.h"

int main(void)
{
  #pragma omp target
  {
    foo("Test");
  }

  return 0;
}
