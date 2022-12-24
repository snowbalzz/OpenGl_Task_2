
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

////////////////////////////////////////////////////////////
//
// Variables
//
////Display 2
//Point Sun_m;
//Point Earth_m;
//Point Moon_m;
//
////Display 1
//Point Sun;
//Point Earth;
//Point Moon;

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
    {50, 50, 100, 1},
    {-50, 50, 100, 1},
    {-50, -50, 100, 1},
    {50, -50, 100, 1},
    {50, 50, 0, 1},
    {-50, 50, 0, 1},
    {-50, -50, 0, 1},
    {50, -50, 0, 1},
};

Point cube_m[8];

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
    // init timer interval
    g_iTimerMSecs = 10;
    
    ViewOrigin = vector(0, 0, 0, 1);
    ViewUp = vector(0,1,0, 0);
    ViewDir = vector(0,0,1, 0);
    
    Focal = 300;
//    Sun.x = 0;
//    Sun.y = 0;
//    Earth.x = -100;
//    Earth.y = 0;
//    Moon.x = -150;
//    Moon.y = 0;
//
//    Sun_m.x = 0;
//    Sun_m.y = 0;
//    Earth_m.x = 100;
//    Earth_m.y = 0;
//    Moon_m.x = 150;
//    Moon_m.y = 0;
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

//// Rotation arounf point, Affine
//Point rotPtReg(Point rp, Point cp, float theta)
//{
//    Point fxy(
//              ((rp.x - cp.x) * cos(theta)) - ((rp.y - cp.y) * sin(theta)) + cp.x,
//              ((rp.x - cp.x) * sin(theta)) + ((rp.y - cp.y) * cos(theta)) + cp.y
//              );
//    return fxy;
//}

//// Setting the translation matrix
//CMat3f transMat(float x, float y) {
//
//    CMat3f tran;
//    tran(0,0) = 1;
//    tran(0,1) = 0;
//    tran(0,2) = x;
//    tran(1,0) = 0;
//    tran(1,1) = 1;
//    tran(1,2) = y;
//    tran(2,0) = 0;
//    tran(2,1) = 0;
//    tran(2,2) = 1;
//
//    return tran;
//}

//// Setting the rotation matrix
//CMat3f rotMat(float theta) {
//
//    CMat3f rot;
//    rot(0,0) = cos(theta);
//    rot(0,1) = -sin(theta);
//    rot(0,2) = 0;
//    rot(1,0) = sin(theta);
//    rot(1,1) = cos(theta);
//    rot(1,2) = 0;
//    rot(2,0) = 0;
//    rot(2,1) = 0;
//    rot(2,2) = 1;
//
//    return rot;
//}

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
    
    CVec4f calc = finalMat() * cor;
    
    return calc;
}

void calcPoints (int cube[8][4], float d){
    
    
    for (int i=0; i < 8; i++) {
        CVec4f vectAlg = projectZallg(vector(cube[i][0], cube[i][1], cube[i][2], cube[i][3]));
        cube_m[i] = calculateCube(vectAlg, projectMat(d));
    }

}

void incFocal(){
    Focal += 10;
}

void decFocal(){
    Focal -= 10;
}

// Rotation around point, Homogeneous
//Point rotPtHomo(Point rp, Point cp, float theta)
//{
//    CMat3f nMat = transMat( cp.x, cp.y);
//    CMat3f rMat = rotMat(theta);
//    CMat3f pMat = transMat( rp.x, rp.y);
//    CMat3f ngMat = transMat( -cp.x, -cp.y);
//
//    CMat3f calc = nMat * rMat * pMat * ngMat;
//
//    Point r;
//    r.x = calc(0, 2);
//    r.y = calc(1, 2);
//
//    return r;
//}

// timer callback function
void timer (int value)
{

//    Earth = rotPtReg(Earth, Sun,  0.00645329252);
//    Moon = rotPtReg(Moon, Earth,  0.1290658504);
//
//    Earth_m = rotPtHomo(Earth_m, Sun_m,  0.005);
//    Moon_m = rotPtHomo(Moon_m, Earth_m,  0.5);
    
    calcPoints(cube, Focal);

    //Display2
    // the last two lines should always be
    glutPostRedisplay ();
    glutTimerFunc (g_iTimerMSecs, timer, 0);    // call timer for next iteration
}


//void plotCircle(int x, int y, int px, int py, Color c){
//
//    glBegin (GL_POINTS);
//
//    glColor3f (c.r, c.g, c.b);
//
//    //Hate this part of the code
//    glVertex2i (x+px,y+py);
//    glVertex2i (y+px,x+py);
//    glVertex2i (-x+px,y+py);
//    glVertex2i (-y+px,x+py);
//    glVertex2i (x+px,-y+py);
//    glVertex2i (y+px,-x+py);
//    glVertex2i (-x+px,-y+py);
//    glVertex2i (-y+px,-x+py);
//
//    glEnd ();
//}

//void drawCube(int cube[8][4], float d, Color c){
//
//    Point newCube[8];
//
//    for (int i=0; i < 8; i++) {
//        newCube[i] = calculateCube(vector(cube[i][0], cube[i][1], cube[i][2], cube[i][3]), projectMat(d));
//    }
//
//    bhamLine(newCube[0].x, newCube[0].y, newCube[1].x, newCube[1].y, c);
//    bhamLine(newCube[1].x, newCube[1].y, newCube[2].x, newCube[2].y, c);
//    bhamLine(newCube[2].x, newCube[2].y, newCube[3].x, newCube[3].y, c);
//    bhamLine(newCube[3].x, newCube[3].y, newCube[0].x, newCube[0].y, c);
//
//    bhamLine(newCube[0].x, newCube[0].y, newCube[4].x, newCube[4].y, c);
//    bhamLine(newCube[1].x, newCube[1].y, newCube[5].x, newCube[5].y, c);
//    bhamLine(newCube[2].x, newCube[2].y, newCube[6].x, newCube[6].y, c);
//    bhamLine(newCube[3].x, newCube[3].y, newCube[7].x, newCube[7].y, c);
//
//    bhamLine(newCube[4].x, newCube[4].y, newCube[5].x, newCube[5].y, c);
//    bhamLine(newCube[5].x, newCube[5].y, newCube[6].x, newCube[6].y, c);
//    bhamLine(newCube[6].x, newCube[6].y, newCube[7].x, newCube[7].y, c);
//    bhamLine(newCube[7].x, newCube[7].y, newCube[4].x, newCube[4].y, c);
//
//}

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

//void bresenhamCircle(Point mp, int r, Color c){
//
//    int x, y, p, d, DSE, DE;
//
//    p = 0;
//    x = p;
//    y = r;
//    d = 5 - 4*r;
//
//    plotCircle(x, y, mp.x, mp.y, c);
//
//    while (y>x) {
//        if (d>=0)
//        {
//            DSE = 4*(2*(x-y)+5);
//            d += DSE;
//            x++;
//            y--;
//        } // SE
//        else {
//            DE = 4*(2*x+3);
//            d +=DE ;
//            x++;
//        } // E
//        plotCircle(x, y, mp.x, mp.y, c);
//    }
//}

//void drawingSolarSystem(Point Sun, Point Earth, Point Moon, Color color) {
//    bresenhamCircle(Sun, 25, cRed);
//    bresenhamCircle(Earth, 15, color);
//    bresenhamCircle(Moon, 7, color);
//}

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
        case '2':
            glutDisplayFunc (display2);
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
