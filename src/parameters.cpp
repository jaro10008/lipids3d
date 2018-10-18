#include"parameters.h"
#include"utils.h"

#include<cctype>
#include<fstream>
#include<iostream>
#include<map>
#include<string>
#include<cstdlib>

using namespace std;

/*
	unsigned int initialTabSize;
	unsigned int steps;
	unsigned int N;
	long double r;
	long double agentSize;
	long double m;
	long double dt;
	long double sizeX, sizeY, sizeZ;
	long double friction;
	long double D;
	bool toroidal;
*/

#define LOAD_PARAMETER(xPar, checkF, castF) value = dict[#xPar];\
	if(checkF(value.c_str())){\
		(xPar) = castF(value.c_str());\
		printf(#xPar" = %s\n", value.c_str());\
	}\
	else{\
		fprintf(stderr, "Wrong value of the parameter '"#xPar"': %s\n", value.c_str());\
		return false;\
	}


#define SAVE_PARAMETER(xPar, flag) fprintf(file, #xPar " = %" flag "\n", xPar);

bool parameters::saveParameters(const char* fileName){
	FILE* file = fopen(fileName, "w");

	if(file == NULL)
		return false;

	fprintf(file, "# File generated automatically\n\n");

	SAVE_PARAMETER(initialTabSize, "u");
	SAVE_PARAMETER(N, "u");
	SAVE_PARAMETER(logTime, "u");
	SAVE_PARAMETER(r, "Lf");
	SAVE_PARAMETER(agentSize, "Lf");
	SAVE_PARAMETER(m, "Lf");
	SAVE_PARAMETER(dt, "Lf");
	SAVE_PARAMETER(sizeX, "Lf");
	SAVE_PARAMETER(sizeY, "Lf");
	SAVE_PARAMETER(sizeZ, "Lf");
	SAVE_PARAMETER(friction, "Lf");
	SAVE_PARAMETER(D, "Lf");
	
	char toroidal;
	if(this->toroidal)
		toroidal = 'T';
	else
		toroidal = 'F';
	SAVE_PARAMETER(toroidal, "c");

	fclose(file);
	return true;
		
}

static bool atob(const char* s){
	return s[0] == 'T';
}

bool parameters::browseDictionary(map<string, string> dict){
	string value;

	LOAD_PARAMETER(initialTabSize, isInteger, atoi);
	LOAD_PARAMETER(N, isInteger, atoi);
	LOAD_PARAMETER(logTime, isInteger, atoi);
	LOAD_PARAMETER(r, isDouble, atof);
	LOAD_PARAMETER(agentSize, isDouble, atof);
	LOAD_PARAMETER(m, isDouble, atof);
	LOAD_PARAMETER(dt, isDouble, atof);
	LOAD_PARAMETER(sizeX, isDouble, atof);
	LOAD_PARAMETER(sizeY, isDouble, atof);
	LOAD_PARAMETER(sizeZ, isDouble, atof);
	LOAD_PARAMETER(friction, isDouble, atof);
	LOAD_PARAMETER(D, isDouble, atof);
	LOAD_PARAMETER(toroidal, isBoolean, atob);

	return true;
}

bool parameters::readParametersFromFile(const char* fileName){
	ifstream file;

	file.open(fileName);

	if(!file)
        	return false;


	string line;
	map<string, string> dict;

	while(getline(file, line)){
		string key, value;

		size_t i = 0;

		for(;i < line.size(); ++i){
			if(line[i] == '='){
				++i;
				break;
			}
			if(line[i] == '#')
				break;
			if(isspace(line[i]))
				continue;
			key.push_back(line[i]);
		}

		for(;i < line.size(); ++i){
			if(line[i] == '#')
				break;
			if(isspace(line[i]))
				continue;
			value.push_back(line[i]);
		}

		if(!key.empty() && value.empty()){
			fprintf(stderr, "Wrong value of the parameter '%s': %s\n", key.c_str(), value.c_str());
			return false;
		}
		dict[key.c_str()] = value;
	}

	file.close();

	if(!browseDictionary(dict))
		return false;

	return true;
}
