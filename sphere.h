#ifndef SPHERE_H
#define SPHERE_H

#include "glut.h"

const float kappa = 6.6742e-11;

class Sphere
{
private:
	float x,y,z;
	float weight;
	float radius;
	float speed[3];
	float tilt;

	float path[1000][4];
	int step;

	float t;
	int rotation_angle;

	GLUquadricObj *quadratic;

	Sphere(){};
	

public:
	Sphere(float *speed, float weight, float radius, float x, float y, float z, float tilt);

	void draw();
	float getSpeed();
	float getWeight();
	float* getPosition();
	void computePosition(float x, float y, float z, float weight);
	float getX();
	float getY();
	float getZ();
};

#endif