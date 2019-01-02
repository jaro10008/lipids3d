#include <cmath>
#include "utils.h"
#include "lipid.h"
#include "parameters.h"
#include "vector3.h"

static lipid* agents;
static lipid** sectors;
static int sectorsX;
static int sectorsY;
static int sectorsZ;
static unsigned int steps;
static long double width;

unsigned int getStepCount() {
    return steps;
}

void setStepCount(unsigned int stepCount) {
    steps = stepCount;
}

void incrementStepCount() {
    ++steps;
}

static int correctIndex(int in, int maxIn){
	bool neg = false;

	if(in < 0){
		neg = true;
		in = - in;
	}

	in %= maxIn;

	if(neg && in){
		in = maxIn - in;	
	}

	return in;
}

int getSectorIndex(int x, int y, int z) {
    parameters& pars = getParameters();
    if (pars.toroidal) {
        x = correctIndex(x, sectorsX);
	y = correctIndex(y, sectorsY);
	z = correctIndex(z, sectorsZ);
    } else {
        if (x < 0 || x >= sectorsX)
            return -1;
        if (y < 0 || y >= sectorsY)
            return -1;
        if (z < 0 || z >= sectorsZ)
            return -1;
    }

    return z * sectorsY * sectorsX + y * sectorsX + x;
}

/**
 * Corrects the coordinate in 1D space, when the function finishes,
 * x is in the range (0,sizeX).
 * During execution the simulation area type is considered.
 * @param x Position of the point.
 * @param sizeX Length of the space.
 */
static void correctCoordinate(long double& x, long double sizeX) {
    parameters& pars = getParameters();
    if (pars.toroidal) {
        if (x < 0) {
            x = fmod(x, sizeX) + sizeX;
        } else if (x >= sizeX)
            x = fmod(x, sizeX);
    } else {
        if (x < 0) {
            x = fmod(x, 2 * sizeX);
            if (x > -sizeX)
                x = -x;
            else
                x += 2 * sizeX;
        } else if (x >= sizeX) {
            x = fmod(x, 2 * sizeX);
            if (x >= sizeX)
                x = 2 * sizeX - x;
        }

    }
}

void correctPosition(long double& x, long double& y, long double& z) {
    parameters& pars = getParameters();
    correctCoordinate(x, pars.sizeX);
    correctCoordinate(y, pars.sizeY);
    correctCoordinate(z, pars.sizeZ);
}

lipid* getFirstInSector(int x, int y, int z) {

    int in = getSectorIndex(x, y, z);

    if (in == -1)
        return NULL;

    return sectors[in];
}

void setFirstInSector(int x, int y, int z, lipid* ptr) {

    int in = getSectorIndex(x, y, z);

    if (in != -1)
        sectors[in] = ptr;
    ;
}

void updateSectors(lipid* l) {
    correctPosition(l->pos.x, l->pos.y, l->pos.z);

    if (l->x == l->getX() && l->y == l->getY() && l->z == l->getZ())
        return;
    if (l->pre != NULL)
        l -> pre -> next = l -> next;
    else if (l->x >= 0)
        setFirstInSector(l->x, l->y, l->z, l->next);

    if (l->next != NULL)
        l -> next -> pre = l -> pre;

    l->pre = NULL;
    l->x = l->getX();
    l->y = l->getY();
    l->z = l->getZ();

    lipid* first = getFirstInSector(l->x, l->y, l->z);

    if (first != NULL)
        first -> pre = l;
    l->next = first;
    setFirstInSector(l->x, l->y, l->z, l);
}

void initializeSimulation(bool initAgents = true) {
    parameters& pars = getParameters();

    width = pars.r + pars.agentSize;

    sectorsX = int(pars.sizeX / width) + 1;
    sectorsY = int(pars.sizeY / width) + 1;
    sectorsZ = int(pars.sizeZ / width) + 1;

    pars.sizeX = sectorsX * width;
    pars.sizeY = sectorsY * width;
    pars.sizeZ = sectorsZ * width;

    agents = new lipid[pars.N];
    sectors = new lipid*[sectorsX * sectorsY * sectorsZ];

    for (int i = 0; i < sectorsX * sectorsY * sectorsZ; ++i) {
        sectors[i] = NULL;
    }

    if (initAgents) {

        for (unsigned int i = 0; i < pars.N; ++i) {
            agents[i].pos.x = randomDoubleWithout1() * pars.sizeX;
            agents[i].pos.y = randomDoubleWithout1() * pars.sizeY;
            agents[i].pos.z = randomDoubleWithout1() * pars.sizeZ;

            agents[i].v = vector3(0.0, 0.0, 0.0);
            agents[i].angularV = vector3(0.0, 0.0, 0.0);

            agents[i].direction = randomUnitVector();

            agents[i].pre = NULL;
            agents[i].next = NULL;
            agents[i].x = -1;

            updateSectors(agents + i);
        }
    }
}

lipid& getAgentAt(int in) {
    return agents[in];
}

long double getSectorWidth() {
    return width;
}



