
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

int Focal;
float z_angle;
float y_angle;
float x_angle;

float rotIndex;

////////////////////////////////////////////////////////////
//
// Variables
//

//Exercise 6 Parametrs
CVec4f EyePoint;
CVec4f ViewOrigin;
CVec4f ViewUp;
CVec4f ViewDir;


// Pre-set color
Color cRed(0.5, 0.1, 0.1);
Color cGreen(0.1, 0.5, 0.1);
Color cBlue(0.1, 0.1, 0.5);
Color cWhite(1, 1, 1);

int  cube[8][4] = {
    {50, 50, 50, 1},
    {0, 50, 50, 1},
    {0, 0, 50, 1},
    {50, 0, 50, 1},
    {50, 50, 0, 1},
    {0, 50, 0, 1},
    {0, 0, 0, 1},
    {50, 0, 0, 1},
};

int  cube2[8][4] = {
    {-50, -50, 50, 1},
    {0, -50, 50, 1},
    {0, 0, 50, 1},
    {-50, 0, 50, 1},
    {-50, -50, 0, 1},
    {0, -50, 0, 1},
    {0, 0, 0, 1},
    {-50, 0, 0, 1},
};

int  cube3[8][4] = {
    {-50, 50, 50, 1},
    {0, 50, 50, 1},
    {0, 0, 50, 1},
    {-50, 0, 50, 1},
    {-50, 50, 0, 1},
    {0, 50, 0, 1},
    {0, 0, 0, 1},
    {-50, 0, 0, 1},
};

int  cube4[8][4] = {
    {50, -50, 50, 1},
    {0, -50, 50, 1},
    {0, 0, 50, 1},
    {50, 0, 50, 1},
    {50, -50, 0, 1},
    {0, -50, 0, 1},
    {0, 0, 0, 1},
    {50, 0, 0, 1},
};


Point cube_m[8];
Point cube_m_2[8];
Point cube_m_3[8];
Point cube_m_4[8];

CVec4f vector(int x, int y, int z, int w){
    CVec4f cor;
    
    cor(0) = x;
    cor(1) = y;
    cor(2) = z;
    cor(3) = w;
    
    return cor;
};

