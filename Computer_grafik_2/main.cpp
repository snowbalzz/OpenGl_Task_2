
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

////////////////////////////////////////////////////////////
//
// private, global variables ... replace by your own ones
//
float r_direction;
float w_const;

//Main points
Point Sun;
Point Earth;
Point Moon;

Point_h Sun_h_2;
Point_h Earth_h_2;
Point_h Moon_h_2;

Point Sun_h;
Point Earth_h;
Point Moon_h;

Color cRed(0.5, 0.1, 0.1);
Color cGreen(0.1, 0.5, 0.1);
Color cBlue(0.1, 0.1, 0.5);
Color cWhite(1, 1, 1);

// function to initialize our own variables
void init ()
{
    // init timer interval
    g_iTimerMSecs = 10;
    
    //Init variable of solar systems
    //Display 1
    r_direction= 1;
    Sun.x = 0;
    Sun.y = 0;
    Earth.x = 100;
    Earth.y = 0;
    Moon.x = 150;
    Moon.y = 0;
    
    //Display 2
    w_const = 1.5;
    if(w_const == 0) {
        throw std::invalid_argument("W parametr can not be 0!");
    }
    
    Sun_h.x = Sun.x/w_const;
    Sun_h.y = Sun.y/w_const;
    Earth_h.x = Earth.x/w_const;
    Earth_h.y = Earth.y/w_const;
    Moon_h.x = Moon.x/w_const;
    Moon_h.y = Moon.y/w_const;
    
    // init variables for display2
//    int aiPos    [2] = {0, 0};
//    int aiPosIncr[2] = {10, 10};
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

Point rotateAroundPtRegular(Point rp, Point cp, float theta)
{
    Point fxy(
              ((rp.x - cp.x) * cos(theta*r_direction)) - ((rp.y - cp.y) * sin(theta*r_direction)) + cp.x,
              ((rp.x - cp.x) * sin(theta*r_direction)) + ((rp.y - cp.y) * cos(theta*r_direction)) + cp.y
              );
    
    return fxy;
}

Point_h rotateAroundPtHomo(Point_h rp, Point_h cp, float theta)
{
    
    rp.x =((rp.x - cp.x) * cos(theta*r_direction)) - ((rp.y - cp.y) * sin(theta*r_direction)) + cp.x;
    rp.y =((rp.x - cp.x) * sin(theta*r_direction)) + ((rp.y - cp.y) * cos(theta*r_direction)) + cp.y;
    
    return rp;
}

// timer callback function
void timer (int value)
{

    //Display 1
    Earth = rotateAroundPtRegular(Earth, Sun,  0.01745329252);
    Moon = rotateAroundPtRegular(Moon, Earth,  0.3490658504);
    
    Earth_h = rotateAroundPtRegular(Earth_h, Sun_h,  0.01745329252);
    Moon_h = rotateAroundPtRegular(Moon_h, Earth_h,  0.3490658504);

    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}

void plotCircle(int x, int y, int px, int py, Color c){
    
    glBegin (GL_POINTS);
    
    //Hate this part of the code
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

void bresenhamCircleHomo(Point_h mp, int r, Color c){
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

    //Completed the First task
    bresenhamCircle(Sun, 25, cRed);
    bresenhamCircle(Earth, 15, cGreen);
    bresenhamCircle(Moon, 7, cWhite);

    glFlush ();
    glutSwapBuffers ();
}


// display callback function
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);
    
    //2nd Tasks implementation here
    bresenhamCircle(Sun_h, 25, cRed);
    bresenhamCircle(Earth_h, 15, cGreen);
    bresenhamCircle(Moon_h, 7, cWhite);
    
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
        case '2':
            glutDisplayFunc (display2);
            break;
        case '3':
            if(r_direction == 1){
                r_direction = -1;
            } else {
                r_direction = 1;
            }
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
    glutDisplayFunc (display2);
    // you might want to add a resize function analog to
    // �bung1 using code similar to the initGL function ...

    // start main loop
    glutMainLoop ();

    return 0;
}