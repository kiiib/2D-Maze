/* Create a 2D Maze */
#include <iostream>
#include <stack>
#include <vector>
#include <time.h>
#include "Angel.h"

using namespace std;

const int MazeWidth = 10;	// Maze's Width
const int MazeHeight = 10;	// Maze's Height
const int eachCellNumPoints = 8;
const int NumPoints = MazeWidth * MazeWidth * eachCellNumPoints;
const int flag = MazeWidth * MazeWidth;
vec2 points[NumPoints];
float drawOffset = 0.01;
float w = 0.2;	// each cell's width and height
int cellNumber = 0;

int index(int coordinateX, int coordinateY) {
	//figure out the invalid index
	if (coordinateX < 0 || coordinateY < 0 || coordinateX > MazeWidth - 1 || coordinateY > MazeWidth - 1)
		return -1;

	return coordinateX + coordinateY * 10;	// 10, because of there is a 10x10 grid
}

class Cell {
public:
	int numCell;
	int flag;	// the gap between each cell is 8, 8 is the points of one cell
	float i;	// the location of OpenGL draw
	float j;
	int coordinateX;	// the location of coordinate
	int coordinateY;
	bool wallTop = true;
	bool wallRight = true;
	bool wallBottom = true;
	bool wallLeft = true;
	bool visited = false;
	bool isExist = true;

	void show() {
		if (wallTop) {
			points[flag] = vec2(i, j);
			points[flag + 1] = vec2(i + w, j);
		}

		if (wallRight) {
			points[flag + 2] = vec2(i + w, j);
			points[flag + 3] = vec2(i + w, j - w);
		}

		if (wallBottom) {
			points[flag + 4] = vec2(i + w, j - w);
			points[flag + 5] = vec2(i, j - w);
		}

		if (wallLeft) {
			points[flag + 6] = vec2(i, j - w);
			points[flag + 7] = vec2(i, j);
		}
		//cout << numCell << endl;
		//cout << coordinateX << "  " << coordinateY << endl;
	}

	Cell checkNeighbors(vector<Cell> myGrid) {
		vector<Cell> neighbors;
		//cout << coordinateX <<  "  "  << coordinateY - 1 << endl;

		//figure out the edge problem
		if (coordinateY - 1 >= 0) {
			Cell top = myGrid[index(coordinateX, coordinateY - 1)];
			if (!top.visited) {
				neighbors.push_back(top);
			}
		}
		if (coordinateX + 1 < MazeWidth) {
			Cell right = myGrid[index(coordinateX + 1, coordinateY)];
			if (!right.visited) {
				neighbors.push_back(right);
			}
		}
		
		if (coordinateY + 1 < MazeHeight) {
			Cell bottom = myGrid[index(coordinateX, coordinateY + 1)];
			if (!bottom.visited) {
				neighbors.push_back(bottom);
			}
		}
		
		if (coordinateX - 1 >= 0) {
			Cell left = myGrid[index(coordinateX - 1, coordinateY)];
			if (!left.visited) {
				neighbors.push_back(left);
			}
		}
		// if neighors are exist, pick up a random neigbor
		if (neighbors.size() > 0) {
			srand((unsigned)time(NULL));
			int random = (floor)(rand() % neighbors.size());
			//cout << random << endl;
			return neighbors[random];
		}
		else {
			neighbors[0].isExist = false;
			return neighbors[0];	// if neighbor doesn't exists, return nothing
		}
		
	}
};
void removeWalls(Cell a, Cell b) {
	int x = a.coordinateX - b.coordinateX;
	/*
		x = 1 means neighbor in the left side
		remove the current cell's left wall and neighbor's right wall

	*/
	if (x == 1) {
		a.wallLeft = false;
	}
}
vector<Cell> grid;
Cell currentCell;

void initGrid() {
	int coordinateX = 0;
	int coordinateY = 0;
	int numCell = 0;
	// init grid
	for (int j = 10; j > -10; j -= 2) {
		for (int i = -10; i < 10; i += 2) {
			Cell cell;
			cell.i = (float)i / 10;
			cell.j = (float)j / 10;
			cell.coordinateX = coordinateX;
			cell.coordinateY = coordinateY;
			cell.flag = cellNumber;
			cell.numCell = numCell;
			// push each cell into grid
			grid.push_back(cell);

			if (coordinateX < 9)
				coordinateX++;
			else
				coordinateX = 0;

			cellNumber += eachCellNumPoints;
			numCell++;
			//cout << cell.i << "  " << cell.j << endl;
		}
		if (coordinateY < 9)
			coordinateY++;
	}
}
void drawGrid() {
	currentCell = grid[55];	// set the first cell is current
	currentCell.visited = true;
	Cell nextCell = currentCell.checkNeighbors(grid);
	if (nextCell.isExist) {
		// mark next cell is visited
		nextCell.visited = true;
		// remove the wall between the current cell and the chosen cell
		removeWalls(currentCell, nextCell);
		// set it to be the next one
		currentCell = nextCell;
	}
	for (int i = 0; i < grid.size(); i++) {
		grid[i].show();
	}
}
void initMaze() {
	initGrid();
	drawGrid();
	
	// Create a vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Load shaders and use the resulting shader program
	GLuint program = InitShader("vshader21.glsl", "fshader21.glsl");
	glUseProgram(program);

	//Initialize the vertex position attribute from the vertex shader
	GLuint loc = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glClearColor(1.0, 1.0, 1.0, 0.8);	// background

}
void display() {
	glClear(GL_COLOR_BUFFER_BIT);	//	clear the window
	glDrawArrays(GL_LINES, 0, NumPoints);    // draw the points

	glFlush();
}
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033:
		exit(EXIT_SUCCESS);
		break;
	}
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	//check the version of freeglut
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutCreateWindow("2D Maze");

	glewInit();

	initMaze();

	// Callback function
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}
