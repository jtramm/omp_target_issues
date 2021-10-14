#include "global.h"
#include <iostream>

int main(void)
{
  // Reserve space in the global queue on host
  int64_t n_particles = 10;
  queue.reserve(n_particles);

  // Allocate space for the queue on device
  queue.allocate_on_device();

  // Fill global queue on host
  for( int64_t i = 0; i < n_particles; i++ )
    queue.thread_safe_append(i);

  // Copy queue contents to device
  queue.copy_host_to_device();

  // Access queue contents on device
  #pragma omp target teams distribute parallel for
  for (int64_t i = 0; i < queue.size(); i++)
  {
    int64_t value = queue[i];
    printf("value = %ld\n", value);
  }

  return 0;
}
