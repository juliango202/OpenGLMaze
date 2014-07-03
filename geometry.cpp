#include "geometry.h"

#include <VECTOR>

#include "globals.h"

using namespace std;

// local variables
static std::vector<double> vertices(0) ;	// coords of all vertices 
static int nbVerticesXWalls;			// total number of vertices for x walls
float exitCoord[2] = {0.0, 0.0};		// maze coordinates of the exit
float exitDirection[2] = {1.0, 0.0};	// direction of the explorer when he exits

// local functions
static void drawExit();
static void assignTextCoord();
static void drawHorizontalPlane(float zCoordinate);


/////
// Create polygons representing the maze
/////
void loadMaze()
{
	// load the maze data from the txt file
	maze.load();	

	// assign the initial maze coordinates
	mazePos[0] = maze.initialx;
	mazePos[1] = maze.initialy;

	// turn the initial maze coordinates into world coordinates
	position[0] = SQUARE_SIZE / 2.f + maze.initialx * SQUARE_SIZE;
	position[1] = WALL_HEIGHT / 2.f;
	position[2] = -SQUARE_SIZE / 2.f - maze.initialy * SQUARE_SIZE;

// Create the array of vertices : 4 per wall

	// x walls
	for(int i = 0; i <= maze.maxy; i++)
	{
		// number of x walls = number of squares + 1 for the last wall
		for( int j = 0; j <= maze.maxx + 1; j++)
		{
			if(maze.xwall[i][j])
			{
				// each wall is divided into several polygons to be able
				// to compute a better lighting
				for(int xPoly = 0; xPoly < POLYS_PER_WALL; xPoly++)
				for(int yPoly = 0; yPoly < POLYS_PER_WALL; yPoly++)
				{
					// 1st vertex
					vertices.push_back(j * SQUARE_SIZE); 
					vertices.push_back(yPoly * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE - (xPoly * SQUARE_SIZE / POLYS_PER_WALL));
					// 2nd vertex
					vertices.push_back(j * SQUARE_SIZE); 
					vertices.push_back((yPoly+1) * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE - (xPoly * SQUARE_SIZE / POLYS_PER_WALL));
					// 3rd vertex
					vertices.push_back(j * SQUARE_SIZE); 
					vertices.push_back((yPoly+1) * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE - ((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL));
					// 4th vertex
					vertices.push_back(j * SQUARE_SIZE); 
					vertices.push_back(yPoly * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE - ((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL));
				}
			}
			// if there is no wall at the boundary it is the exit
			else if(j == 0 || j == maze.maxx + 1)
			{
				// save maze coordinates of the exit
				exitCoord[0] = i; 
				exitCoord[1] = j;
				if(j == 0) 
				{ 
					exitDirection[0] = -1; 
					exitDirection[1] = 0;
				}
				else
				{ 
					exitDirection[0] = 1; 
					exitDirection[1] = 0;
				}
			}
		}
	}

	// save the total number of vertices for x walls to be
	// able to adjust the normal later
	nbVerticesXWalls = vertices.size();

	// y walls
	// number of y walls = number of squares + 1 for the last wall
	for(i = 0; i <= maze.maxy + 1; i++)
	{
		for(int j = 0; j <= maze.maxx; j++)
		{
			if(maze.ywall[i][j])
			{
				// each wall is divided into several polygons to be able
				// to compute a better lighting
				for(int xPoly = 0; xPoly < POLYS_PER_WALL; xPoly++)
				for(int yPoly = 0; yPoly < POLYS_PER_WALL; yPoly++)
				{
					// 1st vertex
					vertices.push_back(j * SQUARE_SIZE + (xPoly * SQUARE_SIZE / POLYS_PER_WALL)); 
					vertices.push_back(yPoly * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE);
					// 2nd vertex
					vertices.push_back(j * SQUARE_SIZE + (xPoly * SQUARE_SIZE / POLYS_PER_WALL)); 
					vertices.push_back((yPoly+1) * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE);
					// 3rd vertex
					vertices.push_back(j * SQUARE_SIZE + ((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL)); 
					vertices.push_back((yPoly+1) * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE);
					// 4th vertex
					vertices.push_back(j * SQUARE_SIZE + ((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL)); 
					vertices.push_back(yPoly * WALL_HEIGHT / POLYS_PER_WALL); 
					vertices.push_back(-i * SQUARE_SIZE);
				}
			}
			// if there is no wall at the boundary it is the exit
			else if(i == 0 || i == maze.maxy + 1)
			{
				// save maze coordinates of the exit
				exitCoord[0] = i;
				exitCoord[1] = j;
				if(i == 0) 
				{ 
					exitDirection[0] = 0; 
					exitDirection[1] = 1;
				}
				else
				{ 
					exitDirection[0] = 0; 
					exitDirection[1] = -1;
				}
			}
		}
	}
}

/////
// Draw polygons representing the maze, plus the ceiling and the floor
/////
void drawMaze()
{
	int rCol = 0, gCol = 0,  bCol = 100;
	
	// draw all the walls stored in the array
	glColor3f(WALLS_COLOR);
	glBegin(GL_QUADS);
		// define the normal
		glNormal3f(-1.0, 0.0, 0.0);

		// draw the vertices
		for(int i = 0; i < vertices.size(); i += 3)
		{
			// if we trace y walls, change the normal
			if(i == nbVerticesXWalls)
				glNormal3f(0.0, 0.0, -1.0);

			// colour each polygone in a different color
			if(colour_polys && i % 12 == 0) 
			{
				rCol = (rCol + 10) % 255;
				gCol = (gCol + 50) % 255;
				bCol = (bCol + 90) % 255;
				glColor3ub(rCol, gCol, bCol);
			}
			assignTextCoord(); glVertex3dv(&vertices[i]);
		}
	glEnd();

	// draw the floor
	glColor3f(FLOOR_COLOR);
	drawHorizontalPlane(0.0);

	// draw the ceiling
	if(!top_view)
	{
		glColor3f(CEILING_COLOR);
		drawHorizontalPlane(WALL_HEIGHT);
	}

	drawExit();
}

/////
// Assign a texture coordinate for next vertex
/////
static void assignTextCoord()
{
	static numVertice = 0;

	if (numVertice % 4 == 0) glTexCoord2f(0,1);
	if (numVertice % 4 == 1) glTexCoord2f(0,0);
	if (numVertice % 4 == 2) glTexCoord2f(1,0);
	if (numVertice % 4 == 3) glTexCoord2f(1,1);

	numVertice++;
}

/////
// Draw an horizontal plane at the given altitude
/////
static void drawHorizontalPlane(float zCoordinate)
{
	int rCol = 0, gCol = 0,  bCol = 100;

	glBegin(GL_QUADS);
		// define the normal
		glNormal3f(0.0, -1.0, 0.0);

		// divide the ceiling into several polygons to be able
		// to compute a better lighting
		for(int xPoly = 0; xPoly < POLYS_PER_WALL * (maze.maxx+1); xPoly++)
		for(int yPoly = 0; yPoly < POLYS_PER_WALL * (maze.maxy+1); yPoly++)
		{
			// colour each polygone in a different color
			if(colour_polys) 
			{
				rCol = (rCol + 10) % 255;
				gCol = (gCol + 50) % 255;
				bCol = (bCol + 90) % 255;
				glColor3ub(rCol, gCol, bCol);
			}
			// draw vertices
			assignTextCoord();glVertex3d(xPoly * SQUARE_SIZE / POLYS_PER_WALL, zCoordinate, -yPoly * SQUARE_SIZE / POLYS_PER_WALL);
			assignTextCoord();glVertex3d(xPoly * SQUARE_SIZE / POLYS_PER_WALL, zCoordinate, -(yPoly+1) * SQUARE_SIZE / POLYS_PER_WALL);
			assignTextCoord();glVertex3d((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL, zCoordinate, -(yPoly+1) * SQUARE_SIZE / POLYS_PER_WALL);
			assignTextCoord();glVertex3d((xPoly+1) * SQUARE_SIZE / POLYS_PER_WALL, zCoordinate, -yPoly * SQUARE_SIZE / POLYS_PER_WALL);
		}
	glEnd();
}


/////
// draw the exit square with a shading
/////
static void drawExit()
{
	// get a matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// move to the exit
	glTranslated(exitCoord[1] * SQUARE_SIZE, 0.0, -exitCoord[0] * SQUARE_SIZE);

	// rotate to the right direction
	// 0.02 values is to avoid error with limited float precision
	if(exitDirection[0] == -1)
	{
		// translate to the center of the poly
		glTranslated(0.0, WALL_HEIGHT / 2.0, -SQUARE_SIZE / 2.0);
		glRotated(180, 0.0, 1.0, 0.0);
		glTranslated(0.0, -WALL_HEIGHT / 2.0, SQUARE_SIZE / 2.0);
	}

	if(exitDirection[1] == -1)
	{
		// translate to the center of the poly
		glTranslated(SQUARE_SIZE, 0.0, 0.0);
		glTranslated(0.0, WALL_HEIGHT / 2.0, 0.0);
		glRotated(90, 0.0, 1.0, 0.0);
		glTranslated(0.0, -WALL_HEIGHT / 2.0, 0.0);
	}

	if(exitDirection[1] == 1)
	{
		glTranslated(0.0, WALL_HEIGHT / 2.0, 0.0);
		glRotated(-90, 0.0, 1.0, 0.0);
		glTranslated(0.0, -WALL_HEIGHT / 2.0, 0.0);
	}

	// if multitexturing available draw exit
	if(multitexture && !top_view)
	{
		// active 2nd texture unit
		glActiveTexture(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);

		glBegin(GL_QUADS);
			// floor
			glColor3f(FLOOR_COLOR);
			glNormal3f(0.0, -1.0, 0.0);
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
			glVertex3d(0.0, 0.0, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, 0.0, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, 0.0, 0.0);			
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
			glVertex3d(0.0, 0.0, 0.0);

			// ceiling
			glColor3f(CEILING_COLOR);
			glNormal3f(0.0, -1.0, 0.0);
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
			glVertex3d(0.0, WALL_HEIGHT, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, WALL_HEIGHT, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, WALL_HEIGHT, 0.0);			
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
			glVertex3d(0.0, WALL_HEIGHT, 0.0);

			// left wall
			glColor3f(WALLS_COLOR);
			glNormal3f(0.0, 0.0, 1.0);
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
			glVertex3d(0.0, 0.0, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, 0.0, -SQUARE_SIZE); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, WALL_HEIGHT,  -SQUARE_SIZE);			
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
			glVertex3d(0.0, WALL_HEIGHT, -SQUARE_SIZE);

			// right wall
			glColor3f(WALLS_COLOR);
			glNormal3f(0.0, 0.0, 1.0);
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 1.0);
			glVertex3d(0.0, 0.0, 0.0); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, 0.0, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 0.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, 0.0, 0.0); 
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 0.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 0.0);
			glVertex3d(SQUARE_SIZE / POLYS_PER_WALL, WALL_HEIGHT, 0.0);			
			glMultiTexCoord2f(GL_TEXTURE0_ARB, POLYS_PER_WALL, 1.0);glMultiTexCoord2f(GL_TEXTURE1_ARB, 1.0, 1.0);
			glVertex3d(0.0, WALL_HEIGHT, 0.0);
		glEnd();

		// disable this texture unit
		glDisable(GL_TEXTURE_2D);
		// select the defaut texture unit
		glActiveTexture(GL_TEXTURE0_ARB);
	}
	
	glPopMatrix();
}
