#include "communication.h"
#include "mpi.h"
#include <stdlib.h>
#include <string.h>
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

vector4 reduce_total_vector_and_length(vector3 vec, float len)
{
    vector4 ret;
    vector4 send;

    memcpy(&send, &vec, 3*sizeof(float));
    send.l = len;

    MPI_Reduce(&send, &ret, 4, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
}

worktime* gather_worktimes(worktime __worktime, int comm_rank, int comm_size)
{
    worktime* ret = 0;

    if(comm_rank==0)
    {
        ret = malloc(comm_size*sizeof(worktime));
    }

    MPI_Gather(&__worktime, 4, MPI_DOUBLE, ret, 4, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return ret;
}
