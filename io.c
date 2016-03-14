#include "io.h"
#include "structs.h"
#include <stdio.h>

workload read_vectors_from_file(char* filename)
{
    FILE* file = fopen(filename, "r");
    vp_growing_array arr = growing_array_create();
    float input[3];

    while((fscanf(file,"%f%f%f",input,input+1,input+2)>2)
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
