// Distributed two-dimensional Discrete FFT transform
// Gauresh Dilip Vanjare
// ECE8893 Project 1


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <signal.h>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include "Complex.h"
#include "InputImage.h"

using namespace std;
Complex* input;

Complex* ROW1D_T;
Complex* ROW1D;
Complex* COL1D;
Complex* AFTER2D;

Complex* IROW1D;
Complex* IROW1D_T;
Complex* ICOL1D;
Complex* IAFTER2D;

Complex* N;




InputImage image("Tower.txt");

int w = image.GetWidth();
int hi = image.GetHeight();



void Transpose(Complex* IN, int w, Complex* OUT){

for(int r=0; r<w; r++){
  for(int c=0; c<w; c++){
      OUT[r+w*c]=IN[r*w+c];  
  }

}

}


	
void Transform2D(const char* inputFN) 
{ // Do the 2D transform here.
  // 1) Use the InputImage object to read in the Tower.txt file and
  //    find the width/height of the input image.
  // 2) Use MPI to find how many CPUs in total, and which one
  //    this process isSSS
  // 3) Allocate an array of Complex object of sufficient size to
  //    hold the 2d DFT results (size is width * height)
  // 4) Obtain a pointer to the Complex 1d array of input data
  // 5) Do the individual 1D transforms on the rows assigned to your CPU
  // 6) Send the resultant transformed values to the appropriate
  //    other processors for the next phase.
  // 6a) To send and receive columns, you might need a separate
  //     Complex array of the correct size.
  // 7) Receive messages from other processes to collect your columns
  // 8) When all columns received, do the 1D transforms on the columns
  // 9) Send final answers to CPU 0 (unless you are CPU 0)
  //   9a) If you are CPU 0, collect all values from other processors
  //       and print out with SaveImageData().
  InputImage image(inputFN);  // Create the helper object for reading the image
  // Step (1) in the comments is the line above.
  // Your code here, steps 2-9
}

void Transform1D(Complex* IN,  int w, Complex* OUT)
{
int N=w;
  	for (int k=0;k<N;++k){
      //  H[k]=0;
        for (int n=0;n<N;++n){
            OUT[k]=OUT[k]+Complex(+cos(2.0*M_PI*n*k/N),-sin(2.0*M_PI*n*k/N))*IN[n];
         }
    if(fabs(OUT[k].imag)<1e-10) OUT[k].imag=0;
    if(fabs(OUT[k].real)<1e-10) OUT[k].real=0;
    }
}


void TransformiDFT(Complex* IN,  int w, Complex* OUT)
{
    int N=w;
  	for (int k=0;k<N;++k){
     
        for (int n=0;n<N;++n){
            OUT[k]=OUT[k]+(Complex(+cos(2.0*M_PI*n*k/N)/N,sin(2.0*M_PI*n*k/N)/N)*IN[n]);
         }
    if(fabs(OUT[k].imag)<1e-10) OUT[k].imag=0;
    if(fabs(OUT[k].real)<1e-10) OUT[k].real=0;
    }
}


int get=0;

int main(int argc, char** argv)
{
  int  numtasks, rank, rc; 
  string fn("Tower.txt"); /* default file name*/
  if (argc > 1) { fn = string(argv[1]);  /* if name specified on cmd line*/ }
  rc = MPI_Init(&argc,&argv);
  if (rc != MPI_SUCCESS) { printf ("Error starting MPI program. Terminating.\n"); MPI_Abort(MPI_COMM_WORLD, rc);}
  
  
  MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
  MPI_Comm_rank(MPI_COMM_WORLD,&rank);
  printf ("Number of tasks= %d My rank= %d\n", numtasks,rank);
	
 input = new Complex[w*hi];
 N = new Complex[w*16];
 	
 ROW1D=new Complex[w*hi];
 ROW1D_T=new Complex[w*hi];
 COL1D=new Complex[w*hi];
 AFTER2D=new Complex[w*hi];
 

 	
 IROW1D=new Complex[w*hi];
 IROW1D_T=new Complex[w*hi];
 ICOL1D=new Complex[w*hi];
 IAFTER2D=new Complex[w*hi];


	input=image.GetImageData();
  int rloop,iter,nRows;

  nRows=hi/numtasks;
  int next_row= w*nRows;

 
//MPI_Status status;
for(rloop=0;rloop<numtasks;rloop++){
    Transform1D( input+rank*next_row+w*rloop,  w, (ROW1D+rank*next_row+w*rloop));  
          }


if(rank!=0){
    rc = MPI_Send(ROW1D+(rank)*next_row, 2*sizeof(ROW1D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
    }
else{
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
			MPI_Status status;
            rc = MPI_Recv(ROW1D+(iter+1)*next_row, 2*sizeof(ROW1D)*next_row, MPI_CHAR, iter+1,0, MPI_COMM_WORLD, &status);
            if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
           }
          }
          
    Transpose(ROW1D, w, ROW1D_T);
           
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
           rc = MPI_Send(ROW1D_T+(iter+1)*next_row, 2*sizeof(ROW1D_T)*next_row, MPI_CHAR, iter+1 ,0, MPI_COMM_WORLD);
           if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
          }
         }   
     }      
                
