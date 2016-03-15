#include "vectors.h"
#include <math.h>
#include <mpi.h>


float count_local_total_length(workload wr)
{
    float ret = 0;
    for(int i=0; i<wr.size; i++)
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

    for(int i=0; i<wr.size; i++)
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

