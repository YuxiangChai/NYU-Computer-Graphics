//#include "pch.h"
#include <sstream>
#include <string>
#include <iostream>
#include <string.h>

#include "Simulation.h"

#include <OpenGl/glu.h>
#include <GLUT/GLUT.h>

using namespace std;

SimBasic* simb = new SimBasic(vector3f(0.0f, -9.81f, 0.0f));
float timeElapsed = 0;

double eyex = 0.0, eyey = 0.0, eyez = 45.0;

uint32_t getTick();
void Update(unsigned int ms);
void Draw(void);
void drawCube(vector3f color);
void drawWalls();
void ReshapeGL();

void ReshapeGL(int width, int height)
{
	glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f, (GLfloat)(width) / (GLfloat)(height), 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(){
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	ReshapeGL(800,600);
}

int tickCount;
int lastTickCount;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	cout << "initialized" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	cout << "display mode initialized" << endl;
	glutInitWindowSize(800, 600);
	cout << "window size initialized" << endl;
	glutInitWindowPosition(100, 100);
	cout << "position initialized" << endl;
	glutCreateWindow("Gizem Kayar Simulation Window");
	cout << "window created" << endl;

	

	bool g_isProgramLooping = true;
	tickCount = 0;
	lastTickCount = getTick();

	//select clearing (background) color
    //glClearColor(0.0, 0.0, 0.0, 0.0);

    //initialize viewing values
	init();
   

	glutDisplayFunc(Draw);
	glutIdleFunc(Draw);
	cout << "display function chosen" << endl;
	glutMainLoop();

	return 1;
}


uint32_t getTick() {
    struct timespec ts;
    unsigned theTick = 0U;
    clock_gettime( CLOCK_REALTIME, &ts );
	//cout << ts.tv_nsec << endl;
    theTick  = ts.tv_nsec / 1000000;
    theTick += ts.tv_sec * 1000;
	//cout << theTick << endl;
    return theTick;
}

void Update(unsigned int ms)
{
	/*
	if (g_keys->keyDown[VK_ESCAPE] == true)
		TerminateApplication(g_window);
	*/

	float sec = ms / 1000.0f;
	timeElapsed += sec;
	simb->simulate();
}

void drawPolygon(){
	glColor3f(1.0,1.0,1.0);
    glBegin(GL_POLYGON);
        glVertex3f(0.25, 0.25, 0.0);
        glVertex3f(0.75, 0.25, 0.0);
        glVertex3f(0.75, 0.75, 0.0);
        glVertex3f(0.25, 0.75, 0.0);
    glEnd();
}

void drawCube(vector3f color)
{
	glBegin(GL_QUADS);

	float x = color.getX();
	float y = color.getY();
	float z = color.getZ();

	// Front Face
	glColor3f(x, y, z);
	glNormal3f(0, 0, 1);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	// Back Face
	glColor3f(x, y, z);
	glNormal3f(0, 0, -1);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);

	// Top Face
	if (x > 0.5f)
		glColor3f(x - 0.5f, y, z);
	else if (x != 0)
		glColor3f(x - 0.3f, y, z);
	else
		glColor3f(x + 0.3f, y, z);
	glNormal3f(0, 1, 0);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);

	// Bottom Face
	if (x > 0.5f)
		glColor3f(x - 0.5f, y, z);
	else if (x != 0)
		glColor3f(x - 0.3f, y, z);
	else
		glColor3f(x + 0.3f, y, z);
	glNormal3f(0, -1, 0);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);

	// Right face
	if (y > 0.5f)
		glColor3f(x, y - 0.5f, z);
	else if (x != 0)
		glColor3f(x, y - 0.3f, z);
	else
		glColor3f(x, y + 0.3f, z);
	glNormal3f(1, 0, 0);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);

	// Left Face
	if (y > 0.5f)
		glColor3f(x, y - 0.5f, z);
	else if (x != 0)
		glColor3f(x, y - 0.3f, z);
	else
		glColor3f(x, y + 0.3f, z);
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);

	glEnd();
}

void drawWalls()
{
	glBegin(GL_QUADS);

	//left
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-10, 10, 10);
	glVertex3f(-10, 10, -10);
	glVertex3f(-10, -10, -10);
	glVertex3f(-10, -10, 10);

	//right
	glColor3f(0.5f, 0.5f, 0.5f);
	glVertex3f(10, 10, -10);
	glVertex3f(10, 10, 10);
	glVertex3f(10, -10, 10);
	glVertex3f(10, -10, -10);

	//far
	glColor3f(0.9f, 0.9f, 0.9f);
	glVertex3f(-10, 10, -10);
	glVertex3f(10, 10, -10);
	glVertex3f(10, -10, -10);
	glVertex3f(-10, -10, -10);

	//bottom
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(-10, -10, -10);
	glVertex3f(10, -10, -10);
	glVertex3f(10, -10, 10);
	glVertex3f(-10, -10, 10);

	//top
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex3f(-10, 10, -10);
	glVertex3f(10, 10, -10);
	glVertex3f(10, 10, 10);
	glVertex3f(-10, 10, 10);


	glEnd();

}

GLvoid glPrint(float x, float y, float z, const char *fmt, ...)
{
	float		length = 0;
	char		text[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsnprintf(text, 256, fmt, ap);
	va_end(ap);
	
  	glRasterPos2f(x, y);
  	int len, i;
  	len = (int)strlen(text);
  	for (i = 0; i < len; i++) {
    	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
  	}
	
}

void Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	vector3f halfWorld = simb->halfWorld;

	drawWalls();
	
//	drawCube(vector3f(0.1f, 0.2f, 0.9f));
	glPrint(-5.0f, -14, 0, "Time elapsed (seconds): %.2f", timeElapsed);

	for (int a = 0; a < simb->noPt; a++)
	{
		vector3f pos = simb->particleList[a].getPosition();
		vector3f color = simb->particleList[a].getColor();

		glTranslatef(pos.getX(), pos.getY(), pos.getZ());
		drawCube(color);
		glTranslatef(-pos.getX(), -pos.getY(), -pos.getZ());
	}

	
	glColor3ub(100, 100, 100);
	glPrint(-5.0f, -12, 0, "Time elapsed (seconds): %.2f", timeElapsed);

	glColor3ub(100, 0, 0);
	glPrint(-5.0f, 13, 0, "Simple Gravity System");
	glPrint(-5.0f, 12, 0, "System Kinetic Energy: %.2f", simb->kinEn);
	glPrint(-5.0f, 11, 0, "System Potential Energy: %.2f", simb->potEn);
	glPrint(-5.0f, 10, 0, "System Total Energy: %.2f", (simb->kinEn + simb->potEn));

	tickCount = getTick();
	Update(tickCount - lastTickCount);
	lastTickCount = tickCount;
	//cout << timeElapsed << endl;

	//ReshapeGL(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 43, 0, 0, 0, 0, 1, 0);
	//cout << glutGet(GLUT_WINDOW_WIDTH) << endl;
	//ReshapeGL(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	

	glutSwapBuffers();
}








