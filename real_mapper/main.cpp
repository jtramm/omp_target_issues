#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

// Note that throughout the code, we are determining the number of
// elements to allocate using a random number provided by rand().
// In a real application, the number of elements would be provided
// by a user input file. Randomization is used to simplify the interface
// while trying to still make it clear that these allocations may
// be of different sizes based on runtime user inputs.

//////////////////////////////////////////////////
// Classes
//////////////////////////////////////////////////

class Surface
{
  public:
    double x0_, y0_, z0_, radius_, radius_sq_;
    double A_, B_, C_, D_, E_, F_, G_, H_, J_, K_;

    int * neighbor_ids; // 1D Array
    int   neighbor_ids_length;

    Surface()
    {
      neighbor_ids_length = rand() % 100;
      neighbor_ids        = new int[neighbor_ids_length];
      
      // Initialize ids to meaningful values here
      // ...
    }
};
#pragma omp declare mapper (Surface s) map(s, \
    s.neighbor_ids[:s.neighbor_ids_length] \
    )

class Cell
{
  public:
    int * material_ids; // 1D Array
    int   material_ids_length;

    double * temps; // 1D Array
    int      temps_length;

    int * neighbor_ids; // 1D Array
    int   neighbor_ids_length;

    Cell()
    {
      material_ids_length = rand() % 100;
      material_ids        = new int[material_ids_length];

      temps_length        = rand() % 100;
      temps               = new double[temps_length];

      neighbor_ids_length = rand() % 100;
      neighbor_ids        = new int[neighbor_ids_length];

      // Initialize arrays to meaningful values here
      // ...
    }
};
#pragma omp declare mapper (Cell c) map(c, \
    c.material_ids[:c.material_ids_length], \
    c.temps[       :c.temps_length], \
    c.neighbor_ids[:c.neighbor_ids_length] \
    )

class UniversePartitioner
{
  public:
    int * surface_ids; // 1D Array
    int   surface_ids_length;

    Cell * partitions; // 2D Jagged Array
    int    partitions_length;
    int    partitions_outer_length;
    int *  partitions_inner_lengths;

    UniversePartitioner()
    {
      surface_ids_length       = rand() % 100;
      surface_ids              = new int[surface_ids_length];

      partitions_outer_length  = rand() % 100;
      partitions_inner_lengths = new int[partitions_outer_length];

      partitions_length = 0;
      for( int i = 0; i < partitions_outer_length; i++ )
      {
        partitions_inner_lengths[i] = rand() % 100;
        partitions_length += partitions_inner_lengths[i];
      }
      
      partitions               = new Cell[partitions_length];

      // Initialize arrays to meaningful values here
      // ...
    }
};
#pragma omp declare mapper (UniversePartitioner up) map(up, \
    up.surface_ids[             :up.surface_ids_length], \
    up.partitions[              :up.partitions_length], \
    up.partitions_inner_lengths[:up.partitions_outer_length] \
    )

class Universe
{
  public:
    int * surface_ids; // 1D Array
    int   surface_ids_length;

    UniversePartitioner * partitioners;        // 1D Array
    int                   partitioners_length;

    Universe()
    {
      surface_ids_length  = rand() % 100;
      surface_ids         = new int[surface_ids_length];
      
      partitioners_length = rand() % 100;
      partitioners        = new UniversePartitioner[partitioners_length];
      
      // Initialize to meaningful values here
      // ...
    }
};
#pragma omp declare mapper (Universe u) map(u, \
    u.surface_ids[ :u.surface_ids_length], \
    u.partitioners[:u.partitioners_length], \
    )

//////////////////////////////////////////////////
// Global Scope Arrays
//////////////////////////////////////////////////

Surface * surfaces; // 1D Array
int       surfaces_length;

Universe * universes; // 1D Array
int        universes_length;

//////////////////////////////////////////////////
// Program
//////////////////////////////////////////////////

void foo(int i)
{
  // Work that requires reading from the global "universes" and "surfaces" arrays
}

int main(int argc, char * argv[])
{
  srand(time(NULL));

  surfaces_length = rand() % 100;
  surfaces = new Surface[surfaces_length];
  
  universes_length = rand() % 100;
  universes = new Universe[universes_length];

  // Initialize global arrays to meaningful values here
  // ...

  #pragma omp target teams distribute parallel for map(surfaces[:surfaces_length], universes[:universes_length])
  for( int i = 0; i < 1000000; i++)
    foo(i);

  return 0;
}
