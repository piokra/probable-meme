#ifndef __COMMUNICATION_H__
#define __COMMUNICATION_H__

#include "structs.h"

float reduce_total_length(float len);
vector3 reduce_total_vector(vector3 vec);
vector4 reduce_total_vector_and_length(vector3 vec, float len);

worktime* gather_worktimes(worktime __worktime, int comm_rank, int comm_size);


#endif // __COMMUNICATION_H__

