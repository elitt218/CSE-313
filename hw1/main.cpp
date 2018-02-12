//
//  main.cpp
//  hw1
//
//  Created by Elizabeth Litt on 9/12/17.
//  Copyright © 2017 Elizabeth Litt. All rights reserved.
//

/* Program description:
 *     Draw a five-leaf rose polar plot which varies with time. */

#include <iostream>
#include <stdlib.h>
#include <math.h>   // Needed for sin, cos, etc.
#include <OpenGL/OpenGL.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>

using namespace std;

int screenWidth = 480, screenHeight = 480;
float pi = 3.14159265;			// We need to define PI to do polar plots
float rad, x, y;
bool color;
float t = 0.0;

void updateColor(double &r, double &g, double &b ) {
    if(color) {
        r += 0.0117;
        g += 0.0018;
        b += 0.04;
    }
    else {
        int points = 180;
        double cStep = 1.0/points;  //color step
        
        r += cStep;
        g += cStep;
        b += cStep;
    }
}

void FiveLeafRose()
{
    float i;
    double r, g, b;
    r = 0.0;
    g = 0.0;
    b = 0.0;
    
    glBegin(GL_LINE_STRIP);
    for(i=t; i<(t+pi); i += pi/270){
        rad = cosf(5*i);
        x = rad * cosf(i);
        y = rad * sinf(i);
        
        glColor3f(r, g, b);
        glVertex2f(x, y);
        updateColor(r, g, b);

    }
    glEnd();
}

void display(void)
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    // Load the identity matrix to undo any rotations from the previous frame
    glLoadIdentity();
    
    // Call our custom drawing function
    FiveLeafRose();
    
    
    // Swap buffers to display the completed buffer (double-buffering)
    glutSwapBuffers();
}

void myKeys(unsigned char key, int x, int y)
{
    switch(key)
    {
            // Quit
        case 'q':	exit(0); // quit
            break;
        case 'c':
            color = true;
            cout << "color\n";
            break;
        case 'g':
            color = false;
            cout << "gray\n";
            break;
        default:	cout <<"No option for this key! Please try c, g, or q\n";
            
    }
    // Call the display function to reflect our changes
    display();
}

/* This is the idle function. */
void spinner(void) // the idle function
{
    t += 0.01;
    //.... Fill in code to update variables that affect what's displayed

    // Redraw to reflect the changes
    display();
}

// Do some basic OpenGL initialization to set up 2D graphics.
void myInit(int argc, char **argv)
{
    color = false;
    
    // Set background color to black
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    // Initialize the projection matrix with the identity matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Initialize the modelview matrix with the identity matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /* Set up a coordinate system so we don't have
     * to convert to screen coordinates manually */
    gluOrtho2D(-1.2, 1.2, -1.2, 1.2);
    // Set our OpenGL viewport to be the size of the glut window
    glViewport(0,0,screenWidth-1,screenHeight-1); //the default
}

// Our main function
int main(int argc, char **argv)
{
    // Initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    
    // Set some parameters for our OpenGL window before creating it
    glutInitWindowSize(screenWidth, screenHeight);
    glutInitWindowPosition(100, 100);
    
    // Create a window with a title
    glutCreateWindow("Polar Rose");
    
    // Register callbacks for glut
    glutKeyboardFunc(myKeys);
    glutDisplayFunc(display);
    glutIdleFunc(spinner);
    
    
    // Initialize our OpenGL window
    myInit(argc, argv);
    
    // Start the glut loop to begin drawing, etc.
    glutMainLoop();
    
    return 0;
}

