#include <mpi.h>
#include <stdio.h>

#include "io.h"
#include "workload_distribution.h"
#include "vectors.h"
#include "communication.h"

//global variables
int rank;
int size;
int vector_count;
char filename[32];
float result_length;
vector3 result_vector;

void do_what_master_does();
void do_what_slave_does();
worktime* do_work();

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank==0)
    {
        do_what_master_does();
    }
    else
    {
        do_what_slave_does();
    }

    MPI_Finalize();
}

void do_what_master_does()
{

    scanf("%s",filename);
    MPI_Bcast(filename,32,MPI_CHAR,0,MPI_COMM_WORLD);

    worktime* worktimes = do_work();

    float fn = vector_count;
    float x = result_vector.x/fn;
    float y = result_vector.y/fn;
    float z = result_vector.z/fn;

    printf("Length: %f\nCoords: %f %f  %f.\n",result_length/fn, x,y,z);

    log_worktimes(worktimes,size);
}

void do_what_slave_does()
{

    MPI_Bcast(filename,32,MPI_CHAR,0,MPI_COMM_WORLD);

    do_work();

}

worktime* do_work()
{
    worktime my_worktime;
    double start,end;

    start = MPI_Wtime();

    workload all_work = read_vectors_from_file(filename);
    vector_count = all_work.size;

    end = MPI_Wtime();
    my_worktime.read_data_time = end - start;

    start = MPI_Wtime();

    workload my_work = select_workload(all_work, rank, size);
    vector3 my_total_vector = count_local_total_vector(my_work);
    float my_length = count_local_total_length(my_work);

    end = MPI_Wtime();
    my_worktime.process_data_time = end - start;

    start = MPI_Wtime();

    result_length = reduce_total_length(my_length);
    result_vector = reduce_total_vector(my_total_vector);

    end = MPI_Wtime();
    my_worktime.reduce_results_time = end - start;

    my_worktime.total = 0;
    my_worktime.total += my_worktime.process_data_time;
    my_worktime.total += my_worktime.read_data_time;
    my_worktime.total += my_worktime.reduce_results_time;

    return gather_worktimes(my_worktime, rank, size);
}
