// TreePart.cpp: implementation of the CTreePart class.
//
//////////////////////////////////////////////////////////////////////

#include "TreePart.h"
 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// recursively construct a treepart of a particular depth, with specified branching factor, etc.
CTreePart::CTreePart(CRand rgen, int depth, int numBranch, double partLen, double partWid, double partTz, double partRx, double partRz)
{

	Tz = partTz;
	Rx = partRx;
	Rz = partRz;

	if (depth<=0)
	{
		this->leaf = true;
		this->length=partLen;
		this->width=partLen*0.5;
	}else{
		//branch
		this->leaf=false;
		this->length = (double) (rgen.randg()) * partLen/4 + partLen;
		this->width = (double) (rgen.randg()) * partWid/4 + partWid;
		
		int numParts = (depth==1) ? numBranch*2 : numBranch;
	
		for (int i=0; i< numParts; i++){
			double nextTz, nextRx, nextRz;
			
			if(i==0){
				// one branch is almost straight
				nextTz = this->length * 0.95f;
				nextRx = (double) (rgen.randu()*10);
				nextRz = (rgen.randg() * 360);
			}else{
				double lenSc;

				// Spread out along branch
				if (depth==1){
					// distribution of leaves on branch
					lenSc = rgen.randu() * 0.95f + 0.1f;
				}else{
					lenSc = (double) rgen.randg()*0.1f + 0.9f;
				}
				if(lenSc < 0.1f) lenSc = 0.1f;
				if(lenSc > 0.95f) lenSc = 0.95f;

				nextTz = this->length *lenSc;

				nextRz = (double) (rgen.randg() * 10.0f + (i*360.0f/(numParts-1)));
				
				nextRx = (double) rgen.randg() * 50.0/5 + 50.0;
			}

			CTreePart * newTreePart = new CTreePart(rgen,depth-1,numBranch,partLen * 0.65f, partWid * 0.6f, nextTz, nextRx, nextRz);

			this->parts.push_back(newTreePart);
		}
	}
}


CTreePart::~CTreePart()
{

	for (unsigned int i=0; i< this->parts.size(); i++){
		delete this->parts[i];
	}

}

// recursively draw a tree component
// first, place the component using transformation for this subtree
// second, draw leaf (if this is a leaf)
// third, draw subtree (if this is an interirior node): draw this node, then recursively all its children
void CTreePart::draw(int cylDisplayList, bool setColor)
{
	glPushMatrix();

	// place this component
	glTranslated(0, 0, Tz);
	glRotated(Rz, 0, 0, 1);
	glRotated(Rx, 1, 0, 0);

	if (leaf) {
		if (setColor)
			glColor3d(0.2, 1.0, 0.1);

		glNormal3d(0, 0, 1);

		// rectangular leaf
		glBegin(GL_POLYGON);
		glVertex3d(0, 0, 0);
		glVertex3d(width/2.0, 0, length/2.0);
		glVertex3d(0, 0, length);
		glVertex3d(-width/2.0, 0, length/2.0);
		glEnd();
	}else{
		if(setColor)
			glColor3d(0.6, 0.6, 0.1);

		glPushMatrix();
		glScaled(width, width, length);
		glCallList(cylDisplayList);
		glPopMatrix();

		// recursively draw the children
		for (unsigned int i=0; i< this->parts.size(); i++){
			this->parts[i]->draw(cylDisplayList, setColor);
		}
	}
	glPopMatrix();
}

