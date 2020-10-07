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
#include <time.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

//struct for passing around coordinates
//just a handy thing to have
//now if only we had the JavaScript spread operator, we could make drawing a lot easier
typedef struct
{
	int x;
	int y;
} coords;

//parameters to be passed to glutTimerFunc
//"but wait, Lane, you can only provide an int as the parameter to glutTimerFunc!"
//you might be in physical pain when you see what I have planned
typedef struct
{
	int frames;
	coords old;
	coords new;
} params;

//coordinates for the cube
coords cube_coords;

//coordinates for the sphere
//and behold: a C feature that's not supported in standard C++!
coords sphere_coords = {
	.x = 250,
	.y = 250
};

//how many frames across which we're animating
int frame_count = 5;

//parameters used for the animation of the sphere
params sphere_params;

//milliseconds it takes to complete the animation
int animation_time = 20;

//Handler for drawing the scene
void drawScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.7f, 0.0f, 1.0f);
	glLoadIdentity();
	glTranslatef((float)cube_coords.x, (float)cube_coords.y, 0.0);
	glutWireCube(50);
	glPopMatrix();
	glLoadIdentity();
	glTranslatef((float)sphere_coords.x, (float)sphere_coords.y, 0.0);
	glutWireSphere(15, 16, 16);
	glPopMatrix();

	
	//Swap buffers because of double buffering
	glutSwapBuffers();
}

void alter_sphere(int _)
{

	sphere_coords.x += (sphere_params.new.x - sphere_params.old.x) / frame_count;
	sphere_coords.y += (sphere_params.new.y - sphere_params.old.y) / frame_count;

	glutPostRedisplay();

	sphere_params.frames--;
	
	if (sphere_params.frames != 0)
		glutTimerFunc(animation_time, alter_sphere, _);
}

void animate(coords old, coords new)
{
	sphere_params = (params){ .frames = frame_count, .old = old, .new = new };
	
	//owch, that cast hurts me to my core
	glutTimerFunc(animation_time, alter_sphere, (int)NULL);
}

bool sphitsp(coords cube, coords sphere)
{
	return sphere.x < cube.x + 25 && sphere.x > cube.x - 25 &&
		sphere.y < cube.y + 25 && sphere.y > cube.y - 25;
}

int win(void)
{
	return 0;
}

//Resizing handler
void resize(int width, int height)
{
	//Set the viewing box
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 500.0, 0.0, 500.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//Set up the vertex array for drawing
void setup(void)
{
	//Disable warning "Random number generator seeded with a disallowed source of seed value will generate a predictable sequence of values."
#pragma warning(push)
#pragma warning(disable:4083)
	//Seed RNG
	srand((unsigned int)time(NULL));
#pragma warning(pop)

	//Generate cube coordinates
	cube_coords.x = rand() % 500;
	cube_coords.y = rand() % 500;
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//Handler for ASCII input
void keyInput(unsigned char key, int x, int y)
{
	if (sphere_params.frames != 0)
		return; //curretly in the middle of an animation, don't handle input right now
	
	switch (key)
	{
		//if w, animate upwards and update the coordinates
	case 'w':
		animate(sphere_coords, (coords){ sphere_coords.x, sphere_coords.y + 10 });
		glutPostRedisplay();
		break;
		//if s, animate downwards and update the coordinates
	case 's':
		animate(sphere_coords, (coords) { sphere_coords.x, sphere_coords.y - 10 });
		glutPostRedisplay();
		break;
		//if a, animate left and update the coordinates
	case 'a':
		animate(sphere_coords, (coords) { sphere_coords.x - 10, sphere_coords.y });
		glutPostRedisplay();
		break;
		//if d, animate right and update the coordinates
	case 'd':
		animate(sphere_coords, (coords) { sphere_coords.x + 10, sphere_coords.y });
		glutPostRedisplay();
		break;
		//if +, increase the animation speed
	case '+':
		animation_time -= 5;
		animation_time < 5 && (animation_time = 5); //limit on 5 milliseconds
		break;
		//if -, decrease the animation speed
	case '-':
		animation_time += 5;
		break;
		//If escape, quit
	case 27:
		exit(0);
	default: break;
	}

	//didn't wanna use if, decided to use && instead
	sphitsp(cube_coords, sphere_coords) && win();
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hey mom i made a game without an engine :o");

	//Attach our handler functions
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);

	//Set this because glew doesn't like it when you don't, and initialize glew
	glewExperimental = GL_TRUE;
	glewInit();

	//Go to the setup function
	setup();

	//Start the main glut loop! We're off to the races!!
	glutMainLoop();
}