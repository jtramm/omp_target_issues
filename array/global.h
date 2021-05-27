#include <array>
namespace settings {

  #pragma omp declare target
  extern int mult;
  extern std::array<double, 4> arr;
  #pragma omp end declare target

}
