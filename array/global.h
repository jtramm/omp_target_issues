#include <array>
#define len 6
namespace settings {

#pragma omp declare target
extern std::array<double, len> arr;
#pragma omp end declare target

}
