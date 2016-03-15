#ifndef __VECTORS_H__
#define __VECTORS_H__

#include "structs.h"

float count_local_total_length(workload wr);
vector3 count_local_total_vector(workload wr);

float reduce_total_length(float len);
vector3 reduce_total_vector(vector3 vec);

worktime* gather_worktimes(worktime __worktime, int comm_rank, int comm_size);

#endif // __VECTORS_H__

