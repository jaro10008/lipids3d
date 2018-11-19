#ifndef VOLUME_H
#define VOLUME_H

#include <cmath>
#include "utils.h"
#include "lipid.h"
#include "parameters.h"

/**
 * Return the current count of steps.
 * @return Current step count.
 */
unsigned int getStepCount();

/**
 * Sets current step count.
 * @param stepCount New step count ot set.
 */
void setStepCount(unsigned int stepCount);

/**
 * Increments current step count.
 */
void incrementStepCount();

/**
 * Initializes global simulation variables.
 * @param initAgents Optional parameter. If true, agents' positions are randomly generated.
 */
void initializeSimulation(bool initAgents = true);

/**
 * Returns an agent with the index given.
 * @param in Index.
 * @return Agent with the given index.
 */
lipid& getAgentAt(int in);

/**
 * Returns the index of the sector with coordinates given.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param z Z coordinate.
 * @return Index of the sector.
 */
int getSectorIndex(int x, int y, int z);

/**
 * Corrects the position given considering area type.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param z Z coordinate.
 */
void correctPosition(long double& x, long double& y, long double& z);

/**
 * Returns the first agent on the sector list.
 * @param Sector X coordinate.
 * @param Sector Y coordinate.
 * @param Sector Z coordinate.
 * @return Pointer to the first lipid on the sector list.
 */
lipid* getFirstInSector(int x, int y, int z);

/**
 * Sets first lipid on the sector list.
 * @param x Sector X coordinate.
 * @param y Sector Y coordinate.
 * @param z Sector Z coordinate.
 * @param ptr Pointer to the lipid, can be null.
 */
void setFirstInSector(int x, int y, int z, lipid* ptr);

/**
 * Updates sectors' data. This function should be executed every time when lipid changes its position.
 * @param l Pointer to the lipid.
 */
void updateSectors(lipid* l);

/*
 * Returns the width of a sector.
 */
long double getSectorWidth();

#endif //VOLUME_H
