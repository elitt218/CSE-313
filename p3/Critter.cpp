// Critter.cpp: implementation of the CCritter class.
//
//////////////////////////////////////////////////////////////////////

#include "Critter.h"
#include "Bug.h"

////////////////////////////////////////////////////////////////////// 
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCritter::CCritter()
{

	dist = 0;
	lastWander =0;

}

CCritter::~CCritter()
{

}

// method to integrate acc to get updated vel and pos.
// also, update the total distance traveled
// this function assumes acc has already been computed
void CCritter::integrate(double dt){

	// ... add in code here	
	// update velocity
    Vector3d dtacc = acc;
    dtacc.scale(dt);
    
    vel.set(dtacc.x + vel.x, dtacc.y + vel.y, dtacc.z + vel.z);
    
	// update position 
    Vector3d dtvel = vel;
    dtvel.scale(dt);
    
    Point3d lastpos = pos;
    pos.set(pos.x + dtvel.x, pos.y + dtvel.y, pos.z + dtvel.z);
    
    // add to total distance traveled
    dist += pos.distance(lastpos);
}

// add in viscous drag (assume mass of 1): a+=-k*v (k>0)
void CCritter::accelDrag(double k){
	
	
	// ... add in code here
	// update acceleration due to viscous drag
    acc.set(acc.x - k*vel.x, acc.y - k*vel.y, acc.z - k*vel.z);
}

// add in attraction acceleration: a += direction*(k*dist^exp)
//(direction is the unit vector from this critter to goal location p.)
// (positive values of k produce attraction, negative values of k produce repulsion.)
// (dist is the distance between this critter and point p.)
// (exp is the distance fall-out factor.  If you want to add an attraction force that is 
// stronger when the critter is farther away from a goal location such as the main bug or 
// the center of the scene, use positive exp value and positive k value.  
// If you want to add a repulsion force that is weaker when the critter is farther 
// away from a goal location such as an obstacle or another critter,  use negative exp value and negative k value.)
void CCritter::accelAttract(Point3d p, double k, double exp){

    //slide 17 in lecture 11
    //k>1 repulsion
    //k<0 attraction
	// ... add in code here
    double dist = pos.distance(p);
    
    Vector3d direction = Vector3d(pos.x-p.x, pos.y-p.y, pos.z-p.z);
    
    direction.normalize();
    
    Vector3d attraction = direction;
    
    attraction.scale(k*pow(dist,exp));
    
    acc.set(acc.x+attraction.x, acc.y+attraction.y, acc.z+attraction.z);

}
 


// add in acceleration towards a temporary goal, which changes regularly
// this is to model the wandering motion of the lead bug
void CCritter::accelWander(double k, double t, double often){

	if (t > lastWander + often) {	
		lastWander =t;

		// generate new goal point near middle, not too close to current bug position
		do{
			pWander.x = (rgen.randu()-0.5)*10.0;
			pWander.y = (rgen.randu()-0.5)*10.0;
			pWander.z = 0;
		}while (pWander.distance(getLocation()) < 4.0);
	}
	accelAttract(pWander, k, 0.8);
}

