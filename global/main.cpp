#include "myobject.h"
#include "globals.h"

int main(void)
{
  // initialize host stuff
  host_stuff.push_back(10);
  host_stuff.push_back(20);
  host_stuff.push_back(30);
  host_stuff.push_back(40);
  host_stuff.push_back(50);
  
  int len = 5;

  // Initialize host array
  for( int i = 0; i < len; i++ )
    host_array.emplace_back(i+1);

  // Initialize on host
  for( auto& obj : host_array )
  {
    obj.show();
  }

  device_array = host_array.data();
  device_stuff = host_stuff.data();
  int array_len = host_array.size();
  int stuff_len = host_stuff.size();

  #pragma omp target teams distribute parallel for\
  map(tofrom: device_array[:array_len]) map(to: device_stuff[:stuff_len])
  for( int i = 0; i < len; i++)
  {
    printf("calling foo on index %d, with length = %d\n", i, device_array[i].len);
    printf("device_stuff[%d] = %d\n", i, device_stuff[i]);
    //device_array[i].foo();
    MyObject& a = device_array[i];
    for( int j = 0; j < a.vec.size(); j++ )
    {
      printf("index %d begins as %d\n", j, a.ptr[j]);
      printf("device stuff index %d is %d\n", j, device_stuff[j]);
      a.ptr[j] = a.ptr[j] * device_stuff[j];
    }
  }
  
  // Show data on host
  for( auto& obj : host_array )
  {
    obj.show();
  }

}
