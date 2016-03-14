#include <mpi.h>

#include "io.h"

//global variables
int rank;
int size;

int main()
{
    workload wl = read_vectors_from_file("test.dat");
    for (int i=0;i<5;i++)
    {
        printf("%f %f %f\n",wl.vectors[i].x,wl.vectors[i].y,wl.vectors[i].z);
    }
    return 0;
}
