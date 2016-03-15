#ifndef __WORKLOAD_DISTRIBUTION_H__
#define __WORKLOAD_DISTRIBUTION_H__

#include <string.h>
#include <stdlib.h>
#include "workload_distribution.h"
#include "structs.h"

#define MIN(x,y) (((x) > (y)) ? (y) : (x))
workload select_workload(workload wl, int rank, int size)
{
    int workload_size = (rank < wl.size % size) ? wl.size/size+1 : wl.size/size;
    int workload_start = wl.size/size*rank+MIN(wl.size%size,rank);
    vector3* buffer = 0;
    workload ret;

    if(workload_size > 0)
    {
        buffer = malloc(3*workload_size*sizeof(float));
        memcpy(buffer,wl.vectors+workload_start,3*workload_size*sizeof(float));
    }

    free(wl.vectors);
    ret.vectors = buffer;
    ret.size = workload_size;


    return ret;
}
#undef MIN(x,y)
#endif // __WORKLOAD_DISTRIBUTION_H__

