// Scene.cpp: implementation of the CScene class.
//
//////////////////////////////////////////////////////////////////////

#include "Scene.h"
 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScene::CScene()
{

	// initialize scene transformation
	tx = 0;
	ty = 0.0;
	tz = 0;
	rx = 0.0;
	ry = 0.0;
	rz = 0.0;
	scale = 1.0;


	// initialize clock
	clock = 0;

	// initialize frameNumber
	frameNumber = 0;


	for (int i=0; i<numObstacles; i++){
		pObstacles[i] = NULL;
	}

	build();

}

CScene::~CScene()
{

	for (int i=1; i<numObstacles; i++){
		delete pObstacles[i];
	}
}

void CScene::build()
{

	// create a tree as Obstacle, need to change "numObstacles" in "Scene.h" if adding more obstacles
	for (int i=0; i<numObstacles; i++){
        
        pObstacles[i]=  new CTree(rgen, 5, 4, 4.5f, 0.6f, 0, (rgen.randu()-0.5)*2.0+3.0);
//        pObstacles[i]=  new CTree(rgen, 6, 4, 4.5f, 0.6f, 0, (rgen.randu()-0.5)*2.0+3.0);
//		pObstacles[i]=  new CTree(rgen, 1, 4, 4.5f, 0.6f, 0, (rgen.randu()-0.5)*2.0+3.0);
        
		
	}


	// ... you can add in code here to create more obstacles	
	
	// reset clock
	resetClock();

}

Point3d CScene::freeSpotUniform(float minDist, float rad)
{
	Point3d p;
	do{
		p.set(2*rgen.randu()*rad - rad, 2*rgen.randu()*rad - rad, 0);
	}while (!isFreeSpot(p, minDist));
	return p;
}

Point3d CScene::freeSpotGaussian(float minDist, float sigma)
{
	Point3d p;
	do{
		p.set((rgen.randg())*sigma, (rgen.randg())*sigma, 0);
	}while (!isFreeSpot(p, minDist));
	return p;
}

bool CScene::isFreeSpot(Point3d p, float minDist)
{
	// check obstacles if any
	for (int i=0; i<numObstacles; i++){
		if (p.distance((pObstacles[i])->getLocation())<minDist)
			return false;
	}
	


	return true;
}

void CScene::sceneTransformation()
{
	
	glTranslated(tx, ty, tz);
	glRotated(rz, 0, 0, 1);
	glRotated(ry, 0, 1, 0);
	glRotated(rx, 1, 0, 0);
	glScaled(scale, scale, scale);
}



void CScene::draw()
{

	int i;
	double ground[] = {0.0, 0.0, 1.0, 0.001};

	// draw ground plane (a square at Z=0)
	glColor3d(0.4, 0.8, 0.35);
	glBegin(GL_POLYGON);
	glNormal3d(0, 0, 1);
	glVertex3d(15, -15, 0);
	glVertex3d(15, 15, 0);
	glVertex3d(-15, 15, 0);
	glVertex3d(-15, -15, 0);
	glEnd();


	
	// enable clipping plane before drawing obstacles
	glClipPlane(GL_CLIP_PLANE0, ground);
	glEnable(GL_CLIP_PLANE0);

	// draw obstacles
	for (i=0; i<numObstacles; i++){
		pObstacles[i]->draw(true);
	}


	
}

void CScene::process(double elapsed)
{
	// elapsed: time elapsed since last frame (last display)

	// update clock
	clock += elapsed;
	frameNumber++;

	double dtMax = 1.0/50.0f;  // maximum time needed to make one step forward
	
	// number of integration steps
	int steps = (int) (elapsed/dtMax);

	if(steps<1) steps =1;


	for (int st=0; st <steps; st++){
		double dt = elapsed/((double) steps);		// time spent per step
		
		}

}

void CScene::shadowProject()
{
	
	// shadow matrix for drop shadow onto z=0 plane
	float *m = new float[4*4];

	float dx = light_position[0];
	float dy = light_position[1];
	float dz = light_position[2];

	float a=0, b=0, c=1, d = -0.01f;

	m[0*4+0]= b*dy + c*dz;
	m[1*4+0]= -b*dx;
	m[2*4+0]= -c*dx;
	m[3*4+0]= -d*dx;

	m[0*4+1]= -a*dy;
	m[1*4+1]= a*dx + c*dz;
	m[2*4+1]= -c*dy;
	m[3*4+1]= -d*dy;

	m[0*4+2]= -a*dz;
	m[1*4+2]= -b*dz;
	m[2*4+2]= a*dx+b*dy;
	m[3*4+2]= -d*dz;

	m[0*4+3]=  0;
	m[1*4+3]=  0;
	m[2*4+3]=  0;
	m[3*4+3]=  a*dx + b*dy + c*dz;

	glMultMatrixf((const float *) m);
	
	delete [] m;
}
