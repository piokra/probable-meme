#ifndef __IO_H__
#define __IO_H__

#include "structs.h"

workload read_vectors_from_file(char* filename, int rank, int size);
workload read_vectors_from_file_parallel(char* filename, int rank, int size);
void log_worktimes(worktime* worktimes, int count, char* tag);

#endif // __IO_H__
