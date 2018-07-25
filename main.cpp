#include <stdio.h>
#include <math.h>

#include "SOIL.h"

#include "glut.h"
#include "ArcBall.h"

#include "sphere.h"
#include "sun.h"


#pragma comment(lib,"glaux.lib")

GLuint earthTexture[3];
GLuint moonTexture[3];
GLuint sunTexture[3];
GLuint particleTexture[3];

int sizeX = 0, sizeY = 0;
float z = 3;
GLUquadricObj *quadratic;

//-------------------------------------------------------------------------------------------------------------
float earthSpeed[3] = {0,-0.01,0.02};
Sphere *earth = new Sphere(earthSpeed,3500000000/*597.36e22*/,6.378,0,0,-15,20);

float moonSpeed[3] = {0,0.1,-0.12};
Sphere *moon = new Sphere(moonSpeed,10/*7.3477e22*/,1.737,-10/*384e6*/,0,-15,-5);

float sunSpeed[3] = {0,0,0};
float sunPosition[3] = {-60,0,-30};
Sun *sun = new Sun(sunSpeed,5e8/*777.3477e22*/, 20, sunPosition[0], sunPosition[1], sunPosition[2], particleTexture); // pomerovo spravna velkost slnka je: 695.202
//-------------------------------------------------------------------------------------------------------------

GLfloat LightAmbient[]=		{ 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ sunPosition[0], sunPosition[1], sunPosition[2], 1.0f };

GLfloat sunAmbient[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat sunDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat sunLightPosition1[]=	{ sunPosition[0]+1000, sunPosition[1]+1000, sunPosition[2]+1000, 1.0f };
GLfloat sunLightPosition2[]=	{ sunPosition[0]-1000, sunPosition[1]-1000, sunPosition[2]-1000, 1.0f };

float camera_angle_x = -90,
	  camera_angle_y,
	  camera_angle_z,
	  camera_position_x = 0,
	  camera_position_y = 7,
	  camera_position_z = 17;

// vektor smeru zoomovania
float direction[3] = {0,0,0.1};

// otocena suradnicova sustava
float x_axis[3] = {1,0,0};
float y_axis[3] = {0,1,0};
float z_axis[3] = {0,0,1};

void x_rotation(float angle)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = 1*direction[0];
	y = a*direction[1] - b*direction[2];
	z = b*direction[1] + a*direction[2];
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
	printf("x rotation: %+.5f %+.5f %+.5f \n",direction[0], direction[1], direction[2]);
}

void x_rotation(float angle, float *vector)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = 1 * vector[0];
	y = a * vector[1] - b * vector[2];
	z = b * vector[1] + a * vector[2];
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	printf("x rotation: %+.5f %+.5f %+.5f \n",vector[0], vector[1], vector[2]);
}

void y_rotation(float angle)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = a*direction[0] + b*direction[2];
	y = 1*direction[1];
	z = a*direction[2] - b*direction[0];
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
	printf("y rotation: %+.5f %+.5f %+.5f \n",direction[0], direction[1], direction[2]);
}

void y_rotation(float angle, float *vector)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = a * vector[0] + b * vector[2];
	y = 1 * vector[1];
	z = a * vector[2] - b * vector[0];
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	printf("y rotation: %+.5f %+.5f %+.5f \n",vector[0], vector[1], vector[2]);
}

void z_rotation(float angle)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = a*direction[0] - b*direction[1];
	y = b*direction[0] + a*direction[1];
	z = 1*direction[2];
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
	printf("z rotation: %+.5f %+.5f %+.5f \n",direction[0], direction[1], direction[2]);
}

void z_rotation(float angle, float *vector)
{
	angle = angle*(3.14/180);
	float a = cos(angle);
	float b = sin(angle);
	
	float x, y, z;
	x = a*vector[0] - b*vector[1];
	y = b*vector[0] + a*vector[1];
	z = 1*vector[2];
	vector[0] = x;
	vector[1] = y;
	vector[2] = z;
	printf("z rotation: %+.5f %+.5f %+.5f \n",vector[0], vector[1], vector[2]);
}

