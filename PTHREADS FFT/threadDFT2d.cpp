// Threaded two-dimensional Discrete FFT transform
// GAURESH DILIP VANJARE
// ECE8893 Project 2


#include <iostream>
#include <string>
#include <math.h>

#include "Complex.h"
#include "InputImage.h"

using namespace std;

 pthread_mutex_t exitMutex;
 pthread_cond_t  exitCond;
 int             startCount;

Complex* input;
Complex* h;
Complex* temp_data;
int nThreads =16;
InputImage image("Tower.txt");
int w = image.GetWidth();
int hi = image.GetHeight();

unsigned ReverseBits(unsigned v)
{ //  Provided to students
  unsigned n = w; // Size of array (which is even 2 power k value)
  unsigned r = 0; // Return value

  for (--n; n > 0; n >>= 1)
    {
      r <<= 1;        // Shift return value
      r |= (v & 0x1); // Merge in next bit
      v >>= 1;        // Shift reversal value
    }
  return r;
}

// GRAD Students implement the following 2 functions.
// Undergrads can use the built-in barriers in pthreads.


// Call MyBarrier_Init once in main
void MyBarrier_Init()// you will likely need some parameters)
{
}

// Each thread calls MyBarrier after completing the row-wise DFT
void MyBarrier() // Again likely need parameters
{
}

void Transpose(Complex* IN, int w, Complex* OUT){
	for(int r=0; r<w; r++){
		for(int c=0; c<w; c++){
			OUT[r+w*c]=IN[r*w+c];
		}
	}
}

void Transform1D(Complex* h, int N)
{
	Complex* W=new Complex[w];
	Complex* temp=new Complex[w*hi];
 	for(int wg=0;wg<N;wg++){
                W[wg]=Complex(cos(2*M_PI*wg/N),-sin(2*M_PI*wg/N));
       		 }
 	for(int ff=2; ff<=N;ff=ff*2){
		for(int p=0; p<N;p=p+ff){
			for(int q=0;q<(ff/2);q++){
				temp[p+q]=h[p+q];
				h[p+q]=(h[p+q]) .operator+( W[((q%N)*N/ff)] .operator* (h[p+q+(ff/2)]));
				h[p+q+ff/2]=(temp[p+q]) .operator- (W[((q%N)*N/ff)] .operator* (h[p+q+(ff/2)]));
			}
		}
 	}
}

void Transform1DINV(Complex* h, int N)
{
        Complex* Wi=new Complex[w];
        Complex* temp=new Complex[w*hi];
        for(int wg=0;wg<N;wg++){
                Wi[wg]=Complex(cos(2*M_PI*wg/N),sin(2*M_PI*wg/N));
                 }
        for(int ff=2; ff<=N;ff=ff*2){
                for(int p=0; p<N;p=p+ff){
                        for(int q=0;q<(ff/2);q++){
                                temp[p+q]=h[p+q];
                                h[p+q]=(h[p+q]) .operator+( Wi[((q%N)*N/ff)] .operator* (h[p+q+(ff/2)]));
                                h[p+q+ff/2]=(temp[p+q]) .operator- (Wi[((q%N)*N/ff)] .operator* (h[p+q+(ff/2)]));
                        }
                }
        }

}

void* Transform2DTHread(void* v)
{

	unsigned long myId = (unsigned long)v;
	int rowsPerThread = hi / nThreads;
  	for(int ro=0;ro<rowsPerThread;ro++){
		Transform1D(&h[myId*rowsPerThread*w+ro*w],w);
	}

 	pthread_mutex_lock(&exitMutex);
  	startCount--;
  	cout << (unsigned long)v << " id is done with 1d"<< endl;


	if (startCount == 0)
    	{
	      	pthread_cond_signal(&exitCond);
      		pthread_mutex_unlock(&exitMutex);
    	}
  	else
    	{
     		 pthread_mutex_unlock(&exitMutex);
    	}

	while(startCount!=nThreads){}

        for(int ro=0;ro<rowsPerThread;ro++){
                Transform1D(&h[myId*rowsPerThread*w+ro*w],w);
        }

        pthread_mutex_lock(&exitMutex);
        startCount--;
        cout << (unsigned long)v << " id is done with 2d"<< endl;


        if (startCount == 0)
        {
                pthread_cond_signal(&exitCond);
                pthread_mutex_unlock(&exitMutex);
        }
        else
        {
                 pthread_mutex_unlock(&exitMutex);
        }

	//IFFT
	while(startCount!=nThreads){}

        for(int ro=0;ro<rowsPerThread;ro++){
                Transform1DINV(&h[myId*rowsPerThread*w+ro*w],w);
        }

        pthread_mutex_lock(&exitMutex);
        startCount--;
        cout << (unsigned long)v << " id is done with 1D IFFT"<< endl;


        if (startCount == 0)
        {
                pthread_cond_signal(&exitCond);
                pthread_mutex_unlock(&exitMutex);
        }
        else
        {
                 pthread_mutex_unlock(&exitMutex);
        }

	 while(startCount!=nThreads){}

        for(int ro=0;ro<rowsPerThread;ro++){
                Transform1DINV(&h[myId*rowsPerThread*w+ro*w],w);
        }

        pthread_mutex_lock(&exitMutex);
        startCount--;
        cout << (unsigned long)v << " id is done with 2d IFFT"<< endl;


        if (startCount == 0)
        {
                pthread_cond_signal(&exitCond);
                pthread_mutex_unlock(&exitMutex);
        }
        else
        {
                 pthread_mutex_unlock(&exitMutex);
        }

 return 0;
}

