#include "vectors.h"
#include <math.h>
#include <mpi.h>


float count_local_total_length(workload wr)
{
    float ret = 0;
    for(int i=0;i<wr.size;i++)
    {
        float x = wr.vectors[i].x;
        float y = wr.vectors[i].y;
        float z = wr.vectors[i].z;

        float ls = x*x+y*y+z*z;

        ret += sqrtf(ls);
    }
    return ret;
}

vector3 count_local_total_vector(workload wr)
{
    vector3 ret;

    ret.x = 0;
    ret.y = 0;
    ret.z = 0;

    for(int i=0;i<wr.size;i++)
    {
        float x = wr.vectors[i].x;
        float y = wr.vectors[i].y;
        float z = wr.vectors[i].z;

        ret.x += x;
        ret.y += y;
        ret.z += z;

    }
    return ret;



}

float reduce_total_length(float len)
{
    float ret;
    MPI_Reduce(&len, &ret, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    return ret;
}

vector3 reduce_total_vector(vector3 vec)
{
    vector3 ret;
    MPI_Reduce(&vec, &ret, 3, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
    return ret;
}

worktime* gather_worktimes(worktime __worktime, int comm_rank, int comm_size)
{
    worktime* ret;
    if(comm_rank==0)
        ret = malloc(comm_size*sizeof(worktime));
    MPI_Gather(&__worktime, 4, MPI_DOUBLE, ret, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return ret;
}
