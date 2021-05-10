#ifndef FOO_H
#define FOO_H

#pragma omp declare target
void foo(int N);
#pragma omp end declare target

#endif
