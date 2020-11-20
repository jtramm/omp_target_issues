// Unifed Shared Memory

typedef struct{
  int len;
  int * inner_array;
} Object;

#pragma omp requires unified_shared_memory

Object * array = (Object *) omp_target_alloc(N * sizeof(Object), omp_get_default_device());

// ... allocate inner arrays in similar manner ...

#pragma omp target
{
  foo(array);
}



// Direct Mapping

typedef struct{
  int len;
  int * inner_array;
} Object;

Object * array = (Object *) malloc(N * sizeof(Object));

// ... allocate inner arrays in similar manner ...

// Problem: this only performs shallow mapping
#pragma omp target map(to: array[0:N])
{
  foo(array);
}



// Custom Mappers

typedef struct{
  int len;
  int * inner_array;
} Object;
#pragma omp declare mapper(Object obj) map(obj.len, obj.inner_array[0:obj.len])

Object * array = (Object *) malloc(N * sizeof(Object));

// ... allocate inner arrays in similar manner ...

// This now performs a deep copy
#pragma omp target map(to: array[0:N])
{
  foo(array);
}



// Target enter/exit data

typedef struct{
  int len;
  int * inner_array;
} Object;

Object * array = (Object *) malloc(N * sizeof(Object));

// ... allocate inner arrays in similar manner ...

#pragma omp target enter data map(to: array[0:N])
for( int i = 0; i < N; i++ )
{
  #pragma omp target enter data map(to: array[i].inner_array[0:array[i].len])
}

#pragma omp target
{
  foo(array);
}



// Manual CUDA or OpenCL-like Control

typedef struct{
  int len;
  int * inner_array;
} Object;

Object * array = (Object *) malloc(N * sizeof(Object));

// ... allocate inner arrays in similar manner ...

Object * device_array = (Object *) omp_target_alloc(N * sizeof(Object), omp_get_default_device());

// ... allocate inner device arrays in similar manner ...

// ... deep copy data from host -> device inner arrays (too complex to show here) ...
// ... deep copy data from host -> device outer array  (too complex to show here) ...

// This now performs a deep copy
#pragma omp target is_device_ptr(device_array)
{
  foo(device_array);
}
