// CameraView.cpp: implementation of the CCameraView class.
 
//////////////////////////////////////////////////////////////////////
  

#include "CameraView.h"
#include <iostream>
//#include <windows.h>
#include <vector>
#include <cmath>
#include <OpenGL/Gl.h>
#include <OpenGL/Glu.h>
#include <GLUT/glut.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCameraView::CCameraView(int w, int h)
{

	width = w;
	height = h;

	cam.init();

}


CCameraView::~CCameraView()
{
	delete shape;
}

void CCameraView::projection()
{

	// ... Choose the GL_PROJECTION matrix, set up perspective projection using 
	//     glFrustum and camera's view volume parameters are stored in the "Camera" class.
	//  or set up orthographic projection using glOrtho.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(cam.getLeft(), cam.getRight(), cam.getBottom(), cam.getTop(), cam.getNear(), cam.getFar());
}

void CCameraView::draw()  
{


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// viewing transformations
	cameraTransformation();

	glPushMatrix();
		// modeling transformations
		shapeTransformation();
		shape->draw();
	glPopMatrix();

}


void CCameraView::cameraTransformation()
{

	// ... Write OpenGL code to apply camera viewing transformations, 
	//     using translation and rotation parameters stored in the "Camera" class.
	glTranslated(cam.getTx(), cam.getTy(), cam.getTz());
    glRotated(cam.getXRoll(), 1, 0, 0); //x axis
    glRotated(cam.getYRoll(), 0, 1, 0); //y axis
    glRotated(cam.getZRoll(), 0, 0, 1); //z axis
	
}


void CCameraView::shapeTransformation()
{
	// ... Write OpenGL code to apply object/shape modeling transformations, 
	//     using object transformation parameters stored in the "Shape" class.
    glTranslated(shape->getTx(), shape->getTy(), shape->getTz());
    glScaled(shape->getScale(), shape->getScale(), shape->getScale());
    
    glRotated(shape->getRx(), 1, 0, 0); // x axis
    glRotated(shape->getRy(), 0, 1, 0); // y axis
    glRotated(shape->getRz(), 0, 0, 1); // z axis

}



