#ifndef VECTOR3_H
#define VECTOR3_H

#include<cmath>
#include"utils.h"

/**
 * Struct representing a 3D vector.
 */
struct vector3 {

    vector3() {
    }

    vector3(long double x, long double y, long double z) : x(x), y(y), z(z) {
    }

    long double x, y, z;

    long double length2() const {
        return x * x + y * y + z * z;
    }

    long double length() const {
        return sqrt(length2());
    }

    long double operator*(vector3 a) const {
        return x * a.x + y * a.y + z * a.z;
    }

    vector3 operator*(long double a) const {
        vector3 result;
        result.x = a * x;
        result.y = a * y;
        result.z = a * z;

        return result;
    }

    vector3& operator*=(long double a) {
        x *= a;
        y *= a;
        z *= a;

        return *this;
    }

    /**
     * Cross product of the vector with another vector.
     * @param a Second vector.
     * @return Vector - the cross product.
     */
    vector3 crossProduct(vector3 a) const {
        vector3 result;
        result.x = y * a.z - a.y * z;
        result.y = z * a.x - a.z * x;
        result.z = x * a.y - a.x * y;


        return result;
    }

    /**
     * Normalizes the vector. After the execution, vector has length equal to 1.
     * @return Reference to the vector.
     */
    vector3& normalize() {
        long double l = length();

        x /= l;
        y /= l;
        z /= l;

        return *this;
    }

    vector3& operator+=(vector3 a) {
        x += a.x;
        y += a.y;
        z += a.z;

        return *this;
    }

    vector3& operator-=(vector3 a) {
        x -= a.x;
        y -= a.y;
        z -= a.z;

        return *this;
    }

    vector3 operator-(vector3 a) const {
        vector3 result;
        result.x = x - a.x;
        result.y = y - a.y;
        result.z = z - a.z;

        return result;
    }

    vector3 operator+(vector3 a) const {
        vector3 result;
        result.x = x + a.x;
        result.y = y + a.y;
        result.z = z + a.z;

        return result;
    }

    vector3 operator-() const {
        vector3 result;
        result.x = -x;
        result.y = -y;
        result.z = -z;

        return result;
    }

    vector3& operator/=(long double a) {
        x /= a;
        y /= a;
        z /= a;

        return *this;
    }

    vector3 operator/(long double a) const {
        vector3 result;
        result.x = x / a;
        result.y = y / a;
        result.z = z / a;

        return result;
    }

};

vector3 operator*(long double a, vector3 b);

/**
 * Considering toroidal space, returns an equivalent distance vector with the least absolute value.
 * @param d Distance vector.
 * @param sizeOfTheArea Simulation volume size.
 * @return Equivalent vector with the least absolute value possible.
 */
vector3 distanceInToroidalSpace(vector3 d, vector3 sizeOfTheArea);

/**
 * Generates a random unit vector. Every direction is equally probable.
 * @return Random unit vector.
 */
vector3 randomUnitVector();

#endif
