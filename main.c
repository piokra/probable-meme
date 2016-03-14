#include <mpi.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//defines
#define BIG_NUM (1000000033)
//structures


//global variables
int rank;
int size;
//general
void do_what_master_does();
void do_what_other_do();
//communication
void communicate_as_master(double* to_send, double** matrices);
void communicate_as_odd(int to, double* to_send, double* recv_buff);
void communicate_as_even(int to, double* to_send, double* recv_buff);
//out
void write_matrix_to_file(FILE* file, double* matrix, int size);
void write_line_to_file(FILE* file, char* line);
void write_rank_info_to_file(FILE* file, int from);
//matrix
double* allocate_square_matrix(int size);
void randomize_square_matrix(double* matrix, int size);
//utility
void set_random_seed();
char* rank_to_filename(int rank);

int main(int argc, char** argv)
{
    MPI_Init(&argc,&argv);

    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&size);

    set_random_seed();

    if(rank==0)
    {
        do_what_master_does();
    }
    else
    {
        do_what_other_do();
    }

    MPI_Finalize();
}
//general
void do_what_master_does()
{
    double* matrices[2];
    double* my_matrix = allocate_square_matrix(size);
    randomize_square_matrix(my_matrix,size);
    communicate_as_master(my_matrix,matrices);

    char* filename = rank_to_filename(rank);
    FILE* file = fopen(filename,"w");

    write_line_to_file(file,"\t\t=== My Matrix ===");
    write_matrix_to_file(file,my_matrix,size);
    write_line_to_file(file,"\t\t=== Received Matrix ===");
    write_matrix_to_file(file,matrices[0],size);
    write_rank_info_to_file(file,rank+1);
    free(matrices[0]);

    if(matrices[1]!=0)
    {
        write_line_to_file(file,"\t\t=== Received Matrix ===");
        write_matrix_to_file(file,matrices[1],size);
        write_rank_info_to_file(file,size-1);
        free(matrices[1]);
    }

    free(filename);
    free(my_matrix);
    fclose(file);

}

void do_what_other_do()
{
    double* my_matrix = allocate_square_matrix(size);
    double* matrix_buf = allocate_square_matrix(size);
    randomize_square_matrix(my_matrix,size);

    if(rank%2==0)
    {
        communicate_as_even((rank+1)%size,my_matrix,matrix_buf);
    }
    else
    {
        communicate_as_odd(rank-1,my_matrix,matrix_buf);
    }

    char* filename = rank_to_filename(rank);
    FILE* file = fopen(filename,"w");

    write_line_to_file(file,"\t\t=== My Matrix ===");
    write_matrix_to_file(file,my_matrix,size);
    write_line_to_file(file,"\t\t=== Received Matrix ===");
    write_matrix_to_file(file,matrix_buf,size);
    if(rank%2==0)
        write_rank_info_to_file(file,(rank+1)%size);
    else
        write_rank_info_to_file(file,rank-1);

    fclose(file);
    free(my_matrix);
    free(matrix_buf);
    free(filename);

}
//communication
void communicate_as_master(double* to_send, double** matrices)
{
    matrices[0] = 0;
    matrices[1] = 0;

    if(size%2!=0)
    {
        matrices[0] = allocate_square_matrix(size);
        matrices[1] = allocate_square_matrix(size);
        communicate_as_even(1,to_send,matrices[0]);
        communicate_as_odd(size-1,to_send,matrices[1]);
    }
    else
    {
        matrices[0] = allocate_square_matrix(size);
        communicate_as_even(1,to_send,matrices[0]);
    }
}
void communicate_as_even(int to, double* to_send, double* recv_buf)
{
    MPI_Status status;
    MPI_Send(to_send,size*size,MPI_DOUBLE,to,0,MPI_COMM_WORLD);
    MPI_Recv(recv_buf,size*size,MPI_DOUBLE,to,0,MPI_COMM_WORLD,&status);
}
void communicate_as_odd(int to, double* to_send, double* recv_buf)
{
    MPI_Status status;
    MPI_Recv(recv_buf,size*size,MPI_DOUBLE,to,0,MPI_COMM_WORLD,&status);
    MPI_Send(to_send,size*size,MPI_DOUBLE,to,0,MPI_COMM_WORLD);
}
//out
void write_matrix_to_file(FILE* file, double* matrix, int size)
{
    for(int i=0; i<size; i++)
    {
        for(int j=0; j<size; j++)
        {
            fprintf(file,"%f ",matrix[i*size+j]);
        }
        fprintf(file,"\n");
    }
}
void write_line_to_file(FILE* file,char* line)
{
    fprintf(file,"%s\n",line);
}
void write_rank_info_to_file(FILE* file,int from)
{
    fprintf(file,"Received from: %d.\n",from);
}
//matrix
double* allocate_square_matrix(int size)
{
    return malloc(size*size*sizeof(double));
}

void randomize_square_matrix(double* matrix, int size)
{
    double rand_maxd = RAND_MAX;
    for(int i=0; i<size*size; i++)
    {
        double t = rand();
        matrix[i] = t/rand_maxd;
    }
}
//utility

void set_random_seed()
{
    srand(BIG_NUM*time(0)*rank+rank);
}

char* rank_to_filename(int rank)
{
    char* buf = malloc(32);
    sprintf(buf,"log.proc_%d",rank);
    return buf;
}
