#include <array>
#pragma omp declare target
extern std::array<double, 4> arr;
#pragma omp end declare target