void Transform2D(const char* inputFN)
{ // Do the 2D transform here.
	InputImage image(inputFN);  // Create the helper object for reading the image
	input = new Complex[w*hi];
	temp_data=new Complex[w*hi];
	h=new Complex[w*hi];
 	input=image.GetImageData();
	//reorder
	for(int out=0;out<w;out++){
 		for(int in=0;in<hi;in++){
 			h[in+out*w] =  input[out*w+ReverseBits(in)];
   		}
 	}
	startCount= nThreads;
	//mutex init
 	pthread_mutex_init(&exitMutex,0);
 	pthread_cond_init(&exitCond, 0);
 	pthread_mutex_lock(&exitMutex);
	//create thread
 	for (int i = 0; i < nThreads; ++i)
    	{
       		pthread_t pt;
       		pthread_create(&pt, 0, Transform2DTHread, (void*)i);
     	}
 	pthread_cond_wait(&exitCond, &exitMutex);


	cout << "all done " << endl;
	//save 1D
	image.SaveImageData("save11.txt", h,  w,  hi);



	//transpose
	Transpose(h,w,temp_data);
	//reorder 2
	for(int out=0;out<w;out++){
                for(int in=0;in<hi;in++){
                        h[in+out*w] =  temp_data[out*w+ReverseBits(in)];
                }
        }
	startCount =nThreads;
	cout<<"Begin 2d"<<endl;
	pthread_cond_wait(&exitCond, &exitMutex);
	Transpose(h,w,temp_data);
 	image.SaveImageData("save12.txt", temp_data,  w,  hi);

	//iFFT

 	for(int out=0;out<w;out++){
                for(int in=0;in<hi;in++){
                        h[in+out*w] =  temp_data[out*w+ReverseBits(in)];
                }
        }

        startCount =nThreads;
        cout<<"Begin 1d iFFT"<<endl;
        pthread_cond_wait(&exitCond, &exitMutex);

	for(int di=0;di<w*hi;di++){
		h[di]=h[di]/w;
	}

	image.SaveImageData("save13.txt", temp_data,  w,  hi);
	Transpose(h,w,temp_data);

	for(int out=0;out<w;out++){
                for(int in=0;in<hi;in++){
                        h[in+out*w] =  temp_data[out*w+ReverseBits(in)];
                }
        }

        startCount =nThreads;
        cout<<"Begin 2d ifft "<<endl;
        pthread_cond_wait(&exitCond, &exitMutex);

	for(int di=0;di<w*hi;di++){
              h[di]=h[di]/w;
        }
	Transpose(h,w,temp_data);

        image.SaveImageData("save14.txt", temp_data,  w,  hi);


}



int main(int argc, char** argv)
{
  string fn("Tower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line
  // MPI initialization here
  Transform2D(fn.c_str()); // Perform the transform.

 /*
Transpose(h,w,ROW1D_T);


 for(int out=0;out<w;out++){
        for(int in=0;in<hi;in++){
                H[in+out*w] =  ROW1D_T[out*w+ReverseBits(in)];

        }
 }


for(int ro=0;ro<hi;ro++){
        for(int fft_size=2; fft_size<=w;fft_size=fft_size*2){
                        Transform1D(H+ro*w,fft_size);
                       }
        }


Transpose(H,w,AFTER2D);

image.SaveImageData("save2.txt", AFTER2D,  w,  hi);

*/


/*




//ifft
for(int out=0;out<w;out++){
        for(int in=0;in<hi;in++){
                ih[in+out*w] =  AFTER2D[out*w+ReverseBits(in)];

        }
 }


calcweightINV();

for(int ro=0;ro<hi;ro++){
        for(int fft_size=2; fft_size<=w;fft_size=fft_size*2){
                Transform1DINV(ih+ro*w,fft_size);
        }
}
for(int di=0;di<w*hi;di++){
ih[di]=ih[di]/w;
}


image.SaveImageData("save3.txt",ih,  w,  hi);


Transpose(ih,w,IROW1D_T);


 for(int out=0;out<w;out++){
        for(int in=0;in<hi;in++){
                iH[in+out*w] =  IROW1D_T[out*w+ReverseBits(in)];

        }
 }


for(int ro=0;ro<hi;ro++){
        for(int fft_size=2; fft_size<=w;fft_size=fft_size*2){
                        Transform1DINV(iH+ro*w,fft_size);
                                }
                                }

for(int di=0;di<w*hi;di++){
iH[di]=iH[di]/w;
}

Transpose(iH,w,IAFTER2D);

image.SaveImageData("save4.txt", IAFTER2D,  w,  hi);

 */
}
