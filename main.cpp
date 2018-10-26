#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <GLUT/glut.h>
#include "readppm.h"
#include "vectorfunctions.h"
#include "readOBJ.h"

using namespace std;


int environmentWidth = 1500;
int environmentHeight = 1000;

vector< vector< double > > vertices;
vector< vector< double > > faces;
vector< vector< double > > vertexNormals;

unsigned char * environment;

void init();
void display();

int main(int argc, char ** argv)
{
	// Initialize an OpenGL window
	glutInit(&argc, (char**) argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	init();

	glutDisplayFunc(display);
	// glutReshapeFunc(reshape);
	// glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}


void init()
{
	// Read in the teapot
	parseFile("teapot.obj");
	createNormals();

	// Read in the texture file
	// texture = readPPM("texture.ppm", &textureWidth, &textureHeight);
	// texture = flipImage(texture, textureWidth, textureHeight);

	// Read in the environment file
	environment = readPPM("environment.ppm", &environmentWidth, &environmentHeight);
	environment = flipImage(environment, environmentWidth, environmentHeight);

	// Initialize Perspective Projection in OpenGL
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(70.f, 1.0, 0.001f, 30.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Create lighting and shadows
	GLfloat white[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat lpos[] = {10.0, 10.0, 10.0, 0.0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);

	// Enable the Z-buffer to remove hidden surfaces
	glEnable(GL_DEPTH_TEST);
}


void display()
{

	// Create the environment map
	GLuint environmentImage;
	glGenTextures(1, &environmentImage);
	glBindTexture(GL_TEXTURE_2D, environmentImage);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, environmentWidth, environmentHeight, GL_RGB, GL_UNSIGNED_BYTE, environment);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glEnable(GL_TEXTURE_2D);


	// Create the lookat vector
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5.f, 6.f, 4.f, 0.f, 1.f, 0.f, 0.f, 1.f, 0.f);



	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glColor3d(1.0, 1.0, 1.0);

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < (int) faces.size(); ++i)
	{
		// Select the vertices that make up the face
		vector< double > vertex1 = vertices[(int) faces[i][0]];
		vector< double > normal1 = vertexNormals[(int) faces[i][0]];
		vector< double > vertex2 = vertices[(int) faces[i][1]];
		vector< double > normal2 = vertexNormals[(int) faces[i][1]];
		vector< double > vertex3 = vertices[(int) faces[i][2]];
		vector< double > normal3 = vertexNormals[(int) faces[i][2]];

		// Draw the vertices
		glNormal3d(normal1[0], normal1[1], normal1[2]);
		glVertex3d(vertex1[0], vertex1[1], vertex1[2]);
		glNormal3d(normal2[0], normal2[1], normal2[2]);
		glVertex3d(vertex2[0], vertex2[1], vertex2[2]);
		glNormal3d(normal3[0], normal3[1], normal3[2]);
		glVertex3d(vertex3[0], vertex3[1], vertex3[2]);
	}
	glEnd();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	// Enable double buffering
	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}