// function to initialize our own variables
void init ()
{
    z_angle = 0;
    x_angle = 0;
    y_angle = 0;
    
    rotIndex = 0.05;
    
    // init timer interval
    g_iTimerMSecs = 10;
    
    ViewOrigin = vector(0, 0, 0, 1);
    ViewUp = vector(0,1,0, 0);
    ViewDir = vector(0,0,1, 0);
    
    Focal = 300;
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

void bhamLine (int x0, int y0, int x1, int y1, Color c) {
    
    glBegin (GL_POINTS);

    glColor3f (c.r, c.g, c.b);
    
    int d, dne, de, f, m1, m2, m3, m4, px, py;
    
    
    px = 0;
    py = 0;
    
    //Simple translation of the line position
    if( x0 == 0 && y0 == 0){
        px = 0;
        py = 0;
    } else{
        px = x0;
        py = y0;
        x0 =0;
        y0 =0;
        x1 = x1 - px;
        y1 = y1 - py;
    }

    int dy = y1 - y0;
    int dx = x1 - x0;
    
    //Checking the first flags
    bool flag1 = (dx>=0);
    bool flag2 = (dy>=0);
    bool flag3 = (abs(dy)>=abs(dx));

    
    ///////
    ///
    ///This part of code dose 2 things:
    /// - Uses the results of flags to determine wher is the line located;
    /// - Selects the correct Constans for multiplication;
    ///
    ///////
    
    if(flag1){
        if(flag2) {

            // x/y
            m1 = 1;
            m2 = 1;
            m3 = 1;
            m4 = 1;

        } else {
            // x/-y
            dx = abs(dx);
            dy = abs(dy);

            if(flag3){
                m1 = 1;
                m2 = 1;
                m3 = 1;
                m4 = -1;

            } else {
                m1 = 1;
                m2 = -1;
                m3 = 1;
                m4 = 1;
            }
        }
    } else{
        if(flag2) {
            //-x/y
            dx = abs(dx);
            dy = abs(dy);

            if(flag3){
                m1 = 1;
                m2 = 1;
                m3 = -1;
                m4 = 1;
            } else {
                m1 = -1;
                m2 = 1;
                m3 = 1;
                m4 = 1;
            }
        } else {
            //-x/-y
            m1 = -1;
            m2 = -1;
            m3 = -1;
            m4 = -1;
        }
    }
    
    //Bellow is the code that only thinks in the first 2 Octants
    //Its also adjusted to use the selected Constans

    x1 = abs(x1);
    y1 = abs(y1);

    dy = y1 - y0;
    dx = x1 - x0;

    flag1 = (dx>=0);
    flag2 = (dy>=0);
    flag3 = (abs(dy)>=abs(dx));
    

    if (flag3) {
        f = y1;
        y1 = x1;
        x1 = f;
    }

    dy = y1 - y0;
    dx = x1 - x0;
    
    dne = 2 * (dy-dx);
    de = 2 * dy;
    d = 2 * dy-dx;

    if (flag1) {
        if(flag2){
            if (!flag3) {

                glVertex2i (x0 + px, y0 + py);

            } else {

                glVertex2i (y0 + px, x0 + py);

            }
            while (x0 < x1) {

                if (d >= 0) {
                    d += dne;
                    x0++;
                    y0++;

                } else {
                    d += de;
                    x0++;
                }
                if (!flag3) {

                    glVertex2i(x0 * m1 + px, y0 * m2+ py);

                } else {

                    glVertex2i (y0 * m3 + px, x0 * m4 + py);

                }
            }
        }
    }
    
    glEnd ();
}

CMat4f projectMat(float d){
    
    CMat4f pot;
    pot(0,0) = 1;
    pot(0,1) = 0;
    pot(0,2) = 0;
    pot(0,3) = 0;
    pot(1,0) = 0;
    pot(1,1) = 1;
    pot(1,2) = 0;
    pot(1,3) = 0;
    pot(2,0) = 0;
    pot(2,1) = 0;
    pot(2,2) = 0;
    pot(2,3) = 0;
    pot(3,0) = 0;
    pot(3,1) = 0;
    pot(3,2) = -(1/d);
    pot(3,3) = 1;
    
    return pot;
}

//Step One
CMat4f zRotate(float angle) {
    
    CMat4f one;

    one(0,0) = cos(angle);
    one(0,1) = -sin(angle);
    one(0,2) = 0;
    one(0,3) = 0;
    one(1,0) = sin(angle);
    one(1,1) = cos(angle);
    one(1,2) = 0;
    one(1,3) = 0;
    one(2,0) = 0;
    one(2,1) = 0;
    one(2,2) = 1;
    one(2,3) = 0;
    one(3,0) = 0;
    one(3,1) = 0;
    one(3,2) = 0;
    one(3,3) = 1;
    
    return  one;
}

//Step One
CMat4f xRotate(float angle) {
    
    CMat4f one;

    one(0,0) = 1;
    one(0,1) = 0;
    one(0,2) = 0;
    one(0,3) = 0;
    
    one(1,0) = 0;
    one(1,1) = cos(angle);
    one(1,2) = -sin(angle);
    one(1,3) = 0;
    
    one(2,0) = 0;
    one(2,1) = sin(angle);
    one(2,2) = cos(angle);
    one(2,3) = 0;

    one(3,0) = 0;
    one(3,1) = 0;
    one(3,2) = 0;
    one(3,3) = 1;
    
    return  one;
}

//Step two
CMat4f yRotate(float angle) {
    
    CMat4f two;
    two(0,0) = cos(angle);
    two(0,1) = 0;
    two(0,2) = sin(angle);
    two(0,3) = 0;
    
    two(1,0) = 0;
    two(1,1) = 1;
    two(1,2) = 0;
    two(1,3) = 0;
    
    two(2,0) = -sin(angle);
    two(2,1) = 0;
    two(2,2) = cos(angle);
    two(2,3) = 0;
    
    two(3,0) = 0;
    two(3,1) = 0;
    two(3,2) = 0;
    two(3,3) = 1;
    
    return two;
}

//Not sure if this is how the teacher wanted to do it.
CMat4f xyzRotation() {
    CMat4f calc = xRotate(x_angle) * yRotate(y_angle) * zRotate(z_angle);
    return calc;
}


Point calculateCube(CVec4f cor, CMat4f pro){
    
    CVec4f calc = pro * cor;
    calc = calc * (1/calc(3));
    
    Point r;
    r.x = calc(0);
    r.y = calc(1);
    
    return r;
}

CVec4f crossProduct(CVec4f one, CVec4f two){
    
    CVec4f cross;
    
    cross(0) = one(1) * two(2) - one(2) * two(1);
    cross(1) = one(2) * two(0) - one(0) * two(2);
    cross(2) = one(0) * two(1) - one(1) * two(0);
    cross(3) = 0.0f;
    
    return cross;
}

CMat4f getTransform(CVec4f ViewOrigin, CVec4f ViewDir, CVec4f ViewUp){
    
    CMat4f Transform;
    
    CVec4f cross = crossProduct(ViewDir, ViewUp);

    Transform(0,0) = cross(0);
    Transform(1,0) = cross(1);
    Transform(2,0) = cross(2);
    Transform(3,0) = cross(3);
    
    Transform(0,1) = ViewUp(0);
    Transform(1,1) = ViewUp(1);
    Transform(2,1) = ViewUp(2);
    Transform(3,1) = ViewUp(3);
    
    Transform(0,2) = ViewDir(0);
    Transform(1,2) = ViewDir(1);
    Transform(2,2) = ViewDir(2);
    Transform(3,2) = ViewDir(3);
    
    Transform(0,3) = ViewOrigin(0);
    Transform(1,3) = ViewOrigin(1);
    Transform(2,3) = ViewOrigin(2);
    Transform(3,3) = ViewOrigin(3);

    return Transform;
}


//3x3 matrix
CMat4f transpose(CMat4f mat)
{
    CMat4f result;
    
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            result(i, j) = mat(j, i);
        }
    }
    
    return result;
}

