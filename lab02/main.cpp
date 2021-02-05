#include<GL/glut.h>
#include<GL/gl.h>
#include<iostream>
#include<cmath>

using namespace std;

float field_of_view = 35;
float rotate_degree = 0;
float rotation_speed = 0;
void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;

    //fH = tan( (fovY / 2) / 180 * pi ) * zNear;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void init_camera_light(){

    glMatrixMode(GL_PROJECTION);

    glViewport(0, 0,640,480);

    glLoadIdentity();

    float aspect = 640/480.0;
    perspectiveGL(field_of_view,aspect, 1.0, 500.0 );

    glMatrixMode(GL_MODELVIEW);

    glClearDepth(1.);
    glEnable(GL_DEPTH_TEST);

    float ambiant_light[] = {0.5, 0.5, 0.5, 1.0};
    float diffuse_light[] = {0.1, 0.1, 0.1, 1.0};
    float specular_light[] = {0.1, 0.1, 0.1, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambiant_light);
    glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glShadeModel(GL_FLAT);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glEnable(GL_DEPTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0,0,-5);
    glRotatef(45,1,0,0);
    glRotatef(-45,0,1,0);

    init_camera_light();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0);
    glTranslatef(0.0,0.0,0.0);
    glRotatef(rotate_degree,0,1,0);
    rotate_degree += rotation_speed;

    glPushMatrix();
    glutSolidTeapot(1);
    glPopMatrix();

    glutSwapBuffers();
}

void mouse_function(int key, int state, int x, int y){
    cout << key<< endl;
    if(key == 3){
        rotation_speed += 0.01;
    }
    if(key == 4){
        rotation_speed -= 0.01;
    }
}

void keyboard(unsigned char key, int x, int y){
    cout << key;
    // zoom in zoom out by press keyboard
}

int main(int argc, char* args[]){
    glutInit(&argc,args);
    glutInitDisplayMode(GLUT_DOUBLE| GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(640, 480);
    glutCreateWindow("3D shape and keyboard, mouse");
    init_camera_light();
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutMouseFunc(mouse_function);
    glutKeyboardFunc(keyboard);


    glutMainLoop();
}
