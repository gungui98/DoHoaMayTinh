#include <windows.h>
#include <cmath>
#include <cstdio>
// #include <unistd.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define MAX_N 1004

float** A;
float** B;
int numberOfVertexes;

void init() {
	// select clearing (background) color
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800.0, 0, 600, -1.0, 1.0);
	//	gluOrtho2D(0.0, 800.0, 0.0, 600.0);
}

void initShape() {
	numberOfVertexes = 8;

	A = new float* [MAX_N];
	B = new float* [MAX_N];
	for (int i = 0; i < MAX_N; ++i) {
		A[i] = new float[2];
		B[i] = new float[2];
	}
	// Than ngoi nha
	A[0][0] = 600.0; A[0][1] = 300.0;
	A[1][0] = 500.0; A[1][1] = 335.0;
	A[2][0] = 310.0; A[2][1] = 400.0;
	A[3][0] = 310.0; A[3][1] = 400.0;
	A[4][0] = 185.0; A[4][1] = 350.0;
	A[5][0] = 40.0;  A[5][1] = 300.0;
	A[6][0] = 40.0;  A[6][1] = 40.0;
	A[7][0] = 600.0;  A[7][1] = 40.0;
	A[8][0] = 600.0; A[8][1] = 40.0;
	// Than oto
	B[0][0] = 600.0; B[0][1] = 300.0;
	B[1][0] = 450.0; B[1][1] = 300.0;
	B[2][0] = 400.0; B[2][1] = 350.0;
	B[3][0] = 200.0; B[3][1] = 350.0;
	B[4][0] = 150.0; B[4][1] = 300.0;
	B[5][0] = 40.0; B[5][1] = 300.0;
	B[6][0] = 40.0; B[6][1] = 200.0;
	B[7][0] = 600.0; B[7][1] = 200.0;
}

float tweening(float a, float b, float t) {
    /// exponential function
    //printf("%f %f\n", a * pow(b /a, t), (1 - t) * a + t * b);
    return a * pow(b / a, t);
	///linear
	//return (1 - t) * a + t * b;
	/// t from 0 to 1.
	// implement tweening function with different
	// interpolation function ( sinusoidal, quadraic, exponential) .
}

void drawTweening(float** A, float** B, int n, float t) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < n; ++i) {
		float Px = tweening(A[i][0], B[i][0], t);
		float Py = tweening(A[i][1], B[i][1], t);
		glVertex2f(Px, Py);
	}
	glEnd();
	glFlush();
}

void display(void) {
	for (float t = 0.0, deltaT = 0.01; ; t += deltaT) {
		glClear(GL_COLOR_BUFFER_BIT);
		drawTweening(A, B, numberOfVertexes, t);
		glutSwapBuffers();
		if (t >= 1.0 || t < 0.0) deltaT = -deltaT;
		Sleep(20);
	}
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	glutInitWindowSize(800, 600);

	// window position
	glutInitWindowPosition(100, 100);

	// title bar
	glutCreateWindow("Tweening");

	// call initialization routines
	init();

	initShape();

	// Register callback function to display graphics
	glutDisplayFunc(display);

	// Enter main loop and process events
	glutMainLoop();
}

