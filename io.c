#include "io.h"
#include "structs.h"
#include "workload_distribution.h"
#include <stdio.h>
#include <regex.h>
#include <mpi.h>
#include <stdlib.h>
//local function definition
workload formatted_buffer_to_workload(char* buffer, int size);



workload read_vectors_from_file(char* filename, int rank, int size)
{
    FILE* file = fopen(filename, "r");
    vp_growing_array arr = growing_array_create();
    float input[3];

    while(fscanf(file,"%f%f%f",input,input+1,input+2)>2)
    {
        growing_array_add(arr,input,sizeof(float)*3);
    }

    workload wr;
    wr.size = growing_array_size(arr)/(3*sizeof(float));
    wr.vectors = growing_array_copy(arr,0,wr.size*3*sizeof(float));

    fclose(file);
    growing_array_free(arr);
    return select_workload(wr,rank,size);

}

workload read_vectors_from_file_parallel(char* filename, int rank, int size)
{

    MPI_File file;
    MPI_Info info = MPI_INFO_NULL;
    MPI_Offset offset = 0;
    MPI_Status status;

    int readstart, readsize;

    int filesize = 0;

    if(MPI_File_open(MPI_COMM_WORLD,filename,MPI_MODE_RDONLY,info,&file))
        exit(MPI_FILE_NULL);

    MPI_File_get_size(file,&filesize);

    count_work_info(filesize/40, rank, size, &readstart, &readsize);

    char* buffer = malloc(readsize*40);

    offset = readstart*40;

    MPI_File_seek(file, offset, MPI_SEEK_SET);
    MPI_File_read(file,buffer,readsize*40,MPI_CHAR,&status);
    MPI_File_close(&file);


    workload ret = formatted_buffer_to_workload(buffer,readsize*40);
    ret.total_size = filesize/40;
    free(buffer);

    return ret;

}

void log_worktimes(worktime* worktimes, int count, char* tag)
{
    char buffer[32];
    sprintf(buffer,"worktimes_%s.log",tag);
    FILE* file = fopen(buffer, "w");
    fprintf(file, "Worktimes by proc: \n");

    double total_read_data = 0;
    double total_process_data = 0;
    double total_reduce_results = 0;
    double total_total = 0;

    for(int i=0; i<count; i++)
    {

        total_read_data += worktimes[i].read_data_time;
        total_process_data += worktimes[i].process_data_time;
        total_reduce_results += worktimes[i].reduce_results_time;
        total_total += worktimes[i].total;

        fprintf(file, "Proc: %d\n", i);
        fprintf(file, "\t%-15s %.6lf\n","read_data",worktimes[i].read_data_time);
        fprintf(file, "\t%-15s %.6lf\n","process_data",worktimes[i].process_data_time);
        fprintf(file, "\t%-15s %.6lf\n","reduce_results",worktimes[i].reduce_results_time);
        fprintf(file, "\t%-15s %.6lf\n\n","total",worktimes[i].total);

    }

    fprintf(file, "Total:\n");
    fprintf(file, "\t%-15s %.6lf\n","read_data",total_read_data);
    fprintf(file, "\t%-15s %.6lf\n","process_data",total_process_data);
    fprintf(file, "\t%-15s %.6lf\n","reduce_results",total_reduce_results);
    fprintf(file, "\t%-15s %.6lf\n\n","total",total_total);


    fclose(file);
}
workload formatted_buffer_to_workload(char* buffer, int size)
{
    int vectorc = size/40;
    vector3* vectors = malloc(sizeof(vector3)*vectorc);
    workload ret;
    ret.size = vectorc;
    ret.vectors = vectors;

    for (int i = 0; i<vectorc ; i++ )
    {

        vectors[i].x = atof(buffer+40*i);
        vectors[i].y = atof(buffer+40*i+13);
        vectors[i].z = atof(buffer+40*i+26);
        //printf("%f %f %f\n",vectors[i].x, vectors[i].y, vectors[i].z);
    }

    return ret;

}


