#ifndef FOO_H
#define FOO_H

#pragma omp declare target
void foo(int i);
#pragma omp end declare target

#endif
