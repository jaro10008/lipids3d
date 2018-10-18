#include"parameters.h"
#include"vector3.h"
#include"lipid.h"

extern parameters pars;

void lipid::update(){
    vector3 a = (FH + FM + FT) / (3 * pars.m);
	v += a * pars.dt;
	pos += v * pars.dt;

	vector3 M = direction.crossProduct(FH - FT);
	vector3 angularA = M / (2 * pars.m * pars.agentSize * pars.agentSize);

	angularV += angularA * pars.dt;
	vector3 mov = angularV.crossProduct(direction);

	if(mov.length2() > 0.0){
        mov.normalize();

        direction += mov * tan(angularV.length() * pars.dt);
        direction.normalize();
	}
}

int lipid::getX(){
	return int(pos.x / pars.r);
}
int lipid::getY(){
	return int(pos.y / pars.r);
}
int lipid::getZ(){
	return int(pos.z / pars.r);
}

void calculateForces(lipid* l1, lipid* l2){
}

static vector3 frictionForce(vector3 v){
	return  - pars.friction * v * v.length();
}

void friction(lipid* l){
	return;
	l->FM += frictionForce(l->v);
	vector3 vAngle = l->angularV.crossProduct(l->direction) * pars.agentSize;
	vector3 vH = l->v + vAngle;
	l->FH += frictionForce(vH);
	vector3 vT = l->v - vAngle;
	l->FT += frictionForce(vT);
}
void diffusion(lipid* l){
	//l->FM += vector3(10.0, 0.0, 0.0);
	l->FH += vector3(10000.0,0.0,0.0);
}