//This is the combination of the matrixesuestion about this!
//Need to ask a q

CMat4f finalMat(){

    CMat4f transposed = transpose(getTransform(ViewOrigin, ViewDir, ViewUp));
    
    //RT*P
    CMat4f ViewOriginMat;
    
    ViewOriginMat(0,0) = ViewOrigin(0);
    ViewOriginMat(1,0) = ViewOrigin(1);
    ViewOriginMat(2,0) = ViewOrigin(2);
    ViewOriginMat(3,0) = ViewOrigin(3);
    
    CMat4f calc = ViewOriginMat * transposed;
    
    CMat4f Transform;
    
    Transform(0,0) = transposed(0,0);
    Transform(1,0) = transposed(1,0);
    Transform(2,0) = transposed(2,0);
    Transform(3,0) = 0;

    Transform(0,1) = transposed(0,1);
    Transform(1,1) = transposed(1,1);
    Transform(2,1) = transposed(2,1);
    Transform(3,1) = 0;

    Transform(0,2) = transposed(0,2);
    Transform(1,2) = transposed(1,2);
    Transform(2,2) = transposed(2,2);
    Transform(3,2) = 0;

    //?????
    Transform(0,3) = calc(1,0);
    Transform(1,3) = calc(2,0);
    Transform(2,3) = calc(3,0);
    Transform(3,3) = 1;
    
    return Transform;
}


