// Draw an Icosahedron
// ECE4893/8893 Project 4
// YOUR NAME HERE
#include<stdio.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606
#define Z .850650808352039932


void Test1();
void Test2();
void Test3();
void Test4();
void Test5(int depth);
void Test6(int depth);
void subDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3,int depth);

// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };


int var=0;
int testNumber; // Global variable indicating which test number is desired
int cmd_depth;
float *R,*G,*B;
int power;
int Winw=500;
int Winh=500;
static GLfloat rotX = 0.0;
static GLfloat rotY = 0.0;
static GLfloat rotZ = 0.0;
static GLfloat dX = 1.0;
static GLfloat dY = 0.8;
static GLfloat dZ = 0.6;
static GLfloat updateRate = 10.0;

static GLuint maxDepth =4;






//helpers
void normalize(GLfloat v[3])
{
	GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	if(d == 0.0)
		return;
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;

}

void debugExtend(GLfloat* v, GLfloat extend)
{
  //return;
	GLfloat mag = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= mag * extend;
	v[1] /= mag * extend;
	v[2] /= mag * extend;
}


void drawIcosahedron(int maxDepth){
  for(int i=0;i<NFACE; i++)
  {
    subDivide(&vdata[tindices[i][0]][0],
              &vdata[tindices[i][1]][0],
              &vdata[tindices[i][2]][0],maxDepth);

  }


}


void drawTriangle(GLfloat* v1, GLfloat* v2, GLfloat* v3)
{
  glBegin(GL_TRIANGLES);
      //	glColor3f(0.0, 0.0, 0.0);	//White Triangles
     	glColor3f(R[var], G[var], B[var]);
      var++;
      //	glNormal3fv(v1);
      glVertex3fv(v1);	//we couuld comment these 3 lines of glNormal. It has only got to do wiht lighting
      //	glNormal3fv(v2);
      glVertex3fv(v2);
      //	glNormal3fv(v3);
      glVertex3fv(v3);
  glEnd();


	glBegin(GL_LINE_LOOP);
      glLineWidth(1.0);
      glColor3f(1.0, 1.0, 1.0);	//white lines
    	//glNormal3fv(v1);
      glVertex3fv(v1);	//we couuld comment these 3 lines. It has only got tot do wiht lighting
    	//glNormal3fv(v2);
    	glVertex3fv(v2);
    	//glNormal3fv(v3);
      glVertex3fv(v3);
	glEnd();
}


void subDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3,int depth)
{
  if(depth == 0 )
  {
    drawTriangle( v1, v2, v3);
    return;
  }

  GLfloat v12[3];
  GLfloat v23[3];
  GLfloat v31[3];
  for(int i =0;i<3;i++)
  {
    v12[i]  =  v1[i]  +  v2[i];
    v23[i]  =  v2[i]  +  v3[i];
    v31[i]  =  v3[i]  +  v1[i];
  }
  normalize(v12);
  normalize(v23);
  normalize(v31);

  subDivide(  v1,  v12,  v31,  depth-1);
  subDivide(  v2,  v23,  v12,  depth-1);
  subDivide(  v3,  v31,  v23,  depth-1);
  subDivide(  v12,  v23,  v31,  depth-1);
}




void display()
{
  glEnable(GL_LINE_SMOOTH);	//enable anti-aliasing
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  //glDepthFunc(GL_LESS);
  // Enable depth test

  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//clear all
  //glClear(GL_COLOR_BUFFER_BIT);

	//glClear(GL_DEPTH_BUFFER_BIT);
	//enable depth buffer
  //glEnable(GL_DEPTH_TEST);
	//draw white icosahedron
  //glColor3f(1.0, 1.0, 1.0);
	//clear the matrix
	glLoadIdentity();
	//set the viewing transformation
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 2.0, 0.0);
  glTranslatef(Winw/2.0, Winh/2.0, 0);
  glScalef(Winw/2.0, Winh/2.0, 10.0);
 	glPushMatrix();

  switch(testNumber){
  case 1:
    Test1();
    break;
  case 2:
    Test2();
    break;
  case 3:
    Test3();
    break;
  case 4:
    Test4();
    break;
  case 5:
    Test5(cmd_depth);
    break;
  case 6:
    Test6(cmd_depth);
    break;
  default:
    break;


  }
  glPopMatrix();

  glFlush();
  //glutSwapBuffers();
}