void keyboard(unsigned char key, int x, int y)
{
    //glColor3f(0,1,0);
    //printf("pressed: %d\n",key);

	switch (key)
	{
	case 'w'://x
		glTranslatef(0,0,-60);
		glRotatef(-1,x_axis[0],x_axis[1],x_axis[2]);
		glTranslatef(0,0,60);
		x_rotation(1,direction);
		//x_rotation(1,x_axis);
		x_rotation(1,y_axis);
		x_rotation(1,z_axis);
		camera_angle_x += -1;
		break;
	case 's'://x
		glTranslatef(0,0,-60);
		glRotatef(1,x_axis[0],x_axis[1],x_axis[2]);
		glTranslatef(0,0,60);
		x_rotation(-1,direction);
		//x_rotation(-1,x_axis);
		x_rotation(-1,y_axis);
		x_rotation(-1,z_axis);
		camera_angle_x += 1;
		break;
	case 'q'://z
		glTranslatef(0,0,-60);
		//glRotatef(1,0,0,1);
		glRotatef(1,z_axis[0],z_axis[1],z_axis[2]);
		glTranslatef(0,0,60);
		z_rotation(1,direction);
		z_rotation(1,x_axis);
		z_rotation(1,y_axis);
		//z_rotation(1,z_axis);
		camera_angle_z += 1;
		break;
	case 'e'://z
		glTranslatef(0,0,-60);
		//glRotatef(-1,0,0,1);
		glRotatef(-1,z_axis[0],z_axis[1],z_axis[2]);
		glTranslatef(0,0,60);
		z_rotation(-1,direction);
		z_rotation(-1,x_axis);
		z_rotation(-1,y_axis);
		//z_rotation(-1,z_axis);
		camera_angle_z += -1;
		break;
	case 'a'://y
		glTranslatef(0,0,-60);
		glRotatef(-1,y_axis[0],y_axis[1],y_axis[2]);
		glTranslatef(0,0,60);
		y_rotation(1,direction);
		y_rotation(1,x_axis);
		//y_rotation(1,y_axis);
		y_rotation(1,z_axis);
		camera_angle_y += -1;
		break;
	case 'd'://y
		glTranslatef(0,0,-60);
		glRotatef(1,y_axis[0],y_axis[1],y_axis[2]);
		glTranslatef(0,0,60);
		y_rotation(-1,direction);
		y_rotation(-1,x_axis);
		//y_rotation(-1,y_axis);
		y_rotation(-1,z_axis);
		camera_angle_y += 1;
		break;
	case 'r':
		//glTranslatef(0,0,0.1);
		glTranslatef(direction[0], direction[1], direction[2]);
		printf("translation: %+.5f %+.5f %+.5f \n",direction[0], direction[1], direction[2]);
		//camera_position_z += 0.1;
		break;
	case 't':
		glLoadIdentity();
		//glTranslatef(0,0,0.1);
		//glTranslatef(direction[0], direction[1], direction[2]);
		//camera_position_z += 0.1;
		break;
	case 'f':
		//glTranslatef(0,0,-0.1);
		glTranslatef(-direction[0], -direction[1], -direction[2]);
		//camera_position_z += -0.1;
		break;
	case 'p':
		gluLookAt(0,3,0,0,0,17,0,1,0);
		break;
	}
	glFlush();
}

// AUX_RGBImageRec *LoadBMP(char *Filename)				// Loads A Bitmap Image
// {
// 	FILE *File=NULL;									// File Handle

// 	if (!Filename)										// Make Sure A Filename Was Given
// 	{
// 		return NULL;									// If Not Return NULL
// 	}

// 	File=fopen(Filename,"r");							// Check To See If The File Exists

// 	if (File)											// Does The File Exist?
// 	{
// 		fclose(File);									// Close The Handle
// 		return auxDIBImageLoad(Filename);				// Load The Bitmap And Return A Pointer
// 	}

// 	return NULL;										// If Load Failed Return NULL
// }


