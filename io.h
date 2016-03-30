#ifndef __IO_H__
#define __IO_H__

#include "structs.h"

workload read_vectors_from_file(char* filename);
workload read_vectors_from_file_parallel(char* filename);
void log_worktimes(worktime* worktimes, int count);

#endif // __IO_H__
