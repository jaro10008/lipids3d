#include "drawing.h"
#include "lipid.h"
#include "statefile.h"
#include "parameters.h"

#include <glm/vec3.hpp>
#include <cstdio>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

using namespace glm;

static Molecule* tab;
static unsigned int n;
static double agentSize;
static GLUquadric* quadratic;

void setMoleculeData(MoleculeData nData) {
    tab = nData.tab;
    n = nData.n;
    agentSize = nData.agentSize;
}

MoleculeData readFile(const char* fileName) {
    parameters pars;
    lipid* temp = NULL;

    MoleculeData nData;
    nData.tab = NULL;

    if (!readRawInfo(fileName, &pars, &temp)) {
        return nData;
    }

    nData.n = pars.N;
    nData.agentSize = pars.agentSize;

    nData.tab = new Molecule[nData.n];

    for (unsigned int i = 0; i < nData.n; ++i) {
        nData.tab[i].position.x = (float) temp[i].pos.x;
        nData.tab[i].position.y = (float) temp[i].pos.y;
        nData.tab[i].position.z = (float) temp[i].pos.z;

        nData.tab[i].orientation.x = (float) temp[i].direction.x;
        nData.tab[i].orientation.y = (float) temp[i].direction.y;
        nData.tab[i].orientation.z = (float) temp[i].direction.z;
    }

    return nData;
}

/**
 * Draws a sphere.
 * @param position Center of the sphere.
 * @param radius Radius of the sphere.
 */
static void drawSphere(vec3 position, float radius) {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere((GLdouble) radius, 25, 25);
    glPopMatrix();
}

/**
 * Draws a cylinder.
 * @param tail Center of the first base.
 * @param head Center of the second base.
 * @param radius Radius of the bases.
 */
static void drawCylinder(vec3 tail, vec3 head, float radius) {
    vec3 diff = head - tail;
    float l = length(diff);
    diff = normalize(diff);
    glPushMatrix();
    glTranslatef(tail.x, tail.y, tail.z);

    float angle = 180.0 * acos(diff.z) / M_PI;


    if (angle > 0.0f) {
        vec3 rotVec(-diff.y, diff.x, 0.0f);
        rotVec = normalize(rotVec);
        glRotatef(angle, rotVec.x, rotVec.y, 0.0f);
    }
    gluCylinder(quadratic, radius, radius, l, 25, 1);
    glPopMatrix();
}

static const GLfloat red[] = {1.0f, 0.0f, 0.0f, 1.0f};
static const GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};

static void drawMolecule(Molecule& m) {
    vec3 diff = m.orientation * (float) agentSize;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    drawSphere(m.position + diff, 0.4f * (float) agentSize);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    drawCylinder(m.position - diff, m.position + diff, 0.2f * (float) agentSize);
    drawSphere(m.position - diff, 0.2f * (float) agentSize);

}

void drawMolecules() {
    quadratic = gluNewQuadric();
    for (unsigned int i = 0; i < n; ++i)
        drawMolecule(tab[i]);
    gluDeleteQuadric(quadratic);
}
