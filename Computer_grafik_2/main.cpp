
#define GL_SILENCE_DEPRECATION
#include "vec.h"
#include "mat.h"
#include "glut.h"

#include "Point.cpp"
#include "Point_h.cpp"
#include "Color.cpp"

#include <stdlib.h>
#include <cmath>
#include <iostream>

////////////////////////////////////////////////////////////
//
// system relevant global variables
//

// window width and height (choose an appropriate size)
const int g_iWidth  = 400;
const int g_iHeight = 400;

// global variable to tune the timer interval
int g_iTimerMSecs;

const float ka = 0.1;
const float kd = 0.3;
const float ks = 0.6;

CVec3f m;
float r;

////////////////////////////////////////////////////////////
//
// Variables
//

//Exercise 6 Parametrs
CVec3f EyePoint;
CVec3f ViewOrigin;
CVec3f ViewUp;
CVec3f ViewDir;


// Pre-set color
Color cRed(0.5, 0.1, 0.1);
Color cGreen(0.1, 0.5, 0.1);
Color cBlue(0.1, 0.1, 0.5);
Color cWhite(1, 1, 1);

CVec3f vector(int x, int y, int z){
    CVec3f cor;
    
    cor(0) = x;
    cor(1) = y;
    cor(2) = z;
    
    return cor;
};

// function to initialize our own variables
void init ()
{
    // init timer interval
    g_iTimerMSecs = 10;
    
    EyePoint = vector(0, 0, 100);
    ViewDir = vector(0,0,-1);
    m = vector(0,0, 0);
    r = 50;
}

// function to initialize the view to ortho-projection
void initGL ()
{
    glViewport (0, 0, g_iWidth, g_iHeight);    // Establish viewing area to cover entire window.

    glMatrixMode (GL_PROJECTION);            // Start modifying the projection matrix.
    glLoadIdentity ();                        // Reset project matrix.
    glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);    // Map abstract coords directly to window coords.

    // tell GL that we draw to the back buffer and // swap buffers when image is ready to avoid flickering
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}

CVec3f calcNormal(CVec3f hitPos) {
    CVec3f normal;
    
    normal(0) = hitPos(0);
    normal(1) = hitPos(1);
    normal(2) = hitPos(2);
    
    float calculation = sqrt(normal(0) * normal(0) + normal(1) * normal(1) + normal(2) * normal(2));

    normal(0) /= calculation;
    normal(1) /= calculation;
    normal(2) /= calculation;
    
    return normal;
}

CVec3f phong(CVec3f HitPos, CVec3f EyePos) {
    CVec3f calc;
    
    CVec3f LightPos = vector(100, 100, 100);
    CVec3f LightColor = vector(1, 1, 1);
    
    CVec3f NormalHit = calcNormal(HitPos);
    
    CVec3f LightDir = LightPos - HitPos;
    float DiffuseCoeff = std::max(0.0f, NormalHit * LightDir);

    
    CVec3f Halfway = (LightDir + (EyePos - HitPos));
    float Spec = std::max(0.0f, NormalHit * Halfway);
    float SpecularCoeff = std::pow(Spec, 40);

    return LightColor * (ka + DiffuseCoeff * kd + SpecularCoeff * ks);
    
    return calc;
}

CVec3f intersect (CVec3f ep, CVec3f vd){
    CVec3f result;
    
    float a = vd * vd;
    float b = 2*(vd * (ep - m));
    float c = (ep - m) * (ep - m) - r*r ;

    float discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0)
    {
        result = vector(0, 0, -1);
        return result;
    }
    
    float t1 = (-b + sqrt(discriminant)) / (2 * a);
    float t2 = (-b - sqrt(discriminant)) / (2 * a);
    
    CVec3f p1 = ep + vd.operator*(t1);
    CVec3f p2 = ep + vd.operator*(t2);
    
    if (ep(2) > p1(2) && p1(2) > p2(2)){
        return p1;
    }
    
    return  p2;
}

// timer callback function
void timer (int value)
{

    //Display2
    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}

// display callback function
void display1 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    
    CVec3f inter = intersect(EyePoint,ViewDir);
    
    CVec3f pCalc = phong(inter, EyePoint);
    
    glFlush ();
    glutSwapBuffers ();
}

// display callback function
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    glFlush ();
    glutSwapBuffers ();
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
        case 'Q':
            exit (0); // quit program
            break;
        case '1':
            glutDisplayFunc (display1);
            break;
        default:
            // do nothing ...
            break;
    };
}

int main (int argc, char **argv)
{
    glutInit (&argc, argv);
    // we have to use double buffer to avoid flickering
    // TODO: lookup "double buffer", what is it for, how is it used ...
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutCreateWindow ("Task2");

    init ();    // init my variables first
    initGL ();    // init the GL (i.e. view settings, ...)

    // assign callbacks
    glutTimerFunc (10, timer, 0);
    glutKeyboardFunc (keyboard);
    glutDisplayFunc (display1);
    // you might want to add a resize function analog to
    // �bung1 using code similar to the initGL function ...

    // start main loop
    glutMainLoop ();

    return 0;
}
