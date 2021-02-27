#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <cmath>
#include <stdio.h>
#include <iostream>


#define ESCAPE 27

int window;
float rcube = 0.0f;

#define textureImageWidth 64
#define textureImageHeight 64
GLubyte textureImage[textureImageHeight][textureImageWidth][4];
GLuint texName;

void loadTexture(void)
{
    int i, j, c;
    for (i = 0; i < textureImageHeight; i++) {
        for (j = 0; j < textureImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
            textureImage[i][j][0] = (GLubyte)c;
            textureImage[i][j][1] = (GLubyte)c;
            textureImage[i][j][2] = (GLubyte)c;
            textureImage[i][j][3] = (GLubyte)255;
        }
    }
}

void readBMP()
{
    FILE* file = fopen("lena_small.bmp", "rb");
    
    unsigned char header[54];

    if(fread(header, sizeof(unsigned char), 54, file) == 54) {
        for (int i = 0; i < textureImageHeight; i++){
            for (int j = 0; j < textureImageWidth; j++){
                fread(&textureImage[i][j][2], sizeof(unsigned char), 1, file);
                fread(&textureImage[i][j][1], sizeof(unsigned char), 1, file);
                fread(&textureImage[i][j][0], sizeof(unsigned char), 1, file);
                textureImage[i][j][3] = 255;
            }
        }
    }
    fclose(file);
}



void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    fH = tan(fovY / 360 * pi) * zNear;
    fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
    glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
    glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
    glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
    glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix

    perspectiveGL(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);	// Calculate The Aspect Ratio Of The Window
    glMatrixMode(GL_MODELVIEW);


    // loadTexture();
    readBMP();
    glGenTextures(2, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImageWidth,
        textureImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        textureImage);
}

/* The main drawing function. */
void DrawGLScene()
{
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset the transformation matrix.
    glTranslatef(0.0f, 0.0f, -7.0f);		// Move Right 3 Units, and back into the screen 7
    glRotatef(rcube, 1.0f, 1.0f, 1.0f);		// Rotate The Cube On X, Y, and Z

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    glBindTexture(GL_TEXTURE_2D, texName);

    // draw a cube (6 quadrilaterals)
    glBegin(GL_QUADS);				// start drawing the cube.

    // top of cube
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, 1.0f, -1.0f);		// Top Right Of The Quad (Top)
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, 1.0f, -1.0f);		// Top Left Of The Quad (Top)
    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);		// Bottom Left Of The Quad (Top)
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);		// Bottom Right Of The Quad (Top)

    // bottom of cube
    glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, -1.0f, 1.0f);		// Top Right Of The Quad (Bottom)
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, 1.0f);		// Top Left Of The Quad (Bottom)
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);		// Bottom Left Of The Quad (Bottom)
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);		// Bottom Right Of The Quad (Bottom)

    // front of cube

    glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Front)
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Front)
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, 1.0f);		// Bottom Left Of The Quad (Front)
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, 1.0f);		// Bottom Right Of The Quad (Front)

    // back of cube.

    glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);		// Top Right Of The Quad (Back)
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);		// Top Left Of The Quad (Back)
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);		// Bottom Left Of The Quad (Back)
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, 1.0f, -1.0f);		// Bottom Right Of The Quad (Back)

    // left of cube

    glTexCoord2f(1.0, 1.0); glVertex3f(-1.0f, 1.0f, 1.0f);		// Top Right Of The Quad (Left)
    glTexCoord2f(1.0, 0.0); glVertex3f(-1.0f, 1.0f, -1.0f);		// Top Left Of The Quad (Left)
    glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f, -1.0f, -1.0f);		// Bottom Left Of The Quad (Left)
    glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f, -1.0f, 1.0f);		// Bottom Right Of The Quad (Left)

    // Right of cube

    glTexCoord2f(1.0, 1.0); glVertex3f(1.0f, 1.0f, -1.0f);	        // Top Right Of The Quad (Right)
    glTexCoord2f(1.0, 0.0); glVertex3f(1.0f, 1.0f, 1.0f);		// Top Left Of The Quad (Right)
    glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f, 1.0f);		// Bottom Left Of The Quad (Right)
    glTexCoord2f(0.0, 1.0); glVertex3f(1.0f, -1.0f, -1.0f);		// Bottom Right Of The Quad (Right)
    glEnd();					// Done Drawing The Cube
    glFlush();
    glDisable(GL_TEXTURE_2D);
    rcube -= 0.1f;					// Decrease The Rotation Variable For The Cube

    // swap the buffers to display, since double buffering is used.
    glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    usleep(100);
    if (key == ESCAPE)
    {
        glutDestroyWindow(window);
        exit(0);
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("Texture map");

    glutDisplayFunc(&DrawGLScene);
    glutIdleFunc(&DrawGLScene);
    glutKeyboardFunc(&keyPressed);
    InitGL(640, 480);

    glutMainLoop();

    return 1;
}