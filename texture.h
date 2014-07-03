#ifndef _IMAGE_H
#define _IMAGE_H

#include <gl/glut.h>

extern void initTextures();


class Image
{
private :
	int sizeX;
	int sizeY;
	unsigned char * data;

	GLuint numTex;

public :
	Image();
	~Image();
	void createTextureFromPPM(char *p_source);
	void setTexture();
};


#endif