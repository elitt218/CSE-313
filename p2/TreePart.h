// TreePart.h: interface for the CTreePart class.
//
//////////////////////////////////////////////////////////////////////
 
#pragma once

#include "stdafx.h"
#include "Rand.h"
#include <vector>

class CTreePart  
{
public:
	CTreePart(CRand rgen, int depth, int numBranch, double partLen, double partWid,  double partTz, double partRx, double partRz);
	virtual ~CTreePart();

public:
     const double minX = 20.0;
     const double maxX = 85.0;
     const int zDif = 50; //max dif for z rotation
	 
	// transformation for this branch/leaf
	double Tz, Rx, Rz;

	// leaf or trunk
	GLboolean leaf;

	// size of part
	double length, width;

	// children
	std::vector<CTreePart *> parts;

	void draw(int cylDisplayList, bool setColor);

};
