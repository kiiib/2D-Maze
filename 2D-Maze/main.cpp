/* Create a 2D Maze */
#include <iostream>
#include <stack>
#include <vector>
#include "Angel.h"

using namespace std;

const int MazeWidth = 10;	// Maze's Width
const int MazeHeight = 10;	// Maze's Height
const int NumPoints = MazeWidth * MazeWidth * 8;
const int flag = MazeWidth * MazeWidth;
vec2 points[NumPoints];
float drawOffset = 0.01;
float w = 0.2;
int cellNumber = 0;

class Cell{
public:
	int flag;
	float i;
	float j;
	bool wallTop = true;
	bool wallRight = true;
	bool wallBottom = true;
	bool wallLeft = true;

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
		
	}
};

void initMaze() {
	
	
	/*int cols = floor(glutGet(GLUT_WINDOW_WIDTH) / w);
	int rows = floor(glutGet(GLUT_WINDOW_HEIGHT) / w);*/
	vector<Cell> grid;
	
	// init grid
	for (int j = 10; j > -10; j -= 2) {
		for (int i = -10; i < 10; i += 2) {
			Cell cell;
			cell.i = (float)i / 10;
			cell.j = (float)j / 10;
			cell.flag = cellNumber;
			cell.show();
			grid.push_back(cell);
			cellNumber += 8;
			//cout << cell.i << "  " << cell.j << endl;
		}
	}

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
	glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
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
