#include "mpi.h"
#include <stdio.h>

main(int argc, char *argv[])  {
    int numtasks, rank, source=0, dest, tag=1, i;
    
    typedef struct
{
    char c;
    int i[2];
    float f[4];
} dd;

dd s,p,s1,p1;
MPI_Init(&argc,&argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
int blockcounts1[3];
blockcounts1[0]=1;
blockcounts1[1]=2;
blockcounts1[2]=4;

MPI_Aint    offsets1[3], block1_address,block2_address,block3_address;
offsets1[0] = 0;
MPI_Get_address(&s.c, &block1_address);
MPI_Get_address(s.i, &block2_address);
offsets1[1] = block2_address-block1_address;
MPI_Get_address(s.f, &block3_address);
offsets1[2] = block3_address-block1_address;

MPI_Datatype  particletype,oldtypes[3];
oldtypes[0] = MPI_CHAR;
oldtypes[1] = MPI_INT;
oldtypes[2] = MPI_FLOAT;

MPI_Type_struct(3, blockcounts1, offsets1, oldtypes, &particletype);
MPI_Type_commit(&particletype);

// task 0 initializes the particle array and then sends it to each task
if (rank == 0) {
    
     s.c='A';
     s.i[0]=1;
     s.i[1]=2;
     s.f[0]=1.2;
     s.f[1]=2.3;
     s.f[2]=1.5;
     s.f[3]=2.1;
    //  s.f=[1.2,3.1,4.1,1.9];
       
    for (i=0; i<numtasks; i++) 
       MPI_Send(&s, 1, particletype, i, tag, MPI_COMM_WORLD);
    }
MPI_Status stat;    
MPI_Recv(&p, 1, particletype, source, tag, MPI_COMM_WORLD, &stat);
printf("rank= %d got point to point message c= '%c' i= [%d %d] f= [%f %f %f %f]\n", rank,p.c,p.i[0],p.i[1],p.f[0],p.f[1],p.f[2],p.f[3]);
if (rank == 0) {
    
     s1.c='B';
     s1.i[0]=33;
     s1.i[1]=21;
     s1.f[0]=121.2;
     s1.f[1]=22.3;
     s1.f[2]=11.5;
     s1.f[3]=21.1;
    //  s.f=[1.2,3.1,4.1,1.9];
    
    }

MPI_Bcast(&s1, 1, particletype, 0, MPI_COMM_WORLD);
printf("rank= %d got Broadcast message c= '%c' i= [%d %d] f= [%f %f %f %f]\n", rank,s1.c,s1.i[0],s1.i[1],s1.f[0],s1.f[1],s1.f[2],s1.f[3]);
// free datatype when done using it
MPI_Type_free(&particletype);
MPI_Finalize();
} 
