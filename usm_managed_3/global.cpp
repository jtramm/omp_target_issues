#include<stdlib.h>

typedef struct{
  int * host_arr;
} Object;

Object obj;

void * allocator(size_t sz);

void init_arr(size_t sz)
{
  obj.host_arr = (int *) allocator(sz);
}
