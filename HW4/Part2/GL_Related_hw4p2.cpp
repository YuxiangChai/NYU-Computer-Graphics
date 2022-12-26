#include <sstream>
#include <string.h>
#include <iostream>

#include "Simulation.h"
// #include "Simulation.cpp"
#include "math3d.h"

#include <GLUT/glut.h>

using namespace std;

SimSpring* simsp = new SimSpring();
float timeElapsed = 0;
bool drawSprings = true;
bool drawTriangles = false;

uint32_t getTick();
void Draw(void);

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

bool is_q_pressed = false;

//https://stackoverflow.com/questions/30728145/check-q-button-is-held-down-using-glut
void KeyboardHandler(unsigned char key, int x, int y) 
{
    switch (key)
    {
	case 27:
		exit(1);
    case 's':
    case 'S':
        drawSprings = true;
		drawTriangles = false;
        break;
	case 't':
	case 'T':
		drawSprings = false;
		drawTriangles = true;
        break;
    }
}

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
    glutKeyboardFunc(KeyboardHandler);

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
	float sec = ms / 1000.0f;
	timeElapsed += sec;
	simsp->simulate();
}

void drawCube(vector3f color)
{
	glBegin(GL_QUADS);

	float x = color.getX();
	float y = color.getY();
	float z = color.getZ();
	
	glColor3f(x, y, z);
	// Front Face
	glNormal3f(0, 0, 1);
	glVertex3f(-0.05f, -0.05f, 0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);

	// Back Face
	glNormal3f(0, 0, -1);
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(-0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, -0.05f);

	// Top Face
	glNormal3f(0, 1, 0);
	glVertex3f(-0.05f, 0.05f, -0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);

	// Bottom Face
	glNormal3f(0, -1, 0);
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);
	glVertex3f(-0.05f, -0.05f, 0.05f);

	// Right face
	glNormal3f(1, 0, 0);
	glVertex3f(0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);

	// Left Face
	glNormal3f(-1, 0, 0);
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(-0.05f, -0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, -0.05f);

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

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();										

	gluLookAt(0, 0, 45, 0, 0, 0 , 0, 1, 0);
	 

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	vector3f halfWorld = simsp->halfWorld;
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawWalls();


	for (int a = 0; a < simsp->mesh.particles.size(); a++)
	{
		vector3f pos = simsp->mesh.particles[a].getPosition();
		vector3f color = simsp->mesh.particles[a].getColor();

		glTranslatef(pos.getX(), pos.getY(), pos.getZ());
		drawCube(color);
		glTranslatef(-pos.getX(), -pos.getY(), -pos.getZ());
	}

	if (drawSprings)
	{
		for (int s = 0; s < simsp->mesh.springs.size(); s++)
		{
			Spring  sp = simsp->mesh.springs[s];
			vector3f position1 = simsp->mesh.particles[sp.p1].getPosition();
			vector3f position2 = simsp->mesh.particles[sp.p2].getPosition();

			glColor3f(0, 0, 1);
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex3f(position1.getX(), position1.getY(), position1.getZ());
			glVertex3f(position2.getX(), position2.getY(), position2.getZ());
			glEnd();
		}
	}

	if (drawTriangles)
	{
		for (int t = 0; t < simsp->mesh.surfaceTriangles.size(); t++)
		{
			Triangle tri = simsp->mesh.surfaceTriangles[t];
			vector3f position1 = simsp->mesh.particles[tri.p1].getPosition();
			vector3f position2 = simsp->mesh.particles[tri.p2].getPosition();
			vector3f position3 = simsp->mesh.particles[tri.p3].getPosition();
			glColor3f(1, 0, 0);
			glLineWidth(1);
			glBegin(GL_TRIANGLES);
			glNormal3f(tri.normal.getX(), tri.normal.getY(), tri.normal.getZ());
			glVertex3f(position1.getX(), position1.getY(), position1.getZ());
			glVertex3f(position2.getX(), position2.getY(), position2.getZ());
			glVertex3f(position3.getX(), position3.getY(), position3.getZ());
			glEnd();
		}
	}

	tickCount = getTick();
	Update(tickCount - lastTickCount);
	lastTickCount = tickCount;

	glColor3ub(200, 255, 200);									
	glPrint(-5.0f, -14, 0, "Time elapsed (seconds): %.2f", timeElapsed);
	glPrint(0.0f, -12, 0, "S_key: draw springs, T_key: draw triangles");
	
	glPrint(-5.0f, 13, 0, "Simple Gravity System");
	glPrint(-5.0f, 12, 0, "System Kinetic Energy: %.2f", simsp->kinEn);
	glPrint(-5.0f, 11, 0, "System Potential Energy: %.2f", simsp->potEn);
	glPrint(-5.0f, 10, 0, "System Spring Energy: %.2f", (simsp->sprEn));
	glPrint(-5.0f, 9, 0, "System Total Energy: %.2f", (simsp->kinEn+simsp->potEn+simsp->sprEn));

	glutSwapBuffers();													
}








