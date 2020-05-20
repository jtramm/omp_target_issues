#pragma once

#include "myobject.h"

extern std::vector<MyObject> host_array;
extern MyObject* device_array;

extern std::vector<int> host_stuff;
  #pragma omp declare target
extern int* device_stuff;
  #pragma omp end declare target

#pragma omp declare target
extern int * arr;
#pragma omp end declare target
