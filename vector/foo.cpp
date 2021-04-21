#include "global.h"
#include "foo.h"
#include <stdio.h>

void foo(int i)
{
  device_arr[i] *= 2;
  device_arr_vec[i] *= 2;
  //device_arr_vec.push_back(i);
  printf("size = %d\n", device_arr_vec.size());
}
