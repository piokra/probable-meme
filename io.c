#include "io.h"
#include "structs.h"
#include "workload_distribution.h"
#include <stdio.h>
#include <regex.h>
//local structs
typedef struct
{
    int size;
    char* char_buffer;
} formatted_char_buffer;
//local function declaration
vector3 lines_to_workload(char* line_start, int size);
float chars_to_float(char* start, int lenght);


workload read_vectors_from_file(char* filename)
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
    return wr;

}

workload read_vectors_from_file_parallel(char* filename, int rank, int size)
{
    
    MPI_File file;
    MPI_Info info = MPI_INFO_NULL;
    if(!MPI_File_open(MPI_COMM_WORLD,filename,MPI_MODE_RDONLY,info,&file))
        exit(420);
    MPI_Offset offset = 0;
    int filesize = MPI_File_get_size(file,offset);
    int readstart, readsize;
    count_work_info(filesize, rank, size, &readstart, &readsize);
    offset = readstart;
    MPI_File_seek(file, offset, MPI_SEEK_SET);
    char* buffer = malloc(readsize);
    MPI_Status status;
    MPI_File_read(file,buffer,readsize,MPI_CHAR,&status);
    MPI_File_close(&file);
    
    return lines_to_vector(buffer,readsize);

}

void log_worktimes(worktime* worktimes, int count)
{
    FILE* file = fopen("worktimes.log", "w");
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

vector3 line_to_vector(char* line_start, int size)
{

    vector3 t;
    int vector_count = size/120;
    int group_count = vector_count*3;
    char* regex_string = "\s*([[0-9].-]*)";
    regex_t regex_compiled;
    regmatch_t* regex_match_groups = malloc(sizeof(regmatch_t)*group_count);
    workload* ret = malloc(sizeof(workload)*vector_count);
        
    if(regcomp(&regex_compiled, regex_string, REG_EXTENDED))
        exit(420);
    
    for(int i=0; i<vector_count; i++)
    {
        ret->vectors[i].x = chars_to_float()
    }
    
    
    
    free(regex_match_groups);
    return t;
}

float chars_to_float(char* source, int length)
{
    char buffer[length+1];//c99
    memcpy(buffer,source,length);
    buffer[length] = 0;
    return atof(buffer);
}
