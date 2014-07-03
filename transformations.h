#include "globals.h"

/////
// Move one step forward in the maze
/////
extern void moveForward();

/////
// Move one step backward in the maze
/////
extern void moveBack();

/////
// Perform a left or right rotation according to the argument
/////
extern void turn(MazeTurnDirection turnDir);


/////
// Set up the world coordinates given the initial position
/////
extern void initPosition();

/////
// Materialize the actual position in the maze by a sphere and the actual
// direction by a line
// Draw a world coordinates basis
/////
extern void drawPosition();

/////
// Perform necessary transformations to see the maze from the top
/////
extern void topView();