// Test cases.  Fill in your code for each test case
void Test1()
{
  var=0;
  for(int i=0;i<NFACE;i++){
    drawTriangle(&vdata[tindices[i][0]][0],
              &vdata[tindices[i][1]][0],
              &vdata[tindices[i][2]][0] );
  }
}

void Test2()
{
  glRotatef(rotX, 1.0, 0.0, 0.0);
  glRotatef(rotY, 0.0, 1.0, 0.0);
  rotX += 1.0;
  rotY += 1.0;

  Test1();
}

void Test3()
{

  var=0;
  drawIcosahedron(cmd_depth);
}

void Test4()
{
  glRotatef(rotX, 1.0, 0.0, 0.0);
  glRotatef(rotY, 0.0, 1.0, 0.0);
  rotX += 1.0;
  rotY+= 1.0;


  Test3();
}

void Test5(int depth)
{
  var=0;
  drawIcosahedron(depth);
}

void Test6(int depth)
{
  glRotatef(rotX, 1.0, 0.0, 0.0);
  glRotatef(rotY, 0.0, 1.0, 0.0);
  rotX += 1.0;
  rotY += 1.0;

  Test5(depth);
}


void timer(int)
{
  //var=0;

  glutPostRedisplay();
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}


void init()
{
  //select clearing (background) color
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);

}



void reshape(int wi, int hi)
{
  int w=500;
  int h=500;
  glViewport(0,0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (GLdouble)w, (GLdouble)0.0, h, (GLdouble)-w, (GLdouble)w);
  //glFrustum(0.0, (GLdouble)w, (GLdouble)0.0, h, (GLdouble)1, (GLdouble)40);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: icosahedron testnumber" << endl;
      exit(1);
    }
  // Set the global test number
  testNumber = atol(argv[1]);

  if(testNumber==5 || testNumber==6){
    if(argc<3) {
      std::cout << "Please Input Depth" << endl;
      exit(1);
      }
    cmd_depth = atol(argv[2]);
  }
  else if(testNumber==1 || testNumber==2){
    cmd_depth = 0;
  }
  else if(testNumber==3 || testNumber==4){
    cmd_depth = 1;
  }
  else{
  cout<<"Please enter a valid test number between 1 and 6"<<endl;
  exit(1);

  }


  power=20*pow(4, cmd_depth);

  R = (float*)calloc(power, sizeof(float));
  G = (float*)calloc(power, sizeof(float));
  B = (float*)calloc(power, sizeof(float));



  // Initialize glut  and create your window here
  // Set your glut callbacks here
  // Enter the glut main loop here
  int initx=100;
  int inity=100;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB |GLUT_DEPTH);
  glutInitWindowSize(Winw, Winh);
  glutInitWindowPosition(initx, inity);
  glutCreateWindow("Icosahedron");
  init();

   srand(time(NULL));
  for(int i=0;i<power;i++){
  /*
    R[i]=(double)((double)rand() / (double)RAND_MAX * 1231243224)/(double)1222341254;
    G[i]=(double)(drand48()* 1234223214)/(double)1212335224;
    B[i]= (double)(drand48()* 1232242134)/(double)1545322214;
    */
    cout<<drand48()<<endl;
    R[i]=(double)(drand48()+drand48())/(double)2.0;
    G[i]=(double)(drand48()+drand48())/(double)2.0;
    B[i]= (double)(drand48()+drand48())/(double)2.0;
   }
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(1000.0 / updateRate, timer, 0);
  glutMainLoop();
  return 0;
}
