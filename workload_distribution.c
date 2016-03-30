#ifndef __WORKLOAD_DISTRIBUTION_H__
#define __WORKLOAD_DISTRIBUTION_H__

#include <string.h>
#include <stdlib.h>
#include "workload_distribution.h"
#include "structs.h"

#define MIN(x,y) (((x) > (y)) ? (y) : (x))



workload select_workload(workload wl, int rank, int size)
{
    int workload_size;
    int workload_start;
    count_work_info(wl.size,rank,size,&workload_start,&workload_size);
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

void count_work_info(int worksize, int rank, int size, int* startbuf, int* sizebuf)
{
    *sizebuf = (rank < worksize % size) ? worksize/size+1 : worksize/size;
    *startbuf = worksize/size*rank+MIN(worksize%size,rank); 
}


#undef MIN
#endif // __WORKLOAD_DISTRIBUTION_H__

