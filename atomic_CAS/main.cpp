#include <assert.h>

int main(void)
{
  int val  = 0;
  int test = 1;
  int set  = 2;
  int cap  = 3;

  #pragma omp target map(tofrom: val, test, set, cap)
  {
    #pragma omp atomic compare capture
    {
       cap = val;
       if (val < test)
         val = set;
    }
  }

  assert(val == 2);
  assert(test == 1);
  assert(set == 2);
  assert(cap == 0);
  
  return 0;
}

// Basic atomic capture does work
/*
int main(void)
{
  int val  = 0;
  int cap  = 3;

  // Execute device kernel
  #pragma omp target map(tofrom: val, cap)
  {
    #pragma omp atomic capture
       cap = val++;
  }

  assert(cap == 0);
  assert(val == 1);
  
  return 0;
}
*/
