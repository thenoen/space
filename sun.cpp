#include "stdio.h"
#include <stdlib.h>
#include <math.h>

#include "sun.h"
#include "glut.h"
//#include "particle.h"

#define PI 3.14

Sun::Sun(float *speed, float weight, float radius, float x, float y, float z, GLuint *particleTexture)
{
	this->t = 0.40; // cas
	this->radius = radius;
	this->x = x;
	this->y = y;
	this->z = z;
	this->rotation_angle = 0;
	this->weight = weight;

	this->speed[0] = speed[0];
	this->speed[1] = speed[1];
	this->speed[2] = speed[2];

	//particle p[1000];
	//this->particles = (particle*)malloc(1000 * sizeof(particle));

	this->particleTexture = particleTexture;

	quadratic=gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

	for(int i=0; i<MAX_PARTICLES; i++)
	{
		initParticle(&particles[i]);
		// for(int j=0; j<MAX_PARTICLES; j++)
			if(particles[i].life <0)
				initParticle(&particles[i]);
			else
				moveParticle(&particles[i]);
	}
	
}

void Sun::draw()
{
	rotation_angle += 1;
	rotation_angle %= 360;
	glTranslatef(x,y,z);
	//printf("%f %f %f\n",x,y,z);
	//glRotatef(20,0,0,1); // odklon od osi rotacie
	glRotatef(rotation_angle,0,1,0); // otacanie okolo vlasnej osi
	glRotatef(-90,1,0,0); // otocenie kvoli texture
	/*float light[3] = {x,y,z};
	glLightfv(GL_LIGHT2, GL_POSITION, light);*/
	gluSphere(quadratic,this->radius,32,32); //nakeresli gulu
	
	//glRotatef(90,1,0,0); // otocenie aby bolo vidno casticu

	// particles
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);		// Enable Blending	
	//glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glBindTexture(GL_TEXTURE_2D, particleTexture[2]);
	//printf("%f\n",particles[1].life);
	for(int i=0; i<MAX_PARTICLES; i++)
	{
		{
			drawParticle(&particles[i]); 
			//printf("%f\n",particles[1].life);
			if(particles[i].life < 0.0)
			{
				initParticle(&particles[i]);
			}
		}
	}
	 glDepthMask(GL_TRUE);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glTranslatef(-x,-y,-z);
}

void Sun::initParticle(particle *p)
{
	p->active = true;
	p->life = 1.0f;					// Give It New Life
	p->fade = float(rand()%100)/1000.0f+0.003f;	// Random Fade Value
	p->x = 0.0f;						// Center On X Axis
	p->y = 0.0f;						// Center On Y Axis
	p->z = 0.0f;						// Center On Z Axis

	float a;

	a= rand()%2001 - 1000;
	a /= 1000;
	float x1 = a;
	
	a= rand()%2001 - 1000;
	a /= 1000;
	float x2 = a;
	
	p->xi = 2*x1*sqrt(1-pow(x1,2)-pow(x2,2));
	p->zi = 2*x2*sqrt(1-pow(x1,2)-pow(x2,2));
	p->yi = 1-2*(pow(x1,2)+pow(x2,2));

	a = this->radius * 3;
	p->xi *= a;
	p->yi *= a;
	p->zi *= a;


	//printf("%f %f %f\n\n", p->x, p->y, p->z);
	
	p->r = 255;			// Select Red From Color Table
	p->g = 125;			// Select Green From Color Table
	p->b = 7;			// Select Blue From Color Table

	//printf("init %f\n\n", p->fade);
}

void Sun::drawParticle(particle *p)
{
	float slowdown = 2.0f;

	float x=p->x;						// Grab Our Particle X Position
	float y=p->y;						// Grab Our Particle Y Position
	float z=p->z;					// Particle Z Pos + Zoom

	//glColor4ub(p.r,p.g,p.b,255);
	//printf("%d %d %d\n\n",p->r,p->g,p->b);
	//glColor4ub(255,127,0,p->life);

	glColor4f(1.0f,0.5f,0.0f,p->life);
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(x+0.5f,y+0.5f,z); // Top Right
		glTexCoord2d(0,1); glVertex3f(x-0.5f,y+0.5f,z); // Top Left
		glTexCoord2d(1,0); glVertex3f(x+0.5f,y-0.5f,z); // Bottom Right
		glTexCoord2d(0,0); glVertex3f(x-0.5f,y-0.5f,z); // Bottom Left
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(x+0.5f,y,z+0.5f); // Top Right
		glTexCoord2d(0,1); glVertex3f(x-0.5f,y,z+0.5f); // Top Left
		glTexCoord2d(1,0); glVertex3f(x+0.5f,y,z-0.5f); // Bottom Right
		glTexCoord2d(0,0); glVertex3f(x-0.5f,y,z-0.5f); // Bottom Left
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2d(1,1); glVertex3f(x,y+0.5f,z+0.5f); // Top Right
		glTexCoord2d(0,1); glVertex3f(x,y-0.5f,z+0.5f); // Top Left
		glTexCoord2d(1,0); glVertex3f(x,y+0.5f,z-0.5f); // Bottom Right
		glTexCoord2d(0,0); glVertex3f(x,y-0.5f,z-0.5f); // Bottom Left
	glEnd();

	/*glDisable(GL_TEXTURE_2D);
	glBegin(GL_POINTS);
		glVertex3f(x,y,z);
	glEnd();	
	glEnable(GL_TEXTURE_2D);*/

	
											// Done Building Triangle Strip

	p->x += p->xi/(slowdown*300);// Move On The X Axis By X Speed
	p->y += p->yi/(slowdown*300);// Move On The Y Axis By Y Speed
	p->z += p->zi/(slowdown*300);// Move On The Z Axis By Z Speed

	//p->xi += p->xg;			// Take Pull On X Axis Into Account
	//p->yi += p->yg;			// Take Pull On Y Axis Into Account
	//p->zi += p->zg;			// Take Pull On Z Axis Into Account
	p->life -= p->fade;		// Reduce Particles Life By 'Fade'
	//p->fade += 0.001;

	//glEnable(GL_LIGHTING);
	//printf("draw particle\n%f %f %f\n\n",x+0.5f,y+0.5f,z);
}

void Sun::moveParticle(particle *p)
{
	float slowdown = 2.0f;

	p->x += p->xi/(slowdown*300);// Move On The X Axis By X Speed
	p->y += p->yi/(slowdown*300);// Move On The Y Axis By Y Speed
	p->z += p->zi/(slowdown*300);// Move On The Z Axis By Z Speed

	//p->xi += p->xg;			// Take Pull On X Axis Into Account
	//p->yi += p->yg;			// Take Pull On Y Axis Into Account
	//p->zi += p->zg;			// Take Pull On Z Axis Into Account
	p->life -= p->fade;		// Reduce Particles Life By 'Fade'
}

float Sun::getWeight()
{
	return this->weight;
}

float* Sun::getPosition()
{
	float xposition[3] = {this->x,this->y,this->z};
	return xposition;
}

float Sun::getX()
{
	return this->x;
}

float Sun::getY()
{
	return this->y;
}

float Sun::getZ()
{
	return this->z;
}