#include <GL/glut.h>
#include <GL/glext.h>
#include "maze.h"
#include "texture.h"

#ifndef _GLOBALS_H
#define _GLOBALS_H


/////////////////////////////////////////////////////////////
// ******************* GAME PARAMETERS ******************* //
/////////////////////////////////////////////////////////////

// maze dimension
const double SQUARE_SIZE = 1.0;
const double WALL_HEIGHT = 1.0;
const int POLYS_PER_WALL = 3;

// light properties
const float SPOT_ATTENUATION = 0.2;
const float SPOT_EXPONENT = 10.0;
#define AMBIENT_LIGHT 0.2, 0.2, 0.2, 1.0

// maze color
#define WALLS_COLOR 0.5, 0.0, 0.0
#define FLOOR_COLOR 0.0, 0.0, 0.5
#define CEILING_COLOR 0.0, 0.5, 0.0

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////



//////////////////////////
//		CONSTANTS
//////////////////////////
const double PI=3.1415926;


//////////////////////////
//	TYPE DEFINITION
//////////////////////////
extern enum MazeTurnDirection {MZ_LEFT = -1, MZ_RIGHT = 1};


//////////////////////////
//	GLOBAL FUNCTIONS
//////////////////////////
extern void display();


//////////////////////////
//	GLOBAL VARIABLES
//////////////////////////
extern PFNGLACTIVETEXTUREARBPROC	glActiveTexture;
extern PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2f;


extern bool top_view;
extern bool colour_polys;
extern bool multitexture;

extern Maze maze;

extern Image square;
extern Image ram;
extern Image foot;
extern Image exitShading;
extern Image none;

extern float direction[3];
extern float position[3];
extern int mazePos[2];
extern int field_of_view;
extern float ratio;



#endif // _GLOBALS_H
