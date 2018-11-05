#ifndef LIPID_H
#define LIPID_H

#include"vector3.h"

struct lipid{
	vector3 pos;
	vector3 v;
	vector3 FH, FM, FT;

	vector3 direction;
	vector3 angularV;

	lipid *pre, *next;

	int x, y, z;

	lipid(){
		x = -1;
		y = -1;
		z = -1;
		pre = NULL;
		next = NULL;
	}

	int getX();
	int getY();
	int getZ();

	void update();
	void updateSectors();
};


void calculateForces(lipid*, lipid*);
void friction(lipid*);
void diffusion(lipid*);


#endif
