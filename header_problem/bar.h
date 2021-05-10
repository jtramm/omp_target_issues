#ifndef BAR_H
#define BAR_H

#pragma omp declare target
void bar(int N);
#pragma omp end declare target

#endif
