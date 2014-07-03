#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "maze.h"

void vectorprint(int *vector, int size)
{
	int i;

	for(i=0;i<size;i++)
		fprintf(stderr,"%3d",vector[i]);
	fprintf(stderr,"\n");
}

Maze::Maze()
{
	int i, j;

	/* zeroing xwall and ywall */
	for(i=0;i<MAX_Y;i++){
		for(j=0;j<MAX_X;j++) {
			ywall[i][j]=0;
			xwall[i][j]=0;
		}
	}
	maxx=0; maxy=0; initialx=0; initialy=0;
}


int Maze::read_xwalls(char *line, int linesize, int *wall)
{
	int i;
	int initialx=-1;
	int position;

	for(i=0;i<MAX_X;i++) {
		position=2*i;
		if(position<linesize) {
			if(line[position]=='|') wall[i]=1;
			else wall[i]=0;
		}
		else wall[i]=0;

		if(position+1<linesize)
			if(line[position+1]=='*') initialx=i;
	}
	return initialx;
}

void Maze::read_ywalls(char *line, int linesize, int *wall)
{
	int i, position;

	for(i=0;i<MAX_X;i++) {
		position=2*i+1;
		if(position<linesize) {
			if(line[position]=='-') wall[i]=1;
			else wall[i]=0;
		}
		else wall[i]=0;
	}
}

void Maze::load(void)
{
	FILE *file;
	int bywall[MAX_Y][MAX_X]; // these will store things in reverse y order
	int bxwall[MAX_Y][MAX_X]; // since reading is done starting with the max y
	int linesize;
	int x,y;
	char line[MAX_LINE_SIZE];
	int i,j;
	
	file=fopen("maze.txt","r");
	if(file==NULL) { fprintf(stderr,"error reading maze.txt\n"); exit(1);}

	y=0;
	while(1) {
		/* get ywalls */
		if(fgets(line,MAX_LINE_SIZE,file)==NULL) break;
		linesize = strlen(line)-1;
		read_ywalls(line,linesize , &(bywall[y][0]));
		

		/* get xwalls */
		if(fgets(line,MAX_LINE_SIZE,file)==NULL) break;
		linesize = strlen(line)-1;
		x=read_xwalls(line,linesize , &(bxwall[y][0]));
		if(x>=0){initialx=x;initialy=y;}
		y++;
	}

	for(i=y+1;i<MAX_Y;i++){
		for(j=0;j<MAX_X;j++) {
			bywall[i][j]=0;
			bxwall[i][j]=0;
		}
	}


	/* find maximum x and y */

	maxx=0; maxy=0;
	for(i=0;i<MAX_Y;i++)
		for(j=0;j<MAX_X;j++) {
			/* for max y, look for bottom |.  Since this is in
                           order of increasing y, can just update when seen */
			if(bxwall[i][j]==1) maxy=i;
			if(bywall[i][j]==1 && j>maxx) maxx=j;
		}

	/* reverse y order */
	for(i=0;i<=maxy;i++)
		for(j=0;j<MAX_X;j++) 
			xwall[maxy-i][j]=bxwall[i][j];
	for(i=0;i<=maxy+1;i++)
		for(j=0;j<MAX_X;j++) 
			ywall[maxy-i+1][j]=bywall[i][j];

	initialy=maxy-initialy;
			
}
