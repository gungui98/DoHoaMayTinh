/* -- INCLUDE FILES ------------------------------------------------------ */
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


/* ----------------------------------------------------------------------- */


void myInit( void )  {
  glClearColor( 0.3, 0.1, 0.3, 0.3 );
  glColor3f( 0.0, 1.0, 0.0 );
  glPointSize( 1.0 );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity( );
  glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

/* ----------------------------------------------------------------------- */

/* ----------------------------------------------------------------------- */
void myDisplay( void )  {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glBegin(GL_POLYGON);
        	glColor3f(1.0f,0.0f,0.0f);
            glVertex3f(0.25, 0.25, 0.0);
	    	//glColor3f(0.0f,1.0f,0.0f);
            glVertex3f(0.75, 0.25, 0.0);
	        //glColor3f(0.5f,0.5f,1.0f);
            glVertex3f(0.75, 0.75, 0.0);
		    //glColor3f(0.0f,0.0f,1.0f);
            glVertex3f(0.25, 0.75, 0.0);

        glEnd();
		glFlush();
  	    glutSwapBuffers();

}

/* ----------------------------------------------------------------------- */

int main( int argc, char *argv[] )  {
  // Initialize GLUT.
  glutInit( &argc, argv );
  // Set the mode to draw in.
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
  // Set the window size in screen pixels.
  glutInitWindowSize( 640, 480 );
  // Set the window position in screen pixels.
  glutInitWindowPosition( 100, 150 );
  // Create the window.
  glutCreateWindow( "18020629_Lab01" );
  // Set the callback funcion to call when we need to draw something.


  myInit( );
  glutDisplayFunc( myDisplay );
  // Initialize some things.
  // Now that we have set everything up, loop responding to events.
  glutMainLoop( );
}

/* ----------------------------------------------------------------------- */
