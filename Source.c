/* Phillip Lane
 * CS 545 01
 * 10/7/2020
 * Dr. Hauenstein
 *
 * Software used:
 * Windows 10 Pro
 * Visual Studio 2017 (v141)
 * Windows SDK Toolkit 10.0.18362.0
 * 
 * Hardware used:
 * 2x Opteron 6380
 * 64GB DDR3
 * Nvidia Quadro K4200
 *
 * [Program description]
 *
 * Controls:
 * [controls]
 * 
 *  esc  : Quit
 */

//Just some libraries we're gonna be using
#include <stdio.h>
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>



//Handler for drawing the scene
void drawScene(void)
{
	
}

//Resizing handler
void resize(int width, int height)
{
	//Set the viewing box
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Set up the vertex array for drawing
void setup(void)
{
	
}

//Handler for ASCII input
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
		//If escape, quit
	case 27:
		exit(0);
	default: break;
	}
}

//Handler for non-ASCII input
void specialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	
	default:
		break;
	}
}

int main(int argc, char **argv)
{
	//Initialize the command line arguments
	glutInit(&argc, argv);

	//Set context version to 4.0 so that it'll play nicely with Dr. Hauenstein's PC (he can't run OpenGL 4.3)
	glutInitContextVersion(4, 0);
	//Compatibility profile, so that we can do glBegin and glEnd, among other things
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	//Set the window options, like display mode, size, position, and text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sine and Cosine Waves");

	//Attach our handler functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	//Set this because glew doesn't like it when you don't, and initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Go to the setup function
	setup();

	//Start the main glut loop! We're off to the races!!
	glutMainLoop();
}