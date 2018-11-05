#ifndef VOLUME_H
#define VOLUME_H

#include <cmath>
#include "utils.h"
#include "lipid.h"
#include "parameters.h"

unsigned int getStepCount();
void setStepCount(unsigned int);
void incrementStepCount();
void initializeSimulation(bool initAgents = true);
lipid& getAgentAt(int);
int getSectorIndex(int, int, int);
void correctPosition(long double&, long double&, long double&);
lipid* getFirstInSector(int, int, int);
void setFirstInSector(int, int, int, lipid*);
void updateSectors(lipid*);

#endif //VOLUME_H