if(rank!=0){
	MPI_Status status;
    rc = MPI_Recv(ROW1D_T+(rank)*next_row, 2*sizeof(ROW1D_T)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD, &status);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
                   

}          


  
for(rloop=0;rloop<numtasks;rloop++){
    Transform1D( ROW1D_T+rank*next_row+w*rloop,  w, (COL1D+rank*next_row+w*rloop));  
    }
  
  
if(rank!=0){  
    rc = MPI_Send(COL1D+(rank)*next_row, 2*sizeof(COL1D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
 
 
  }
else{

    for(iter=0;iter<numtasks;iter++){
		if(iter!=(numtasks-1)){
			MPI_Status status;
            rc = MPI_Recv(COL1D+(iter+1)*next_row, 2*sizeof(COL1D)*next_row, MPI_CHAR, iter+1,0, MPI_COMM_WORLD,&status);
			if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " recv failed, rc " << rc << endl;MPI_Finalize();}  
			}
         }
         
    Transpose(COL1D, w, AFTER2D);
    image.SaveImageData("MyAfter2D.txt",AFTER2D, w,w);
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
           rc = MPI_Send(AFTER2D+(iter+1)*next_row, 2*sizeof(AFTER2D)*next_row, MPI_CHAR, iter+1 ,0, MPI_COMM_WORLD);
           if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
           }
        }   
    }   
          
		  
if(rank!=0){
	MPI_Status status;
    rc = MPI_Recv(AFTER2D+(rank)*next_row, 2*sizeof(AFTER2D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD, &status);
              if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}

}          

//----------------------------------------------------------IDFT----------------------------------------------------------------------         
  
for(rloop=0;rloop<numtasks;rloop++){
    TransformiDFT( AFTER2D+rank*next_row+w*rloop,  w, (IROW1D+rank*next_row+w*rloop));  
          }

if(rank!=0){
    rc = MPI_Send(IROW1D+(rank)*next_row, 2*sizeof(IROW1D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
    }

else{
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
			MPI_Status status;
            rc = MPI_Recv(IROW1D+(iter+1)*next_row, 2*sizeof(IROW1D)*next_row, MPI_CHAR, iter+1,0, MPI_COMM_WORLD, &status);
            if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
           }
          }
          
    Transpose(IROW1D, w, IROW1D_T);
           
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
			rc = MPI_Send(IROW1D_T+(iter+1)*next_row, 2*sizeof(IROW1D_T)*next_row, MPI_CHAR, iter+1 ,0, MPI_COMM_WORLD);
            if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
            }
		}    
    }      
          
          
if(rank!=0){
	MPI_Status status;
    rc = MPI_Recv(IROW1D_T+(rank)*next_row, 2*sizeof(IROW1D_T)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD, &status);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
}          


for(rloop=0;rloop<numtasks;rloop++){
    TransformiDFT( IROW1D_T+rank*next_row+w*rloop,  w, (ICOL1D+rank*next_row+w*rloop));  
    }
  
  
if(rank!=0){  
    rc = MPI_Send(ICOL1D+(rank)*next_row, 2*sizeof(ICOL1D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
  }
  
else{
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
         MPI_Status status;
         rc = MPI_Recv(ICOL1D+(iter+1)*next_row, 2*sizeof(ICOL1D)*next_row, MPI_CHAR, iter+1,0, MPI_COMM_WORLD,&status);
         if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " recv failed, rc " << rc << endl;MPI_Finalize();}  
          }
        }
          
          
    Transpose(ICOL1D, w, IAFTER2D);
    image.SaveImageData("MyAfterInverse.txt",IAFTER2D, w,w);
    for(iter=0;iter<numtasks;iter++){
        if(iter!=(numtasks-1)){
        rc = MPI_Send(IAFTER2D+(iter+1)*next_row, 2*sizeof(IAFTER2D)*next_row, MPI_CHAR, iter+1 ,0, MPI_COMM_WORLD);
        if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}
           }    
        }
    }   
          
if(rank!=0){
	MPI_Status status;
	rc = MPI_Recv(IAFTER2D+(rank)*next_row, 2*sizeof(IAFTER2D)*next_row, MPI_CHAR, 0,0, MPI_COMM_WORLD, &status);
    if (rc != MPI_SUCCESS){cout << "Rank " << rank<< " send failed, rc " << rc << endl;MPI_Finalize();}

}          
     

//  Transform2D(fn.c_str()); /* Perform the transform.*/
  
  /* Finalize MPI here*/
  MPI_Finalize();
}  


  

