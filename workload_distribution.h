#ifndef __WORKLOAD_DISTRIBUTION_H__
#define __WORKLOAD_DISTRIBUTION_H__

#include "structs.h"

workload select_workload(workload wl, int rank, int size);

#endif // __WORKLOAD_DISTRIBUTION_H__

