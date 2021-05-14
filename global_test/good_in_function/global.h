#ifndef GLOBAL_H
#define GLOBAL_H

class Global{
  public:
  double a;
  double b;
  double * c;
  int N;
};

#pragma omp declare target
extern Global A;
#pragma omp end declare target

#endif
