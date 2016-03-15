#ifndef __STRUCTS_H__
#define __STRUCTS_H__

//task structures
typedef struct
{
    float x;
    float y;
    float z;
} vector3;
typedef struct
{
    int size;
    vector3* vectors;
} workload;
typedef struct
{

    double read_data_time;
    double process_data_time;
    double reduce_results_time;
    double total;
} worktime;
//utility structures

typedef void* vp_growing_array;

typedef struct
{
    int size;
    int max_size;
    void* buffer;
} growing_array;

vp_growing_array growing_array_create();
void growing_array_free(vp_growing_array arr);
void growing_array_add(vp_growing_array arr, void* data, int size);
void* growing_array_access(vp_growing_array arr, int pos);
void* growing_array_copy(vp_growing_array arr, int pos, int len);
int growing_array_size(vp_growing_array arr);
#endif // __STRUCTS_H__