int LoadGLTextures(char *fileName, GLuint *texture)									// Load Bitmaps And Convert To Textures
{

	// GLuint tex_2d = SOIL_load_OGL_texture
	// (
	// 	fileName,//"img.png",
	// 	SOIL_LOAD_AUTO,
	// 	SOIL_CREATE_NEW_ID,
	// 	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	// );

	

	// if( 0 == tex_2d )
	// {
	// 	printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	// }

	// int Status=FALSE;									// Status Indicator

	// AUX_RGBImageRec *TextureImage[1];					// Create Storage Space For The Texture

	// memset(TextureImage,0,sizeof(void *)*1);           	// Set The Pointer To NULL

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	// if (TextureImage[0]=LoadBMP(fileName))
	// {
		// Status=TRUE;									// Set The Status To TRUE

		int width, height;
		unsigned char*  image;
		glGenTextures(3, &texture[0]);					// Create Three Textures

		image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGB);

		printf( "SOIL loading status: '%s'\n", fileName );
		printf( "SOIL loading status: '%s'\n", SOIL_last_result() );

		// Create Nearest Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);		
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		// Create Linear Filtered Texture
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		// Create MipMapped Texture
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);		
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

		SOIL_free_image_data(image);
		
	return 1;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,1000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	printf("initGL\n");
	if (!LoadGLTextures("Data/earth.bmp",earthTexture))								// Jump To Texture Loading Routine
	{
		return 1;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("Data/moon.bmp",moonTexture))								// Jump To Texture Loading Routine
	{
		return 1;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("Data/sunParticle.bmp",particleTexture))								// Jump To Texture Loading Routine
	{
		return 1;									// If Texture Didn't Load Return FALSE
	}

	if (!LoadGLTextures("Data/sun.bmp",sunTexture))								// Jump To Texture Loading Routine
	{
		return 1;									// If Texture Didn't Load Return FALSE
	}

	//
	//glEnable(GL_COLOR_MATERIAL);
	//
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	/* ambient light for sun */
	glLightfv(GL_LIGHT2, GL_AMBIENT, sunAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, sunDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT2, GL_POSITION, sunLightPosition1);	// Position The Light

	glLightfv(GL_LIGHT3, GL_AMBIENT, sunAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT3, GL_DIFFUSE, sunDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT3,GL_POSITION, sunLightPosition2);	// Position The Light

	
	//quadratic=gluNewQuadric();							// Create A Pointer To The Quadric Object (Return 0 If No Memory) (NEW)
	//gluQuadricNormals(quadratic, GLU_SMOOTH);			// Create Smooth Normals (NEW)
	//gluQuadricTexture(quadratic, GL_TRUE);				// Create Texture Coords (NEW)

	return 0;										// Initialization Went OK
}

float angle = 0;
float distance = 120;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.0f,0.0f);	
	//glLoadIdentity();

	// otacanie kamery
	/*glRotatef(camera_angle_x,1,0,0);
	glRotatef(camera_angle_y,0,1,0);
	glRotatef(camera_angle_z,0,0,1);

	direction[0] = camera_position_x;
	direction[1] = camera_position_y;
	direction[2] = camera_position_z;
	x_rotation(camera_angle_x);
	y_rotation(camera_angle_y);
	z_rotation(camera_angle_z);
	glTranslatef(direction[0], direction[1], direction[2]);*/

	
	glTranslatef(0,0,-distance);// ***
	//glRotatef(1,0,1,0);// ***

	

	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition); // musi byt tu - pred posunutim
	glLightfv(GL_LIGHT2, GL_POSITION,sunLightPosition1);
	glLightfv(GL_LIGHT3, GL_POSITION,sunLightPosition2);

	earth->computePosition(sun->getX(), sun->getY(), sun->getZ(), sun->getWeight());
	//printf("%f %f %f %f\n\n",moon->getX(), moon->getY(), moon->getZ(), moon->getWeight());
	moon->computePosition(earth->getX(), earth->getY(), earth->getZ(), earth->getWeight());


	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, earthTexture[2]);
		earth->draw();
	glPopMatrix();
	
	//LightPosition[0] -= 0.01;
	//printf("%f \n", LightPosition[0]);
	
	
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, moonTexture[2]);
		moon->draw();
	glPopMatrix();

	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glDisable(GL_LIGHT1);
	glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, sunTexture[2]);
		sun->draw();
	glPopMatrix();
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHT3);
	glDisable(GL_LIGHT2);
    	

	glTranslatef(0,0,distance);// ***
	
	glFlush();
	glutSwapBuffers();
}

void Timer(int extra)
{
	glutPostRedisplay();
	glutTimerFunc(40,Timer,0); //25fps = 40
}

int main(int argc, char *argv[])
{
	printf("Hello World!\n");

	glutInit(&argc, argv);

	//size = 0;
	int windowWidth = glutGet(GLUT_SCREEN_WIDTH);
	int windowHeight = glutGet(GLUT_SCREEN_HEIGHT);
	sizeX = windowWidth - 50;
	sizeY = int(sizeX/2);

	glutInitWindowPosition((windowWidth-sizeX)/2, (windowHeight-sizeY)/2);
			

	//printf("Size: %d \n",size);
	glutInitWindowSize(sizeX, sizeY);

	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow ("Space");

	//glutReshapeFunc(reshape);

	glutDisplayFunc(display);
	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardFunc(keyboard);
	
	InitGL();
	glutTimerFunc(0,Timer,0);
	glutMainLoop();

	

	return 0;
}