// Calculate and display the Mandelbrot set
// ECE4893/8893 final project, Fall 2011

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "complex.h"
//#include<cmath>


pthread_barrier_t barr;
int nThreads =16;


using namespace std;


Complex  minC(-2.0, -1.2);
Complex  maxC( 1.0, 1.8);
int      maxIt = 2000;     // Max iterations for the set computations
int Winw = 512;
int Winh = 512;

  
  
static GLfloat updateRate = 10.0;
static GLuint maxDepth =4;


double G[2001];
double R[2001];
double B[2001];
int x_press=-1,y_press=-1,x_rel=-1,y_rel=-1;

int global_arr[512][512];
    
typedef struct blabla{
  Complex minval[3000]; 
  Complex maxval[3000];
  int position;
}zin;

zin zn;
 
 
 
 void* Transform2DINVTHread(void* v)
{ 
	
	unsigned long myId = (unsigned long)v;
  //cout<<myId<<endl;

  
  double stepx;
  double stepy;

  
  stepx = (maxC.real - minC.real)/Winw;
  stepy = (maxC.imag - minC.imag)/Winh;
     
	//mandelbrot(present_index,myId);
  Complex c(minC.real + stepx * (myId+1)*32,minC.imag);
  bool flag = false;
    
  for(int i=myId *32; i <  (myId+1)*32 ; i++){
        
       c.real=c.real+stepx;
       c.imag = maxC.imag;
       
       for(int j = 0;j<Winh ;j++){
          c.imag=c.imag-stepy;
          //c = Complex(c.real,c.imag);

          Complex Zn(0,0);
   
          for (int k = 0; k < 2000 ; k++){

              Zn = Zn * Zn + c;

              if(Zn.Mag().real > 2.0 ){
                  
                  global_arr[i][j]=k;
                  flag=true;
                  break;

               }
              
           }
           if(flag==false){
                    global_arr[i][j]=2000;
           }
           flag=false;

      }

    }


  //	cout<<"after" <<endl;
   int rc = pthread_barrier_wait(&barr);
 

	//barrier->Enter(myId);
// return 0;
}

void static_color(){
// srand(time(NULL));
  for(int i=0;i<2000;i++){
    R[i]=drand48();
    G[i]=drand48();
    B[i]=drand48();

  }
  R[2000]=0.0;
  G[2000]=0.0;
  B[2000]=0.0;
}





void display(void)
{ // Your OpenGL display code

  glEnable(GL_LINE_SMOOTH);
  glClear(GL_COLOR_BUFFER_BIT );
 
	glLoadIdentity();
	//set the viewing transformation
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  	 for (int i = 0; i < nThreads; ++i)
        {
                pthread_t pt;
                pthread_create(&pt, 0, Transform2DINVTHread, (void*)i);
        }
  pthread_barrier_wait(&barr);

  //put barrier
    cout<<"I am here"<<endl;
   glBegin(GL_POINTS);
   for(int i=0;i<Winw;i++){
      for(int j=0;j<Winh;j++){  
          
         glColor3f(R[global_arr[i][j]],G[global_arr[i][j]],B[global_arr[i][j]]);
         glVertex2f(i,j);
        
  
         }
    }
    
   glEnd();
   
 
  //drawCircle();
  //keglPopMatrix();

  //glFlush();

   glutSwapBuffers();
}

void init()
{ // Your OpenGL initialization code here
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

}

void reshape(int w, int h)
{ // Your OpenGL window reshape code here
  glViewport(0,0, (GLsizei)Winw, (GLsizei)Winh);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)Winw, (GLdouble)0.0, Winh, (GLdouble)-Winw, (GLdouble)Winw);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}



void mouse(int button, int state, int x, int y)
{ // Your mouse click processing here
  // state == 0 means pressed, state != 0 means released
  // Note that the x and y coordinates passed in are in
  // PIXELS, with y = 0 at the top.
 
  //cout<<"in"<<endl;


  int x1=-1,y1=-1,x2=-1,y2=-1;
  
  int x3=-1,y3=-1,x4=-1,y4=-1; 
  
   
  if(button == GLUT_LEFT && state == GLUT_DOWN){
    //x1 = (double(((x - 0) * (1.0 - (-0.2)))) / double((512 - 0))) + (-0.2)
    x_press = x;
    y_press = y;
    /*
    x1 = minC.real + ((maxC.real - minC.real)/Winw )*x;
    y1 = minC.imag + ((maxC.imag - minC.imag)/Winh )*y;
     */
  }
  else if(button == GLUT_LEFT && state == GLUT_UP){
  
    x_rel = x;
    y_rel = y;  
  /*
    x2 = minC.real + ((maxC.real - minC.real)/Winw )*x;
    y2 = minC.imag + ((maxC.imag - minC.imag)/Winh )*y;
    */
    if(x_press < x_rel){
      x1=x_press;
      x2=x_rel;
    }  
    else{
      x1=x_rel;
      x2=x_press;
    }
    
    if(y_press < y_rel){
      y1=y_press;
      y2=y_rel;
    }  
    else{
      y1=y_rel;
      y2=y_press;
    }
    
    
    x3 = x1;
    y3 = y1;
    x4 = x2;
    y4 = y2;
    printf("x1 %d y1 %d x2 %d y2 %d",x3,y3,x4,y4);
    
    if(abs(x1-x2)>abs(y1-y2)){
        
            x4 = x1 + abs(y2-y1);
            x3 = x1;
    
    }
    
    else if(abs(x1-x2)<abs(y1-y2)){
    
            y4 = y1 + abs(x2-x1);
             x4 = x2;
    }
    
    
    printf("x1 %d y1 %d x2 %d y2 %d",x3,y3,x4,y4);
    cout<<" "<<endl;
    
    
    zn.minval[zn.position]=minC;
    zn.maxval[zn.position]=maxC;
    zn.position++;
    
    double temp_minc_real = minC.real;
    double temp_minc_imag = minC.imag;
    
    minC.real = minC.real + ((maxC.real - minC.real)/Winw )*x3;
    minC.imag = minC.imag + ((maxC.imag - minC.imag)/Winh )*y3;
  
    
    
    maxC.real = minC.real + ((maxC.real - temp_minc_real)/Winw )*x4;
    maxC.imag = minC.imag + ((maxC.imag - temp_minc_imag)/Winh )*y4;
     
    glutPostRedisplay();
     
    

  }
  
}

void motion(int x, int y)
{ // Your mouse motion here, x and y coordinates are as above

}

void keyboard(unsigned char c, int x, int y)
{ // Your keyboard processing here
   
    if(c=='b' || c=='B'){
      if (zn.position !=0){
          minC = zn.minval[zn.position-1];
          maxC = zn.maxval[zn.position-1];
          zn.position--;
      }
    
    glutPostRedisplay();    
    }

}

void timer(int)
{
  glutPostRedisplay();
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}
int main(int argc, char** argv)
{
 
  //zn.position=0;
  
  // Barrier initialization
  pthread_barrier_init(&barr, 0, nThreads+1);
    
  // Initialize OpenGL, but only on the "master" thread or process.
  // See the assignment writeup to determine which is "master"
  // and which is slave.
  int initx=100;
  int inity=100;
  static_color();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB );

  glutInitWindowSize(Winw, Winh);
  glutInitWindowPosition(initx, inity);
  glutCreateWindow("MandelBrot");
  init();

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutKeyboardFunc(keyboard);
  
 // glutTimerFunc(1000.0 / updateRate, timer, 0);
  glutMainLoop();
  return 0;

}
