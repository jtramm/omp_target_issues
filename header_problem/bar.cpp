#include "bar.h"
#include "global.h"

void bar(int N)
{
  for( int i = 0; i < N; i++ )
    global_array[i] *= 2;
}
