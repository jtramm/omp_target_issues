#include "myobject.h"
#include"globals.h"

MyObject::MyObject(int idx)
{
  for( int i = 0; i < idx; i++ )
  {
    vec.push_back(i);
  }
  len = vec.size();
  ptr = vec.data();
}

void MyObject::foo()
{
  printf("len = %d\n", len);
  for( int i = 0; i < vec.size(); i++ )
  {
    printf("index %d begins as %d\n", i, ptr[i]);
    printf("device stuff index %d is %d\n", i, device_stuff[i]);
    ptr[i] = ptr[i] * device_stuff[i];
  }
}

void MyObject::show()
{
  for( int i = 0; i < vec.size(); i++ )
    std::cout << vec[i] << " ";
  std::cout << std::endl;
}
