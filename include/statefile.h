#ifndef STATEFILE_H
#define STATEFILE_H

#include "parameters.h"
#include "lipid.h"

/**
 * Loads simulation data from state file.
 * @param fileName State file name.
 * @return True if no errors occured, false otherwise.
 */
bool loadSimDataFromFile(const char* fileName);

/**
 * Creates a state file with the current simulation data.
 * @param fileName State file name.
 * @return True if no errors occured, false otherwise.
 */
void saveSimDataToFile(const char* fileName);

/**
 * Extracts parameters from the state file and creates a parameter file.
 * @param inFile State file name.
 * @param parFile Parameter file name.
 */
void extractParameters(const char* inFile, const char* parFile);

/**
 * Reads simulation data from file, but doesn't update any global variables.
 * @param inFile Input file name.
 * @param pars Pointer to a parameter class to which parameters should be saved.
 * @param tab Pointer to a pointer. After the execution finishes, this pointer will be pointing to the tab of lipids read.
 * @return True if no errors occured, false otherwise.
 */
bool readRawInfo(const char* inFile, parameters* pars, lipid** tab);

#endif //STATEFILE_H
