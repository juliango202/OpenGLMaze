#include "lighting.h"

/////
// set up the lighting model
/////
void initLighing()
{
	// use 2-sided lighting model to make normals correct
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// use gouraud shading
	glShadeModel(GL_SMOOTH);

	// enable the lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	// define properties of the light
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, SPOT_ATTENUATION);
	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, 45.0);
	glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, SPOT_EXPONENT);

	// low ambient light
	GLfloat lmodel_ambient[] = {AMBIENT_LIGHT} ;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient) ;
}

/////
// draw a spot light at the viewer position
/////
void drawLight()
{
	float lpos[] = {position[0], position[1], position[2], 1.0};
	float ldir[] = {direction[0], direction[1], direction[2]};

	// assign viewer position and direction values to the spot light
	glLightfv(GL_LIGHT0,GL_POSITION, lpos);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION, ldir);
}