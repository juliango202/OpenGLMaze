#include <GL/glut.h>
#include <math.h>
#include <vector>

#include "globals.h"

#include "geometry.h"
#include "lighting.h"
#include "texture.h"
#include "transformations.h"


void init(void)
{
	//use this color when clearing frame buffer
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// load maze data into memory
	loadMaze();

	// set the position to the start of the maze
	initPosition();

	// initialize texture and lighting
	initLighing();
	initTextures();

	glEnable(GL_DEPTH_TEST);

	// print description of commands on stdout
	printf("- ThE MaZe -\n\n");
	printf(" F1            - Toggle top view\n");
	printf(" F2            - Show all polygons\n");
	printf(" F3            - Toggle lighting\n");
	printf(" F4            - Cycle through textures\n");
	printf(" v             - Change field of view\n");
	printf(" q             - Quit\n");
	printf("\n Have fun!\n");
}

void changeFieldOfView(int fov)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ratio, 0.1, 250);
	glMatrixMode(GL_MODELVIEW);
}

void Reshape(int width, int height)
{
	glViewport(0,0,width,height);
	ratio = float(width)/float(height);
	changeFieldOfView(field_of_view);
}

void keyboard(unsigned char key, int x, int y)  
{  
	switch (key) 
	{
		case 'q': exit(0); break;
		case 'v':	int fov = field_of_view;
					if(fov == 90) field_of_view = 30;
					if(fov == 30) field_of_view = 120;
					if(fov == 120) field_of_view = 90; 
					changeFieldOfView(field_of_view);break;
	}

	// redraw the scene
	glutPostRedisplay() ;
}

void keyboardSpecial(int key, int x, int y)  
{
	static int numTexture = 0;

	switch (key) 
	{  
		case GLUT_KEY_UP :		moveForward(); break;
		case GLUT_KEY_DOWN :	moveBack(); break;
		case GLUT_KEY_LEFT :	turn(MZ_LEFT); break;
		case GLUT_KEY_RIGHT :	turn(MZ_RIGHT); break;

		// switch top view
		case GLUT_KEY_F1 :		topView(); break;
		
		// switch polygon colouring
		case GLUT_KEY_F2 :		colour_polys = !colour_polys; break;

		// switch lighting	
		case GLUT_KEY_F3 :		if(glIsEnabled(GL_LIGHTING)) glDisable(GL_LIGHTING);
								else glEnable(GL_LIGHTING); break;

		// change texture
		case GLUT_KEY_F4 :		numTexture++;
								if(numTexture % 4 == 0) square.setTexture();
								if(numTexture % 4 == 1) ram.setTexture();
								if(numTexture % 4 == 2) foot.setTexture();
								if(numTexture % 4 == 3) none.setTexture();break;
	}
	
	// redraw the scene
	glutPostRedisplay() ;
}

extern void display(void)
{
	// clear both color buffer and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawPosition();

	drawLight();
	
	drawMaze();
	
	// double buffering : swap buffers to send results to screen
	glutSwapBuffers(); 
}

int main(int argc, char **argv)
{
	// window & callbacks initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow(argv[0]);
	init();
	glutReshapeFunc(Reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard) ;
	glutSpecialFunc(keyboardSpecial) ;
	glutMainLoop();
	return 0;
}