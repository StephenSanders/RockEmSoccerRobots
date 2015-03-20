#pragma once
/*
XAnimatorTypes.h
Holds types for extended info functions
Written by Keith Ditchburn, 2010 - 2013
*/

struct TXAnimatorMesh
{
	char *meshName;
	int numMaterials;
	char **textureFilenames;	// array of size numMaterials. Note: any can be NULL
	D3DMATERIAL9* materials;

	TXAnimatorMesh *nextMesh;
	TXAnimatorMesh *prevMesh; // for ease

	TXAnimatorMesh() : meshName(0),numMaterials(0),textureFilenames(0),materials(0),nextMesh(0),prevMesh(0){}
	~TXAnimatorMesh()
	{
		delete[] meshName;
		for (int i=0;i<numMaterials;i++)
		{
			delete[] textureFilenames[0];
		}
		delete[] textureFilenames;
		delete[] materials;
	}
};

struct TXAnimatorFrame
{	
	char *frameName;
	//D3DXMATRIX	TransformationMatrix;

	TXAnimatorMesh *mesh;

	TXAnimatorFrame *nextSiblingFrame;
	TXAnimatorFrame *childFrame;

	TXAnimatorFrame *parentFrame; // just for ease
	TXAnimatorFrame *prevSiblingFrame; // just for ease

	TXAnimatorFrame() : frameName(0), mesh(0), nextSiblingFrame(0),prevSiblingFrame(0),childFrame(0),parentFrame(0)  {}
	~TXAnimatorFrame()
	{
		delete[] frameName;
		delete mesh;
		delete nextSiblingFrame;
		delete childFrame;
	}
};

