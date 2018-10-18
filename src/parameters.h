#ifndef PARAMETERS_H
#define PARAMETERS_H

#include<map>
#include<string>

struct parameters{
	unsigned int initialTabSize;
	unsigned int N;
	unsigned int logTime;
	long double r;
	long double agentSize;
	long double m;
	long double dt;
	long double sizeX, sizeY, sizeZ;
	long double friction;
	long double D;
	bool toroidal;

	bool readParametersFromFile(const char*);
	bool saveParameters(const char*);
    private:
    bool browseDictionary(std::map<std::string, std::string>);
};

#endif
