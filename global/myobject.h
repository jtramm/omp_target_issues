#pragma once

#include<iostream>
#include<cmath>
#include<cstdlib>
#include<vector>
#include<omp.h>

class MyObject{
  public:
    MyObject(int idx);
#pragma omp declare target
    void foo();
#pragma omp end declare target
    void show();
    void allocate_on_device();
    void copy_to_device();
    void copy_from_device();
    std::vector<int> vec;
    int * ptr;
    int len;
};

#pragma omp declare mapper(MyObject a) map(a, a.ptr[:a.len])
