#ifndef GLOBAL_H
#define GLOBAL_H

class MyType{
public:
    int value;
    int* array;
};
#pragma omp declare target
extern MyType A;
#pragma omp end declare target

#endif
