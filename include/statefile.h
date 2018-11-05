#ifndef STATEFILE_H
#define STATEFILE_H

#include "parameters.h"
#include "lipid.h"

bool loadSimDataFromFile(const char*);
void saveSimDataToFile(const char*);
void extractParameters(const char*, const char*);
bool readRawInfo(const char*, parameters*, lipid**);

#endif //STATEFILE_H
