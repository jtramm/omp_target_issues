#pragma once

#pragma omp declare target
void foo(int i);
#pragma omp end declare target
