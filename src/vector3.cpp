#include"vector3.h"
#include"utils.h"
#include<cmath>

vector3 operator*(long double a, vector3 b) {
    vector3 result;
    result.x = a * b.x;
    result.y = a * b.y;
    result.z = a * b.z;

    return result;
}

/**
 * Considering 1D toroidal space, this function gives the position with the least possible absolute value.
 * @param x Position of the point.
 * @param sizeX Length of the space.
 * @return Equivalent position for the point, which has the least absolute value.
 */
static long double theLeastPossible(long double& x, long double sizeX) {
    x = fmod(x, sizeX);

    if (x < -sizeX / 2)
        x += sizeX;
    else if (x > sizeX / 2)
        x -= sizeX;
    return x;
}

vector3 distanceInToroidalSpace(vector3 d, vector3 sizeOfTheArea) {
    theLeastPossible(d.x, sizeOfTheArea.x);
    theLeastPossible(d.y, sizeOfTheArea.y);
    theLeastPossible(d.z, sizeOfTheArea.z);

    return d;
}

vector3 randomUnitVector() {
    long double z = 2 * randomDouble() - 1;
    long double theta = randomDoubleWithout1() * 2 * M_PI;

    long double root = sqrt(1 - z * z);

    vector3 result;

    result.x = root * cos(theta);
    result.y = root * sin(theta);
    result.z = z;

    return result;
}

