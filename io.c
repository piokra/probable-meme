#include "io.h"
#include "structs.h"
#include <stdio.h>

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

void log_worktimes(worktime* worktimes, int count)
{
    FILE* file = fopen("worktimes.log", "w");
    fprintf(file, "Worktimes by proc: \n");
    for(int i=0; i<count; i++)
    {

        fprintf(file, "Proc: %d\n", i);
        fprintf(file, "\t%-15s %.6lf\n","read_data",worktimes[i].read_data_time);
        fprintf(file, "\t%-15s %.6lf\n","process_data",worktimes[i].process_data_time);
        fprintf(file, "\t%-15s %.6lf\n","reduce_results",worktimes[i].reduce_results_time);
        fprintf(file, "\t%-15s %.6lf\n\n","total",worktimes[i].total);

    }

    fclose(file);
}
