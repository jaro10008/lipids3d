#ifndef READFILE_H
#define READFILE_H

#include <glm/vec3.hpp>

/**
 * Type representing a molecule in 3D space.
 */
struct Molecule {
    glm::vec3 position;
    glm::vec3 orientation;
};

/**
 * Type representing the data about molecules in space (simulation state).
 */
struct MoleculeData {
    Molecule* tab;
    unsigned int n;
    double agentSize;
};

/**
 * Sets the currently displayed molecule data.
 * @param nData New molecular data to set.
 */
void setMoleculeData(MoleculeData nData);

/**
 * Reads molecular data from a simulation state file.
 * @param fileName State file name.
 * @return Molecular data. If an error occured, tab property is set to NULL.
 */
MoleculeData readFile(const char* fileName);

/**
 * Draws molecules in 3D.
 */
void drawMolecules();

#endif //READFILE_H
