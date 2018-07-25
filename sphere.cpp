#include "stdio.h"
#include <math.h>

#include "sphere.h"
#include "glut.h"


Sphere::Sphere(float *speed, float weight, float radius, float x, float y, float z, float tilt)
{
	this->t = 5; // cas
	this->radius = radius;
	this->x = x;
	this->y = y;
	this->z = z;
	this->rotation_angle = 0;
	this->tilt = tilt;

	this->weight = weight;

	this->speed[0] = speed[0];
	this->speed[1] = speed[1];
	this->speed[2] = speed[2];

	quadratic=gluNewQuadric();// Vrátí ukazatel na nový kvadrik
	gluQuadricNormals(quadratic, GLU_SMOOTH);// Vygeneruje normálové vektory (hladké)
	gluQuadricTexture(quadratic, GL_TRUE);// Vygeneruje texturové koordináty

	step = 0;
	for(int i=0; i<1000; i++)
		path[i][0] = 1.0f;
	
	
}

void Sphere::draw()
{
	glBegin(GL_POINTS);
	for(int i=0; i<1000; i++)
	{

		if(path[i][0] == 1.0f)
		{
			glVertex3f(path[i][1],path[i][2],path[i][3]);
		}

	}
	glEnd();
	rotation_angle += 1;
	rotation_angle %= 360;
	glTranslatef(x,y,z);

	glRotatef(tilt,0,0,1); // odklon od osi rotacie
	glRotatef(rotation_angle,0,1,0);
	glRotatef(-90,1,0,0);
	gluSphere(quadratic,this->radius,32,32);
	//glRotatef(90,1,0,0);	
	//glTranslatef(-x,-y,-z);S
	//glRotatef(-rotation_angle,0,1,0);
}

void Sphere::computePosition(float px, float py, float pz, float weight)
{
	float gravity = 0;

	// smer vektora sily v jendolivych smeroch
	float dx = px - this->x; 
	float dy = py - this->y;
	float dz = pz - this->z;

	gravity = sqrt(pow(dx,2) + pow(dy,2) + pow(dz,2)); // dlzka vektora - vzdialenost

	float force = kappa * (this->weight * weight) / pow(gravity,2); // velkost sily

	//printf("%e\n",force);

	float gx, gy, gz;

	// jednotkovy vektor sily
	gx = dx/gravity;
	gy = dy/gravity;
	gz = dz/gravity;

	//float j = sqrt(pow(gx,2) + pow(gy,2) + pow(gz,2)); // test jednotkoveho vektora - OK
	//printf("%f\n",j);

	// zlozky vektora sily
	gx *= force;
	gy *= force;
	gz *= force;
	//printf("%f\n",gx);


	// rychlost v smere x
	this->speed[0] += gx/this->weight * this->t;
	this->speed[1] += gy/this->weight * this->t;
	this->speed[2] += gz/this->weight * this->t;
	//this->speed[0] /= 1000;
	//printf("%f\n",this->speed[0]);


	//printf("%f \n", this->speed[0]);

	// poszicia v smere x
	this->x += this->speed[0] * this->t;
	this->y += this->speed[1] * this->t;
	this->z += this->speed[2] * this->t;


	path[step][0] = 1.0f;
	path[step][1] = this->x;
	path[step][2] = this->y;
	path[step][3] = this->z;

	step++;

	if(step > 999)
		step = 0;

	//this->x += this->speed[0] * this->t;
	//this->y += this->speed[1] * this->t;
	//this->z += this->speed[2] * this->t;

	/*printf("force: %f\n",gf[0]);
	printf("speed: %f\n",this->speed[0]);
	printf("position: %f\n\n",this->x);*/

	/*printf("%+.5f\n",this->x);
	printf("%+.5f\n",this->y);
	printf("%+.5f\n\n",this->z);*/

	/*printf("%+.5f\n",this->speed[0]);
	printf("%+.5f\n",this->speed[1]);
	printf("%+.5f\n\n",this->speed[2]);*/

	/*printf("%+.5f\n",gf[0]);
	printf("%+.5f\n",gf[1]);
	printf("%+.5f\n\n",gf[2]);*/

}

float Sphere::getWeight()
{
	return this->weight;
}

float* Sphere::getPosition()
{
	float xposition[3] = {this->x,this->y,this->z};
	return xposition;
}

float Sphere::getX()
{
	return this->x;
}

float Sphere::getY()
{
	return this->y;
}

float Sphere::getZ()
{
	return this->z;
}