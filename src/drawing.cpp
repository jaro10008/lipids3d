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

struct Molecule {
	vec3 position;
	vec3 orientation;
};

static unsigned int n;
static Molecule* tab = NULL;
static double agentSize;
static GLUquadric* quadratic;

bool readFile(const char* fileName) {
	parameters pars;
    lipid* temp = NULL;    

    if(!readRawInfo(fileName, &pars, &temp))
    {
        return false;
    }

    n = pars.N;
    agentSize = pars.agentSize;

    if(tab == NULL)
        tab = new Molecule[n];

    for(unsigned int i = 0; i < n; ++i){
        tab[i].position.x = (float) temp[i].pos.x;
        tab[i].position.y = (float) temp[i].pos.y;
        tab[i].position.z = (float) temp[i].pos.z;

        tab[i].orientation.x = (float) temp[i].direction.x;
        tab[i].orientation.y = (float) temp[i].direction.y;
        tab[i].orientation.z = (float) temp[i].direction.z;
    }

	return true;
}


static void drawSphere(vec3 position, float radius){
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glutSolidSphere((GLdouble) radius, 25, 25);
    glPopMatrix();
}

static void drawCylinder(vec3 tail, vec3 head, float radius){
    vec3 diff = head - tail;
    float l = length(diff);
    diff = normalize(diff);
    glPushMatrix();
    glTranslatef(tail.x, tail.y, tail.z);

    float angle = 180.0 * acos(diff.z) / M_PI;


    if(angle > 0.0f){
        vec3 rotVec(-diff.y, diff.x, 0.0f);
        rotVec = normalize(rotVec);
        glRotatef(angle,rotVec.x, rotVec.y, 0.0f);
    }
    gluCylinder(quadratic, radius,radius,l,25,1);
    glPopMatrix();
}

static const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f };
static const GLfloat white[] = { 1.0f, 1.0f, 1.0f, 1.0f };

static void drawMolecule(Molecule& m) {
    vec3 diff = m.orientation * (float)agentSize;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    drawSphere(m.position + diff, 0.4f * (float)agentSize);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
    drawCylinder(m.position - diff, m.position + diff, 0.2f * (float)agentSize);
    drawSphere(m.position - diff, 0.2f * (float)agentSize);

}

void drawMolecules() {
    quadratic = gluNewQuadric();
	for (unsigned int i = 0; i < n; ++i)
		drawMolecule(tab[i]);
    gluDeleteQuadric(quadratic);
}
