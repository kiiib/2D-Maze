/* Create a 2D Maze */
#include <iostream>
#include "Angel.h"

using namespace std;

const int MazeWidth = 5;	// Maze's Width
const int MazeHeight = 5;	// Maze's Height
const int NumPoints = MazeWidth * MazeWidth * 2;
//const int NumPoints = 2;

int w = 50;
int cols = floor(glutGet(GLUT_WINDOW_WIDTH) / w);	// cols number = window's width divide by w
int rows = floor(glutGet(GLUT_WINDOW_HEIGHT) / w);

class Cell{
public:
	int i;
	int j;

};

void initMaze() {
	for (int j = 0; j < cols; j++) {
		for (int i = 0; i < rows; i++) {
			
		}
	}

	vec2 points[NumPoints];
	points[0] = vec2(-0.9, 0.9);
	points[1] = vec2(0.5, 0.9);
	points[2] = vec2(0.5, 0.9);
	points[3] = vec2(0.9, 0.9);

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
	glutInitWindowSize(500, 500);

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