CVec4f projectZallg(CVec4f cor){
    
    CVec4f calc = finalMat() * cor * xyzRotation(); 
    
    return calc;
}

void calcPoints (int cube[8][4], float d, Point cube_new[8]){
    
    
    for (int i=0; i < 8; i++) {
        CVec4f vectAlg = projectZallg(vector(cube[i][0], cube[i][1], cube[i][2], cube[i][3]));
        cube_new[i] = calculateCube(vectAlg, projectMat(d));
    }

}

void incFocal(){
    Focal += 10;
}

void decFocal(){
    Focal -= 10;
}

void ZAxis(float angle){
    ViewOrigin = ViewOrigin * zRotate(angle);
    ViewDir = ViewDir * zRotate(angle);
    ViewUp = ViewUp * zRotate(angle);
}


void YAxis(float angle){
    ViewOrigin = ViewOrigin * yRotate(angle);
    ViewDir = ViewDir * yRotate(angle);
    ViewUp = ViewUp * yRotate(angle);
}

//Horizon is the cross product of view dir and up!
void XAxis(float angle){
    ViewOrigin = ViewOrigin * xRotate(angle);
    ViewDir = ViewDir * xRotate(angle);
    ViewUp = ViewUp * xRotate(angle);
}

void reset(){
    ViewOrigin = vector(0, 0, 0, 1);
    ViewUp = vector(0,1,0, 0);
    ViewDir = vector(0,0,1, 0);
    Focal = 300;
}


// timer callback function
void timer (int value)
{
    calcPoints(cube, Focal, cube_m);
    calcPoints(cube2, Focal, cube_m_2);
    calcPoints(cube3, Focal, cube_m_3);
    calcPoints(cube4, Focal, cube_m_4);

    //Display2
    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}


void drawCube2(Point newCube[8], Color c){
    
    bhamLine(newCube[0].x, newCube[0].y, newCube[1].x, newCube[1].y, c);
    bhamLine(newCube[1].x, newCube[1].y, newCube[2].x, newCube[2].y, c);
    bhamLine(newCube[2].x, newCube[2].y, newCube[3].x, newCube[3].y, c);
    bhamLine(newCube[3].x, newCube[3].y, newCube[0].x, newCube[0].y, c);
   
    bhamLine(newCube[0].x, newCube[0].y, newCube[4].x, newCube[4].y, c);
    bhamLine(newCube[1].x, newCube[1].y, newCube[5].x, newCube[5].y, c);
    bhamLine(newCube[2].x, newCube[2].y, newCube[6].x, newCube[6].y, c);
    bhamLine(newCube[3].x, newCube[3].y, newCube[7].x, newCube[7].y, c);
    
    bhamLine(newCube[4].x, newCube[4].y, newCube[5].x, newCube[5].y, c);
    bhamLine(newCube[5].x, newCube[5].y, newCube[6].x, newCube[6].y, c);
    bhamLine(newCube[6].x, newCube[6].y, newCube[7].x, newCube[7].y, c);
    bhamLine(newCube[7].x, newCube[7].y, newCube[4].x, newCube[4].y, c);

}


// display callback function
void display1 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);


    glFlush ();
    glutSwapBuffers ();
}

// display callback function
void display2 (void)
{
    glClear (GL_COLOR_BUFFER_BIT);

    drawCube2(cube_m, cWhite);
    drawCube2(cube_m_2, cWhite);
    drawCube2(cube_m_3, cWhite);
    drawCube2(cube_m_4, cWhite);


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
        case 'f':
            incFocal();
            break;
        case 'F':
            decFocal();
            break;
        case 'z':
            ZAxis(-0.1);
            break;
        case 'Z':
            ZAxis(0.1);
            break;
        case 'y':
            YAxis(-0.1);
            break;
        case 'Y':
            YAxis(0.1);
            break;
        case 'x':
            XAxis(-0.1);
            break;
        case 'X':
            XAxis(0.1);
            break;
        case 'R':
            reset();
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
