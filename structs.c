#include "structs.h"
#include <stdlib.h>
#include <string.h>

#define GROWING_ARRAY_SMALL_CHUNK 32

vp_growing_array growing_array_create()
{
    growing_array* buffer = malloc(sizeof(growing_array));
    buffer->size=0;
    buffer->max_size = GROWING_ARRAY_SMALL_CHUNK;
    buffer->buffer = malloc(GROWING_ARRAY_SMALL_CHUNK);
}

void growing_array_free(vp_growing_array arr)
{
    growing_array* ga = arr;
    free(ga->buffer);
    free(arr);
}

void growing_array_add(vp_growing_array arr, void* data, int size)
{
    growing_array* ga = arr;
    void* buffer = 0;
    if(ga->size+size > ga->max_size)
    {
        while(ga->size+size > ga->max_size)
        {
            ga->max_size *= 2;
        }
        buffer = malloc(ga->max_size);
        memcpy(buffer,ga->buffer,ga->size);
        free(ga->buffer);
        ga->buffer=buffer;
    }
    memcpy(ga->buffer+ga->size,data,size);
    ga->size+=size;
}

void* growing_array_access(vp_growing_array arr, int pos)
{
    growing_array* ga = arr;
    return ga->buffer+pos;
}
void* growing_array_copy(vp_growing_array arr, int pos, int len)
{
    growing_array* ga = arr;
    void* buf = malloc(len);
    memcpy(buf,ga->buffer+pos,len);
    return buf;
}
int growing_array_size(vp_growing_array arr)
{
    growing_array* ga = arr;
    return ga->size;
}

