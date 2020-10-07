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
 * This program is a miniature game written in OpenGL. You move a sphere with WASD into a box, whose position is random on the board.
 * Moving the sphere into the box triggers text to appear saying "You win!" You can also speed up and slow down the animation of the sphere.
 *
 * Controls:
 *   W  - move up
 *   A  - move left
 *   S  - move down
 *   D  - move right
 *   +  - speed up the animation
 *   -  - slow down the animation
 *  esc - Quit
 */

//Just some libraries we're gonna be using
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define FRAME_COUNT 5

//struct for passing around coordinates
//just a handy thing to have
//now if only we had the JavaScript spread operator, we could make drawing a lot easier
typedef struct
{
	int x;
	int y;
} coords;

//parameters to be used with glutTimerFunc
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

//parameters used for the animation of the sphere
params sphere_params;

//milliseconds it takes to complete the animation
int animation_time = 20;

//boolean for winning, determines if we need to accept further input or not
bool won = false;

//Handler for drawing the scene
void drawScene(void)
{
	//clear color and depth buffer, set color to that same pretty purple I used in the previous program
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.7f, 0.0f, 1.0f);

	//translate and draw the cube
	glLoadIdentity();
	glTranslatef((float)cube_coords.x, (float)cube_coords.y, 0.0);
	glutWireCube(50);
	glPopMatrix();

	//translate and draw the sphere
	glLoadIdentity();
	glTranslatef((float)sphere_coords.x, (float)sphere_coords.y, 0.0);
	glutWireSphere(15, 16, 16);
	glPopMatrix();

	//if we've won
	if (won)
	{
		//set color to black, set raster position, and write "You won!" to the screen
		//I saw that glutBitmapString existed so I decided to use it instead of writing my own and using glutBitmapCharacter
		glColor3f(0.0f, 0.0f, 0.0f);
		glLoadIdentity();
		glRasterPos3f(200.0f, 400.0f, 0.0f);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, "You won!");
		glPopMatrix();
	}
	
	//Swap buffers because of double buffering
	glutSwapBuffers();
}

bool sphitsp(coords cube, coords sphere)
{
	//check for collision
	return sphere.x < cube.x + 25 && sphere.x > cube.x - 25 &&
		sphere.y < cube.y + 25 && sphere.y > cube.y - 25;
}

void win(void)
{
	//set the win boolean and redraw
	won = true;
	glutPostRedisplay();
}

void alter_sphere(int _)
{
	//translate the sphere's coordinates based on the frame count
	sphere_coords.x += (sphere_params.new.x - sphere_params.old.x) / FRAME_COUNT;
	sphere_coords.y += (sphere_params.new.y - sphere_params.old.y) / FRAME_COUNT;

	//draw the screen
	glutPostRedisplay();

	//decrease the frames remaining in the animation
	sphere_params.frames--;

	//if we're not out of frames in the animation, set a timer for the next frame draw
	//if we ARE out of frames, check for collision and trigger the win() function if there is collision
	if (sphere_params.frames != 0)
		glutTimerFunc(animation_time, alter_sphere, _);
	else if (sphitsp(cube_coords, sphere_coords))
		win();
}

void animate(coords old, coords new)
{
	//set the remaining frames to FRAME_COUNT and set the old and new positions for the sphere
	sphere_params = (params){ .frames = FRAME_COUNT, .old = old, .new = new };
	
	//call the timer func
	glutTimerFunc(animation_time, alter_sphere, (int)NULL);
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
	//Seed RNG
	srand((unsigned int)time(NULL));

	//Repeat if we accidentally generate a board in winning position
	do
	{
		//Generate cube coordinates
		cube_coords.x = rand() % 400 + 50;
		cube_coords.y = rand() % 400 + 50;
	} while (sphitsp(cube_coords, sphere_coords));
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//Handler for ASCII input
void keyInput(unsigned char key, int x, int y)
{
	if (sphere_params.frames != 0 || won)
		return; //either curretly in the middle of an animation, or user has already won. either way, don't handle input right now
	
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