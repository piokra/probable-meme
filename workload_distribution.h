#ifndef __WORKLOAD_DISTRIBUTION_H__
#define __WORKLOAD_DISTRIBUTION_H__

#include "structs.h"

workload select_workload(workload wl, int rank, int size);
void count_work_info(int worksize, int rank, int size, int* startbuf, int* sizebuf);


#endif // __WORKLOAD_DISTRIBUTION_H__

