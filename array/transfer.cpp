#include "transfer.h"
void copy_host_to_device()
{
  #pragma omp target update to(arr)
}

void copy_device_to_host()
{
  #pragma omp target update from(arr)
}
