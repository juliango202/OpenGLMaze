#ifndef _K_MAZE
#define _K_MAZE 

#define MAX_LINE_SIZE 67
#define MAX_X 33
#define MAX_Y 33

class Maze { 
public: 
	int ywall[MAX_Y][MAX_X]; 
	int xwall[MAX_Y][MAX_X]; 
	int initialx, initialy;
	int maxx, maxy;

private:
	int read_xwalls(char *line, int linesize, int *wall);
	void read_ywalls(char *line, int linesize, int *wall);
public:
	Maze();
	void Maze::load(void);
};
#endif
