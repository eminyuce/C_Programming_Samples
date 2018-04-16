/******************************************************************************
* FILE: mm.c
* DESCRIPTION:  
*   A MPI program that implements Gauss Elimination.
* AUTHOR: Emin Yüce 
******************************************************************************/

#include <stdio.h>
#include "mpi.h"

#define S 10 /* number of rows in matrix */
#define MASTER 0 /* taskid of first task */


void readMatrix();
void writeMatrix();
void readVector();
void writeVector();
void GaussianElimination(int,int,MPI_Status);

float A [S][S]; //A matrix
float V [S];    //Temp matrix for gaussion eliminitaion
float B [S];	//B matrix


main(int argc, char* argv[]) {
	int numtasks; 			//number of tasks  
	int taskid;  			//tasks identifier
	int source,			//task id of message source 
	destination;			//task id of message destination
	double first, last; 		//a variable to calculate elapsed time
	MPI_Status status;
	int i, j, k;			//General variable to make operations
		
	//Initalize the processes task id and tasks identifier
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    	/**************************** master task ************************************/
	/*Process 0 send matrix data to the slaves */
	if (taskid == MASTER) {	    
		//Read a matrix from an input file called "matrix.in" 
		readMatrix();
		//And a vector from an input file called "vector.in"
		readVector();

		//Measure the overall time spent for the parallel execution.
		first = MPI_Wtime();

		//Process 0 are striped among the given 
		//number of processors with row-wise cycling striping.
		//Process 0 sends each process only its related part
		for (i=1; i<S; i++) {   
			destination = i % numtasks;
			if(destination != MASTER){
				MPI_Send(A[i], S, MPI_FLOAT, destination, 0, MPI_COMM_WORLD);
				MPI_Send(&B[i], 1, MPI_FLOAT, destination, 1, MPI_COMM_WORLD);
			}
		}	
		
	}else{
		
		for (i=1; i<S; i++) {
			source = i % numtasks;
			if(source == taskid){
				MPI_Recv(A[i], S,  MPI_FLOAT, MASTER, 0, MPI_COMM_WORLD, &status);
				MPI_Recv(&B[i], 1, MPI_FLOAT, MASTER, 1, MPI_COMM_WORLD,&status);
			}
		}
	}
	//Parallel Gaussian elimination algorithm that converts the system of linear equations Ax=b to a
	// unit triangular system Ux=y.
	GaussianElimination(taskid,numtasks,status);
	
		//Process 0 gets partial results from slaves processes
		for (i=1; i<S; i++) {   
			destination = i % numtasks;
			
			if(destination == taskid){
				if(destination != MASTER){
				MPI_Send(A[i], S, MPI_FLOAT, MASTER, 3, MPI_COMM_WORLD);
				MPI_Send(&B[i], 1, MPI_FLOAT, MASTER, 4, MPI_COMM_WORLD);
				}
			}else if(taskid == MASTER){
				MPI_Recv(A[i], S,  MPI_FLOAT,destination, 3, MPI_COMM_WORLD, &status);
				MPI_Recv(&B[i], 1,  MPI_FLOAT, destination,4, MPI_COMM_WORLD, &status);
			}
		}
		
	

	//MASTER process 0 writes result matrix and result vertor in files called "matrix.out"
	// and "vector.out" respectively
	if (taskid == MASTER) {
		last = MPI_Wtime();
		printf("Elapsed Time of Computation  is: %f\n", last - first);
		writeMatrix();
		writeVector();

	}
	MPI_Finalize();
}  /* main */

void GaussianElimination(int taskid, int pSize,MPI_Status status){
	int i, j, k, n,mod,destination;
	n=S;
	for(k=0; k<n; k++){	// Outer loop
		mod = k % pSize;
		if(taskid  == mod){
			for(j=k+1; j<n; j++){
				
				if(A[k][k] != 0){ 
					A[k][j] = A[k][j]/A[k][k];	// Division step	
				}else{
					printf("Error : Divided by zero.\n");
				}
						
			}
			if(A[k][k] != 0){
				V[k] = B[k]/A[k][k];
			}else{
				printf("Error : Divided by zero.\n");
			}
				A[k][k] = 1;

			if (k + 1 < S) {

			MPI_Send(A[k], S, MPI_FLOAT, (taskid+1) % pSize, 8, MPI_COMM_WORLD);
			MPI_Send(&V[k], 1, MPI_FLOAT, (taskid+1) % pSize, 9, MPI_COMM_WORLD);

			}
		}
		
		
		//MPI_Bcast(A[k], S, MPI_FLOAT, mod , MPI_COMM_WORLD);
		

		for(i=k+1; i<n; i++){
			if(taskid  == i % pSize){
			
			if(taskid > 0){
				destination = taskid -1; 
			}else{
				destination = pSize - 1;	
			}
			MPI_Recv(A[k],S,MPI_FLOAT, destination,8, MPI_COMM_WORLD, &status);
			MPI_Recv(&V[k],1,MPI_FLOAT, destination,9, MPI_COMM_WORLD, &status);
				

			if (i + 1 < S) {

			MPI_Send(A[k], S, MPI_FLOAT, (taskid+1) % pSize, 8, MPI_COMM_WORLD);
			MPI_Send(&V[k], 1, MPI_FLOAT, (taskid+1) % pSize, 9, MPI_COMM_WORLD);

			}

			for(j=k+1; j<n; j++){
				A[i][j] = A[i][j]- A[i][k] * A[k][j];	// Elimination step	
			}	
			B[i] = B[i] - A[i][k] * V[k];
			A[i][k] = 0;
			
			

			}
		}	
	}
}
void readMatrix(){
	
	FILE * f;
	int i, j;
	f = fopen("matrix.in","r");
	if(f == NULL){
		printf("Error : File not found\n");
		return;
	}	
	for (i = 0; i < S; i++){
		for (j = 0; j < S; j++){			
			fscanf(f,"%f",&A[i][j]);							
		}
	}	
	fclose(f);
}
void readVector(){
	
	FILE * f;
	int i, j;

	f = fopen("vector.in","r");
	if(f == NULL){
		printf("Error : File not found\n");
		return;
	}	
	for (i = 0; i < S; i++){
		fscanf(f,"%f",&B[i]);							
	}	
	fclose(f);
}
void writeVector(){	
	FILE * f;
	int i, j;	

	f = fopen("vector.out","w");
	if(f == NULL){
		printf("Error : File not found\n");
		return;
	}	
	for (i = 0; i < S; i++){
		fprintf(f,"%2.1f ",B[i]);			
	}
	
	fclose(f);
}
void writeMatrix(){	
	FILE * f;
	int i, j;	

	f = fopen("matrix.out","w");
	if(f == NULL){
		printf("Error : File not found\n");
		return;
	}	
	for (i = 0; i < S; i++){
		for (j = 0; j < S; j++){			
			fprintf(f,"%2.1f",&A[i][j]);							
		}		
	}
	fclose(f);
}
