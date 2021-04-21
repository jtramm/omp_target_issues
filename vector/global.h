#ifndef GLOBAL_H
#define GLOBAL_H
#include<vector>

#pragma omp declare target
extern std::vector<int> device_arr_vec;
extern int * device_arr;
#pragma omp end declare target

#endif
