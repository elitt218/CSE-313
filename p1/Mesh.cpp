#include "Mesh.h"
 
//#include <windows.h>
#include <OpenGL/Gl.h>
#include <OpenGL/Glu.h>
#include <GLUT/glut.h>
#include <iostream>
#include <fstream>
#include <string>
  
using namespace std;


Mesh::Mesh()
{
	shape = new Shape();

}


Mesh::Mesh(Shape *shape2)
{
	shape = shape2;
}



Mesh::~Mesh()
{

}



// Make the mesh: vertex list, face list, normal list.
void Mesh::makeSurfaceMesh()
{

	// ...  Extra Credits:
	// instead of reading an object file, make a tesselated mesh surface (e.g. cylinder, cone) 
	// by yourself using tesselation here.

	// the following line just initializes the window to display a cube.
	readfile("cube.obj");

}



void Mesh::draw() // use openGL to draw this mesh
{

	for(unsigned int f = 0;f <face.size();f++) // for each face
	{
		glColor3f(1,1,1);
		if(!shape->getWire())  // draw in wireframe or solid mode?
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glBegin(GL_POLYGON);
		}
		else
		{
			glDisable(GL_LIGHTING);
			glBegin(GL_LINE_LOOP);
		}
		for(int v=0;v<face[f].nVerts;v++) // for each vertex in this face
		{	
			int in = face[f].vert[v].normIndex; // index of this normal
			int iv = face[f].vert[v].vertIndex; // index of this vertex
			if(shape->getSmooth())
				glNormal3f(norm[in].norm.x, norm[in].norm.y, norm[in].norm.z);
			else
				glNormal3f(face[f].faceNorm.norm.x, face[f].faceNorm.norm.y, face[f].faceNorm.norm.z);

			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		
		}
		glEnd();

	}


	if(shape->getNormals())  // drawing normal is enabled
	{
        glDisable(GL_LIGHTING);
        glColor3f(1, 0, 0);
        double NORM_SCALE = 0.1;
        glBegin(GL_LINES);
        //smooth shading: draw line from vertex
        if(shape->getSmooth()) {
            for(auto norms = norm.begin(), end = norm.end(); norms != end; norms++) {
                glVertex3d(norms->pt.x, norms->pt.y, norms->pt.z);
                glVertex3d(norms->pt.x + norms->norm.x * NORM_SCALE, norms->pt.y + norms->norm.y * NORM_SCALE, norms->pt.z + norms->norm.z * NORM_SCALE);
            }
        }
        //flat shading: draw line from face
        else {
            for(auto faces = face.begin(), end = face.end(); faces != end; faces++) {
                glVertex3d(faces->faceNorm.pt.x, faces->faceNorm.pt.y, faces->faceNorm.pt.z);
                glVertex3d(faces->faceNorm.pt.x + faces->faceNorm.norm.x * NORM_SCALE, faces->faceNorm.pt.y + faces->faceNorm.norm.y * NORM_SCALE, faces->faceNorm.pt.z + faces->faceNorm.norm.z * NORM_SCALE);
            }
        }
        glEnd();
		// .... If flat shading, draw each face normal using a short line originating from center of the face
		// .... If smooth shading, draw each vertex normal using a short line originating from the vertex

	}
	

}


// Calculate Newell Face Normal 
void Mesh::calcNewell()
{
    for(auto faces = face.begin(), end = face.end(); faces != end; faces++) {
        Normal norms;
        for(int points=0, length=faces->nVerts; points < length; points++) {
            
            Vector3d vcurr, vnext;
            
            vcurr = pt[faces->vert[points].vertIndex];
            vnext = pt[faces->vert[(points+1)%length].vertIndex];
            
            norms.norm.x += ((vcurr.y - vnext.y) * (vcurr.z + vnext.z));
            norms.norm.y += ((vcurr.z - vnext.z) * (vcurr.x + vnext.x));
            norms.norm.z += ((vcurr.x - vnext.x) * (vcurr.y + vnext.y));
            
            norms.pt.x += vcurr.x/length;
            norms.pt.y += vcurr.y/length;
            norms.pt.z += vcurr.z/length;
        }
        norms.norm.normalize();
        faces->faceNorm = norms;
    }
}


//Calculate Normal at Vertices
void Mesh::calcGourand()
{
	// ... Calculate vertex normals for the purpose of smooth shading
    for(int points = 0, length = pt.size(); points < length; points++) {
        int numFaces = 0;
        Normal norms;
        norms.pt = pt[points];
        for(auto faces = face.begin(), end = face.end(); faces != end; faces++) {
            for(int iter = 0, length = faces->nVerts; iter < length; iter++) {
                if(faces->vert[iter].vertIndex == points) {
                    norms.norm.add(faces->faceNorm.norm);
                    numFaces++;
                }
            }
        }
        norms.norm.set(norms.norm.x/numFaces, norms.norm.y/numFaces, norms.norm.z/numFaces);
        norms.norm.normalize();
        norm[points] = norms;
    }

}


void Mesh::readfile(string filename)
{
	// read a mesh from a Wavefront OBJ object model file.
	// fill in the content of the three lists

	ifstream in(filename.c_str());
	string s;
	double x,y,z;
	int vindex;

	Vector3d v;
	Face f;
	VertexID vid;

	norm.resize(0);
	face.resize(0);
	pt.resize(0);

	in >> s;
	while(1)
	{
		if(s=="v")
		{
			in >> x >> y >> z;
			v.set(x,y,z);
			pt.push_back(v);
			if(!(in >> s))
				break;

		}
		else if(s=="f")
		{

			while((in >> s) && s[0]>='0' && s[0]<='9')
			{
	
				vindex = atoi(s.c_str());
	
				vid.vertIndex = vid.normIndex = vindex-1;
				f.vert.push_back(vid);
				f.nVerts=f.vert.size();
			}
			face.push_back(f);
			f.vert.resize(0);
		}
		else
		{
			getline(in,s);
			if(!(in>>s))
				break;
		}
	} 

	norm.resize(pt.size());

	numVerts = pt.size();
	numFaces = face.size();
	numNorms = norm.size();

	// call the functions to calculate face normals and vertex normals
	calcNewell();
	calcGourand();


	in.close();
}

// Extra credits: write a function for output an OBJ model file. For instance, you can make a surface 
// mesh using tesselation and then output the surface in an OBJ file.
