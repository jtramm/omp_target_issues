#ifndef GLOBAL_H
#define GLOBAL_H

#pragma omp declare target
extern int * global_array;
#pragma omp end declare target

#endif
