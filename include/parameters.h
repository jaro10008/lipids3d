#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<map>
#include<string>

/**
 * Type representing simulation parameters.
 */
struct parameters {
    unsigned int N;
    unsigned int logTime;
    long double r;
    long double agentSize;
    long double m;
    long double dt;
    long double sizeX, sizeY, sizeZ;
    long double friction;
    long double D;
    long double epsilonHH;
    long double rmHH;
    long double epsilonHT;
    long double rmHT;
    long double epsilonTT;
    long double rmTT;
    bool toroidal;

    /**
     * Reads parameters from file.
     * @param fileName File name.
     * @return True if no errors occured, false otherwise.
     */
    bool readParametersFromFile(const char* fileName);
    
    /**
     * Saves parameters to file.
     * @param fileName File name.
     * @return True if no errors occured, false otherwise.
     */
    bool saveParameters(const char* fileName);
private:
    /**
     * Loads parameters from the dictionary.
     * @param dict Dictionary.
     * @return True if no errors occured, false otherwise.
     */
    bool browseDictionary(std::map<std::string, std::string> dict);
};

/**
 * Gets the current parameter class.
 * @return Current parameter class.
 */
parameters& getParameters();

#endif
