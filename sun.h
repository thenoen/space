#ifndef SUN_H
#define SUN_H

#include "glut.h"
//#include "particle.h"

#define MAX_PARTICLES 10000 // <-------------------------------------- treba dat do konsturktora

//typedef struct	// Create A Structure For Particle
class particle
{
public:
	bool	active;					// Active (Yes/No)
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	GLuint	r;						// Red Value
	GLuint	g;						// Green Value
	GLuint	b;						// Blue Value
	float	x;						// X Position
	float	y;						// Y Position
	float	z;						// Z Position
	float	xi;						// X Direction
	float	yi;						// Y Direction
	float	zi;						// Z Direction
	float	xg;						// X Gravity
	float	yg;						// Y Gravity
	float	zg;						// Z Gravity
};//particle;

class Sun
{
private:
	float x,y,z;
	float weight;
	float radius;
	float speed[3];

	GLuint *particleTexture;
	particle particles[MAX_PARTICLES];

	float t;
	int rotation_angle;

	GLUquadricObj *quadratic;

	Sun(){};
	void initParticle(particle*);
	void drawParticle(particle*);
	void moveParticle(particle *p);

public:
	Sun(float *speed, float weight, float radius, float x, float y, float z, GLuint *particleTexture);

	void draw();
	float getSpeed();
	float getWeight();
	float* getPosition();
	float getX();
	float getY();
	float getZ();
};

#endif