//////////////////////////////////////////////// /////////////////////
// //
// Code framework for lecture computer graphics WS 2022/23 Exercise 1 //
// //
//////////////////////////////////////////////// /////////////////////
#define GL_SILENCE_DEPRECATION
#include "vec.h"
#include "mat.h"
#include "glut.h"
#include "Point.cpp"
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

//
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// private, global variables ... replace by your own ones
//
// some global state variables used to describe ...
float g_iPos;        // ... position and ...
float g_iPosIncr;    // ... position increment (used in display1)
float r_direction;

//Main points
Point Sun;
Point Earth;
Point Moon;

CVec2i g_vecPos;        // same as above but in vector form ...
CVec2i g_vecPosIncr;    // (used in display2)

Color cRed(0.5, 0.1, 0.1);
Color cGreen(0.1, 0.5, 0.1);
Color cBlue(0.1, 0.1, 0.5);
Color cWhite(1, 1, 1);
//
/////////////////////////////////////////////////////////////

// function to initialize our own variables
void init ()
{

    // init timer interval
    g_iTimerMSecs = 10;

    // init variables for display1
    g_iPos     = 0;
    g_iPosIncr = 2;
    
    //rotation direction
    r_direction= 1;

    // init variable of solar system
    Sun.x = 0;
    Sun.y = 0;
    
    Earth.x = 100;
    Earth.y = 0;
    
    Moon.x = 150;
    Moon.y = 0;
    
    // init variables for display2
    int aiPos    [2] = {0, 0};
    int aiPosIncr[2] = {2, 2};
    
    g_vecPos.setData (aiPos);
    g_vecPosIncr.setData (aiPosIncr);
}

// function to initialize the view to ortho-projection
void initGL ()
{
    glViewport (0, 0, g_iWidth, g_iHeight);    // Establish viewing area to cover entire window.

    glMatrixMode (GL_PROJECTION);            // Start modifying the projection matrix.
    glLoadIdentity ();                        // Reset project matrix.
    glOrtho (-g_iWidth/2, g_iWidth/2, -g_iHeight/2, g_iHeight/2, 0, 1);    // Map abstract coords directly to window coords.

    // tell GL that we draw to the back buffer and
    // swap buffers when image is ready to avoid flickering
    glDrawBuffer (GL_BACK);

    // tell which color to use to clear image
    glClearColor (0,0,0,1);
}


int min (int a, int b) { return a>b? a: b; }

Point rotateAroundPt(Point rp, Point cp, float theta)
{
    Point fxy(
              ((rp.x - cp.x) * cos(theta*r_direction)) - ((rp.y - cp.y) * sin(theta*r_direction)) + cp.x,
              ((rp.x - cp.x) * sin(theta*r_direction)) + ((rp.y - cp.y) * cos(theta*r_direction)) + cp.y
              );
    
    return fxy;
}

// timer callback function
void timer (int value)
{
    ///////
    // update your variables here ...
    //
    
    Earth = rotateAroundPt(Earth, Sun,  0.01745329252);
    Moon = rotateAroundPt(Moon, Earth,  0.3490658504);



    int size2 = min (g_iWidth, g_iHeight) / 2;

    // variables for display1 ...
    if (g_iPos<=-size2 || g_iPos>=size2) g_iPosIncr = -g_iPosIncr;
    g_iPos += g_iPosIncr;

    // variables for display2 ...
    if (g_vecPos(1)<=-size2 || g_vecPos(1)>=size2) g_vecPosIncr = -g_vecPosIncr;
    g_vecPos += g_vecPosIncr;

    //
    ///////

    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}

void plotCircle(int x, int y, int px, int py, Color c){
    
    glBegin (GL_POINTS);
    
    Point xy0(x+px,y+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy0.x, xy0.y);

    Point xy1(y+px,x+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy1.x, xy1.y);

    Point xy2(-x+px,y+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy2.x, xy2.y);

    Point xy3(-y+px,x+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy3.x, xy3.y);

    Point xy4(x+px,-y+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy4.x, xy4.y);

    Point xy5(y+px,-x+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy5.x, xy5.y);

    Point xy6(-x+px,-y+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy6.x, xy6.y);

    Point xy7(-y+px,-x+py);
    glColor3f (c.r, c.g, c.b);
    glVertex2i (xy7.x, xy7.y);
    
    glEnd ();
}

void bresenhamCircle(Point mp, int r, Color c){
    glBegin (GL_POINTS);
    
    int x, y, p, d, DSE, DE;

    p = 0;
    x = p;
    y = r;
    d = 5 - 4*r;
    
    plotCircle(x, y, mp.x, mp.y, c);
    
    while (y>x) {
        if (d>=0)
        {
            DSE = 4*(2*(x-y)+5);
            d += DSE;
            x++;
            y--;
        } // SE
        else {
            DE = 4*(2*x+3);
            d +=DE ;
            x++;
        } // E
        plotCircle(x, y, mp.x, mp.y, c);
    }
    glEnd ();
}

// display callback function
void display1 (void)
{

    glClear (GL_COLOR_BUFFER_BIT);

    bresenhamCircle(Sun, 25, cRed);
    bresenhamCircle(Earth, 15, cGreen);
    bresenhamCircle(Moon, 7, cWhite);

    // In double buffer mode the last
    // two lines should alsways be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
}


// display callback function
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    ///////
    // display your data here ...
    //
    
    glBegin (GL_QUADS);
        glColor3f (1,0,0);
        glVertex2i (-g_vecPos(1), -g_vecPos(2));
        glColor3f (0,1,0);
        glVertex2i (g_vecPos(1), -g_vecPos(2));
        glColor3f (0,0,1);
        glVertex2i (g_vecPos(1), g_vecPos(2));
        glColor3f (1,1,0);
        glVertex2i (-g_vecPos(1), g_vecPos(2));
    glEnd ();

    //
    ///////

    // In double buffer mode the last
    // two lines should alsways be
    glFlush ();
    glutSwapBuffers (); // swap front and back buffer
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
            //glutPostRedisplay ();    // not needed since timer triggers redisplay
            break;
        case '2':
            r_direction = -1;
            glutDisplayFunc (display2);
            //glutPostRedisplay ();    // not needed since timer triggers redisplay
            break;
        case '3':
            r_direction = 1;
            break;
        case '4':
            r_direction = -1;
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
