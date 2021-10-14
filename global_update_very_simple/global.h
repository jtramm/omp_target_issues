#pragma once
#include "shared_array.h"

#pragma omp declare target
extern SharedArray<int64_t> queue;
#pragma omp end declare target
