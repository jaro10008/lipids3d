#ifndef LIPID_H
#define LIPID_H

#include<cstddef>

#include"vector3.h"

/**
 * Type representing a single lipid.
 */
struct lipid {
    vector3 pos;
    vector3 v;
    vector3 F[3]; //0 - head, 1 - middle, 2 - tail

    vector3 direction;
    vector3 angularV;

    lipid *pre, *next;

    int x, y, z;

    lipid() {
        x = -1;
        y = -1;
        z = -1;
        pre = NULL;
        next = NULL;
    }

    /**
     * Gets sector X coordinate.
     * @return Sector X coordinate.
     */
    int getX();

    /**
     * Gets sector Y coordinate.
     * @return Sector Y coordinate.
     */
    int getY();

    /**
     * Gets sector Z coordinate.
     * @return Sector Z coordinate.
     */
    int getZ();

    /**
     * Updates lipid's velocity, position and rotation using forces computed earlier.
     */
    void update();
};

/**
 * Computes interaction forces between lipids. Updates forces only in l1.
 * @param l1 First lipid.
 * @param l2 Second lipid.
 */
void calculateForces(lipid* l1, lipid* l2);

/**
 * Computes the friction force for the lipid and updates its forces.
 * @param l Lipid.
 */
void friction(lipid* l);

/**
 * Performs diffusion.
 * @param l Lipid.
 */
void diffusion(lipid* l);


#endif
