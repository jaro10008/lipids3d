#include"parameters.h"
#include"vector3.h"
#include"lipid.h"
#include"utils.h"
#include"volume.h"

#include<cmath>

void lipid::update() {
    parameters& pars = getParameters();
    vector3 a = (F[0] + F[1] + F[2]) / (3 * pars.m);
    v += a * pars.dt;
    pos += v * pars.dt;

    vector3 M = direction.crossProduct(F[0] - F[2]);
    vector3 angularA = M / (2 * pars.m * pars.agentSize * pars.agentSize);

    angularV += angularA * pars.dt;
    vector3 mov = angularV.crossProduct(direction);

    if (mov.length2() > 0.0) {
        mov.normalize();

        direction += mov * tan(angularV.length() * pars.dt);
        direction.normalize();
    }
}

int lipid::getX() {
    return int(pos.x / getSectorWidth());
}

int lipid::getY() {
    return int(pos.y / getSectorWidth());
}

int lipid::getZ() {
    return int(pos.z / getSectorWidth());
}

/**
 * Computes force according to LJ potential function.
 * @param distance 3D vector - difference between points' positions.
 * @param epsilon Epsilon parameter.
 * @param rm "rm" parameter.
 * @return 3D vector representing resulting points.
 */
static vector3 LJForce(vector3 distance, long double epsilon, long double rm) {
    long double r = distance.length();
    rm = rm / r;

    long double rm2 = rm * rm;
    long double rm6 = rm2 * rm2 * rm2;
    long double rm12 = rm6 * rm6;



    return 12 * epsilon * (rm12 - rm6) * distance / (r * r);
}

void calculateForces(lipid* l1, lipid* l2) {
    parameters& pars = getParameters();

    vector3 tab1[3];
    tab1[0] = l1->pos + l1->direction * pars.agentSize;
    tab1[1] = l1->pos;
    tab1[2] = l1->pos - l1->direction * pars.agentSize;

    vector3 tab2[3];
    tab2[0] = l2->pos + l2->direction * pars.agentSize;
    tab2[1] = l2->pos;
    tab2[2] = l2->pos - l2->direction * pars.agentSize;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            vector3 distance = tab1[i] - tab2[j];
            if (distance.length() > pars.r)
                continue;
            long double epsilon, rm;
            if (!i && !j) {
                epsilon = pars.epsilonHH;
                rm = pars.rmHH;
            } else if (!i || !j) {
                epsilon = pars.epsilonHT;
                rm = pars.rmHT;
            } else {
                epsilon = pars.epsilonTT;
                rm = pars.rmTT;
            }

            vector3 force = LJForce(distance, epsilon, rm);
            l1->F[i] += force;
            //l2->F[j] -= force;
        }
    }
}

/**
 * Computes the friction force.
 * @param v Velocity of the object.
 * @return 3D vector representing force.
 */
static vector3 frictionForce(vector3 v) {
    parameters& pars = getParameters();
    return -pars.friction * v * v.length();
}

void friction(lipid* l) {
    parameters& pars = getParameters();
    l->F[1] += frictionForce(l->v);
    vector3 vAngle = l->angularV.crossProduct(l->direction) * pars.agentSize;
    vector3 vH = l->v + vAngle;
    l->F[0] += frictionForce(vH);
    vector3 vT = l->v - vAngle;
    l->F[2] += frictionForce(vT);
}

void diffusion(lipid* l) {
    parameters& pars = getParameters();
    static long double dCoeff = sqrt(24 * pars.D * pars.dt);
    vector3 diff = dCoeff * randomDouble() * randomUnitVector();
    l->pos += diff;
}
