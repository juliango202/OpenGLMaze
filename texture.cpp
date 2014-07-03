#include <IOSTREAM>
#include <STDIO.H>
#include <STRING>

#include "texture.h"

#include "globals.h"
#include <gl/glext.h>


#define NB_COMPOSANTES 3

void initTextures()
{
	// load all textures from disk
	ram.createTextureFromPPM("images/ram.ppm");
	foot.createTextureFromPPM("images/foot.ppm");
	square.createTextureFromPPM("images/square.ppm");
	none.createTextureFromPPM("images/none.ppm");

	// enable texturing
	glEnable(GL_TEXTURE_2D);

// multitexturing used for draw the exit with as a 2 textures blend

	// look if multitexturing is available
	glActiveTexture = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
	if (glActiveTexture == NULL) {
		printf("No glActiveTexture function available\n");
		multitexture = false;
		return;
	}
	
	glMultiTexCoord2f = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
	if (glMultiTexCoord2f == NULL)
	{
		printf("glMultiTexCoord2fv not supported");
		multitexture = false;
		return;
	}

	if(!multitexture)
		MessageBox(NULL, "Your card doesn't support multitexturing!", "Multitexturing disabled", MB_OK | MB_ICONINFORMATION);

// exit square texture
	exitShading.createTextureFromPPM("images/shading.ppm");
	glActiveTexture(GL_TEXTURE1_ARB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// this texture unit use blend mode with white color
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	float white[4] = {1,1,1,1};
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, white);
	// assign a texture for this texture unit
	exitShading.setTexture();

// defaut texture
	glActiveTexture(GL_TEXTURE0_ARB);
	// bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	square.setTexture();
}

Image::Image()
{
	data = NULL ;
}

Image::~Image()
{
	if(data)
		delete [] data ;
	numTex = -1 ;
}

void Image::createTextureFromPPM(char *p_source)
{
	FILE *fileh;
	char buf[BUFSIZ];
	int nbCol;
	
// load the ppm file
	fileh=fopen(p_source,"r");
	if (!fileh) 
	{ 
		printf("! fichier %s introuvable !",p_source); return; 
	}

	// read image identifier
	fscanf(fileh, " %s ", buf);

	// read image width and height
	fscanf(fileh, "%d %d", &sizeX, &sizeY);

	// read maximum color-component value
	fscanf(fileh, "%d", &nbCol);
	
	// allocate space
	if(data)
		delete [] data ;
	data = new unsigned char[sizeX * sizeY * NB_COMPOSANTES] ;

	// read pixel colors
	int number;
	for(int i = 0; i < sizeX * sizeY * NB_COMPOSANTES; i++)
	{
		fscanf(fileh, "%d", &number);
		data[i] = number;
	}
	fclose(fileh);

// create the texture
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glGenTextures(1,&numTex);
	glBindTexture(GL_TEXTURE_2D,numTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, NB_COMPOSANTES, sizeX,	sizeY, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void Image::setTexture()
{
	if(numTex != -1)
		glBindTexture(GL_TEXTURE_2D,numTex);
}