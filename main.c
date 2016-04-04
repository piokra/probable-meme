#include <mpi.h>
#include <stdio.h>

#include "io.h"
#include "workload_distribution.h"
#include "vectors.h"
#include "communication.h"

//global variables
int rank;
int size;
char mode;
char mode_name[8];
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
    printf("Mode: [A]ll/[P]arallel\n");
    scanf("%c", filename+31);
    printf("Filename:\n");
    scanf("%s",filename);
    MPI_Bcast(filename,32,MPI_CHAR,0,MPI_COMM_WORLD);

    worktime* worktimes = do_work();

    float fn = vector_count;
    float x = result_vector.x/fn;
    float y = result_vector.y/fn;
    float z = result_vector.z/fn;

    printf("Length: %f\nCoords: %f %f  %f.\n",result_length/fn, x,y,z);

    log_worktimes(worktimes,size,mode_name);
}

void do_what_slave_does()
{

    MPI_Bcast(filename,32,MPI_CHAR,0,MPI_COMM_WORLD);

    do_work();

}

worktime* do_work()
{
    switch(filename[31])
    {
        case 'A':
            mode=0;
            sprintf(mode_name,"ALL");
            break;
        case 'P':
            mode=1;
            sprintf(mode_name,"PAR");
            break;
        default:
        exit(-1);
    }

    worktime my_worktime;
    double start,end;

    start = MPI_Wtime();

    workload work;
    if(mode==1)
        work = read_vectors_from_file_parallel(filename,rank,size);
    if(mode==0)
        work = read_vectors_from_file(filename,rank,size);

    vector_count = work.total_size;

    end = MPI_Wtime();
    my_worktime.read_data_time = end - start;

    start = MPI_Wtime();

    vector3 my_total_vector = count_local_total_vector(work);
    float my_length = count_local_total_length(work);

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
