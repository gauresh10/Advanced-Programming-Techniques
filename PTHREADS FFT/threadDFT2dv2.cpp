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


class GoodBarrier {
public:
      GoodBarrier(int P0);   // P is the total number of threads
      void Enter(int myId);  // Enter the barrier, don’t exit till alll there
      private:
      int  P;
      int  count;             // Number of threads presently in the barrier
      int FetchAndDecrementCount();
      pthread_mutex_t countMutex;
      bool* localSense;       // We will create an array of bools, one per thread
      bool  globalSense;      // Global sense
};

GoodBarrier::GoodBarrier(int P0): P(P0), count(P0)
{
    pthread_mutex_init(&countMutex, 0);
    localSense = new bool[P];
    for (int i = 0; i < P; ++i) localSense[i] = true;
    globalSense = true;
}
void GoodBarrier::Enter(int myId)
{
    localSense[myId] = !localSense[myId];
    if (FetchAndDecrementCount() == 1)
        {
	//pthread_cond_signal(&exitCond);
 	//pthread_mutex_unlock(&exitMutex);
        count = P;
        globalSense = localSense[myId];
        //cout<<"exiting final stage reached"<<endl;
	}
    else
        {
        while (globalSense != localSense[myId]) {  }
        }
}
int GoodBarrier::FetchAndDecrementCount()
{
    pthread_mutex_lock(&countMutex);
    int myCount = count;
    count--;
    pthread_mutex_unlock(&countMutex);
    return myCount;
}




GoodBarrier *barrier = new GoodBarrier(nThreads + 1);







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
void* Transform2DINVTHread(void* v){
	unsigned long myId = (unsigned long)v;
        int rowsPerThread = hi / nThreads;

        for(int ro=0;ro<rowsPerThread;ro++){
                Transform1DINV(&h[myId*rowsPerThread*w+ro*w],w);
        }
        cout << (unsigned long)v << " id is done with I FFT"<< endl;
        barrier->Enter(myId);
return 0;
}
void* Transform2DTHread(void* v)
{

	unsigned long myId = (unsigned long)v;
	int rowsPerThread = hi / nThreads;

	for(int ro=0;ro<rowsPerThread;ro++){
		Transform1D(&h[myId*rowsPerThread*w+ro*w],w);
	}
  	cout << (unsigned long)v << " id is done with FFT"<< endl;
	barrier->Enter(myId);
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

 	for (int i = 0; i < nThreads; ++i)
    	{
       		pthread_t pt;
       		pthread_create(&pt, 0, Transform2DTHread, (void*)i);
     	}


	barrier->Enter(nThreads);
	cout << "all done 1d" << endl;
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
	for (int i = 0; i < nThreads; ++i)
        {
                pthread_t pt;
                pthread_create(&pt, 0, Transform2DTHread, (void*)i);
        }

	//startCount =nThreads;
	cout<<"Begin 2d"<<endl;
	barrier->Enter(nThreads);

	Transpose(h,w,temp_data);
 	image.SaveImageData("save12.txt", temp_data,  w,  hi);

	//iFFT

 	for(int out=0;out<w;out++){
                for(int in=0;in<hi;in++){
                        h[in+out*w] =  temp_data[out*w+ReverseBits(in)];
                }
        }
	 for (int i = 0; i < nThreads; ++i)
        {
                pthread_t pt;
                pthread_create(&pt, 0, Transform2DINVTHread, (void*)i);
        }

        cout<<"Begin 1d iFFT"<<endl;
         barrier->Enter(nThreads);

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
	 for (int i = 0; i < nThreads; ++i)
        {
                pthread_t pt;
                pthread_create(&pt, 0, Transform2DINVTHread, (void*)i);
        }

        startCount =nThreads;
        cout<<"Begin 2d ifft "<<endl;
        barrier->Enter(nThreads);

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
}
