#ifndef VECTOR3_H
#define VECTOR3_H

#include<cmath>
#include"utils.h"

struct vector3{
	vector3(){}
	vector3(long double x, long double y, long double z):x(x), y(y), z(z){}

	long double x, y, z;

	long double length2() const{
		return x * x + y * y + z * z;
	}
	long double length() const{
		return sqrt(length2());
	}

	long double operator*(vector3 a) const{
		return x * a.x + y * a.y + z * a.z;
	}

	vector3 operator*(long double a) const{
		vector3 result;
		result.x = a * x;
		result.y = a * y;
		result.z = a * z;

		return result;
	}

	vector3& operator*=(long double a){
		x *= a;
		y *= a;
		z *= a;

		return *this;
	}

	vector3 crossProduct(vector3 a) const{
		vector3 result;
		result.x = y * a.z - a.y * z;
		result.y = z * a.x - a.z * x;
		result.z = x * a.y - a.x * y;


		return result;
	}

	vector3& normalize(){
		long double l = length();

		x /= l;
		y /= l;
		z /= l;

		return *this;
	}

	vector3& operator+=(vector3 a){
		x += a.x;
		y += a.y;
		z += a.z;

		return *this;
	}

	vector3& operator-=(vector3 a){
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

	vector3& operator/=(long double a){
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

vector3 operator*(long double, vector3);

vector3 distanceInToroidalSpace(vector3, vector3);

vector3 randomUnitVector();

#endif
