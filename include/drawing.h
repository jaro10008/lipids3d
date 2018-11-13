#ifndef READFILE_H
#define READFILE_H

#include <glm/vec3.hpp>

struct Molecule {
	glm::vec3 position;
	glm::vec3 orientation;
};

struct MoleculeData{
    Molecule* tab;
    unsigned int n;
    double agentSize;
};

void setMoleculeData(MoleculeData);
MoleculeData readFile(const char*);
void drawMolecules();

#endif //READFILE_H
