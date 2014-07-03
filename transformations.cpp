#include "transformations.h"

#include <IOSTREAM>
#include <WINDOWS.H>
#include <MATH.H>

// Constants used for tranformations
const int	TURN_NUMBER_OF_STEP = 20;
const int	TURN_TIME = 500;
const float TURN_ANGLE = 90.f;
const float MOVEMENT_SIZE = SQUARE_SIZE;


/////
// When the explorer leaves the maze display a msg and exit
/////
void success()
{
	MessageBox(NULL, "Congratulation, YOU WON!!", "Exit from maze", MB_OK | MB_ICONINFORMATION);
	exit(0);
}

/////
// Move one step forward in the maze
/////
void moveForward()
{
// collision detection
	if(direction[0] == 1.0 && maze.xwall[mazePos[1]][mazePos[0] + 1] == 1) return;
	if(direction[0] == -1.0 && maze.xwall[mazePos[1]][mazePos[0]] == 1) return;
	if(direction[2] == -1.0 && maze.ywall[mazePos[1] + 1][mazePos[0]] == 1) return;
	if(direction[2] == 1.0 && maze.ywall[mazePos[1]][mazePos[0]] == 1) return;

// if there is no wall compute the transformation
	glTranslatef(-direction[0] * MOVEMENT_SIZE, -direction[1] * MOVEMENT_SIZE, -direction[2] * MOVEMENT_SIZE);

	// update position in world coordinates
	position[0] += direction[0] * MOVEMENT_SIZE;
	position[1] += direction[1] * MOVEMENT_SIZE;
	position[2] += direction[2] * MOVEMENT_SIZE;

	// update position in maze coordinates
	mazePos[0] += (int)direction[0];
	mazePos[1] -= (int)direction[2];

	// test if the position is outside the maze
	if(mazePos[0] < 0 || mazePos[0] > maze.maxx || mazePos[1] < 0 || mazePos[1] > maze.maxy) success();
}


/////
// Move one step backward in the maze
/////
void moveBack()
{	
// collision detection
	if(direction[0] == 1.0 && maze.xwall[mazePos[1]][mazePos[0]] == 1) return;
	if(direction[0] == -1.0 && maze.xwall[mazePos[1]][mazePos[0] + 1] == 1) return;
	if(direction[2] == -1.0 && maze.ywall[mazePos[1]][mazePos[0]] == 1) return;
	if(direction[2] == 1.0 && maze.ywall[mazePos[1] + 1][mazePos[0]] == 1) return;

// if there is no wall compute the transformation
	glTranslatef(direction[0] * MOVEMENT_SIZE, direction[1] * MOVEMENT_SIZE, direction[2] * MOVEMENT_SIZE);

	// update position in world coordinates
	position[0] -= direction[0] * MOVEMENT_SIZE;
	position[1] -= direction[1] * MOVEMENT_SIZE;
	position[2] -= direction[2] * MOVEMENT_SIZE;
	
	// update position in maze coordinates
	mazePos[0] -= (int)direction[0];
	mazePos[1] += (int)direction[2];

	// test if the position is outside the maze
	if(mazePos[0] < 0 || mazePos[0] > maze.maxx || mazePos[1] < 0 || mazePos[1] > maze.maxy) success();
}


/////
// Perform a left or right rotation according to the argument
/////
void turn(MazeTurnDirection turnDir)
{
	int elapsedTime, waiting;
	
	// decompose the rotation into several steps
	for(int i = 0; i < TURN_NUMBER_OF_STEP; i++)
	{
		// wait for 100 millisecond
		waiting = 0;
		elapsedTime= glutGet(GLUT_ELAPSED_TIME) ;
		while(waiting < TURN_TIME / TURN_NUMBER_OF_STEP)
		{
			waiting = glutGet(GLUT_ELAPSED_TIME) - elapsedTime;
		}
		
		// perform the rotate step
		glTranslatef(position[0], position[1], position[2]);
		glRotatef(turnDir * TURN_ANGLE / (float)TURN_NUMBER_OF_STEP, 0.f, 1.0f, 0.f);
		glTranslatef(-position[0], -position[1], -position[2]);

		// update the direction
		float saveDir = direction[0];
		direction[0] = cos(-(turnDir * TURN_ANGLE / (float)TURN_NUMBER_OF_STEP) / 180.f * PI) * direction[0] 
				+ sin(-(turnDir * TURN_ANGLE / (float)TURN_NUMBER_OF_STEP) / 180.f * PI) * direction[2];
		direction[2] = -sin(-(turnDir * TURN_ANGLE / (float)TURN_NUMBER_OF_STEP) / 180.f * PI) * saveDir 
				+ cos(-(turnDir * TURN_ANGLE / (float)TURN_NUMBER_OF_STEP) / 180.f * PI) * direction[2];

		// draw this step
		display();
	}

	// round the direction to undo the error caused by float precision
	direction[0] = (int)direction[0];
	direction[2] = (int)direction[2];
}


/////
// Set up the world coordinates given the initial position
/////
void initPosition()
{
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glTranslatef(-position[0], -position[1], -position[2]);
}


/////
// Materialize the actual position in the maze by a sphere and the actual
// direction by a line
// Draw a world coordinates basis
/////
void drawPosition()
{
	// get a new matrix
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		// translate to the actual position and draw the sphere
		glTranslatef(position[0], position[1], position[2]);
		glutSolidSphere(0.1f, 10, 10);
		// draw a line materializing the direction
		glBegin(GL_LINES);
			glVertex3f(0.f, 0.f, 0.f);
			glVertex3f(direction[0] * SQUARE_SIZE, direction[1] * SQUARE_SIZE, direction[2] * SQUARE_SIZE);
		glEnd();
	glPopMatrix();

	// Draw world coordinates basis 
	glPushMatrix();
	glTranslatef(-1.f, 0.f, 1.f);
	glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(0.5f, 0.f, 0.f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(0.f, 0.5f, 0.f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(0.f, 0.f, 0.f); glVertex3f(0.f, 0.f, 0.5f);
	glEnd();
	glPopMatrix();
}


/////
// Perform necessary transformations to see the maze from the top
/////
void topView()
{
	static float orthoPosVect[3];

	// compute the vector orthogonal to the current direction
	// it will be used as the rotation axis to obtain a top view
	orthoPosVect[0] = direction[2];
	orthoPosVect[1] = 0.f;
	orthoPosVect[2] = -direction[0];

	if(!top_view)
	{
		// rotate about the position in the maze
		glTranslatef(position[0], position[1], position[2]);
		glRotatef(-90.f, orthoPosVect[0], orthoPosVect[1], orthoPosVect[2]);
		glTranslatef(-position[0], -position[1], -position[2]);
		// move back to see the whole maze
		glTranslatef(0.f, -4.f, 0.f);
		top_view = true;
	}
	else
	{
		// undo the transformations
		glTranslatef(0.f, 4.f, 0.f);
		glTranslatef(position[0], position[1], position[2]);
		glRotatef(90.f, orthoPosVect[0], orthoPosVect[1], orthoPosVect[2]);
		glTranslatef(-position[0], -position[1], -position[2]); 
		top_view = false;
	}
}