#include "globals.h"


//////////////////////////
//	GLOBAL VARIABLES INITIALIZATION
//////////////////////////
PFNGLACTIVETEXTUREARBPROC       glActiveTexture = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2f = NULL;

bool top_view = false;
bool colour_polys = false;
bool multitexture = true;


Maze maze;
Image square;
Image ram;
Image foot;
Image exitShading;
Image none;

float direction[3] = {0, 0, -1};
float position[3] =	{0.f, 0.f, 0.f};
int mazePos[2] = {0.0, 0.0};
int field_of_view = 90;
float ratio;
