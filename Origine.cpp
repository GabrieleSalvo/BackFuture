// ----------------------------------------------------------------------------
// Simple sample to prove that Assimp is easy to use with OpenGL.
// It takes a file name as command line parameter, loads it using standard
// settings and displays it.
//
// If you intend to _use_ this code sample in your app, do yourself a favour 
// and replace immediate mode calls with VBOs ...
//
// The vc8 solution links against assimp-release-dll_win32 - be sure to
// have this configuration built.
// ----------------------------------------------------------------------------


// assimp include files. These three are usually needed.
#include "Scene.h"
#include "assimp.h"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "GL/glut.h"
#include <windows.h>
#include <IL/il.h>
//#include <IL/ilu.h>
#include <iostream>
//to map image filenames to textureIds
#include <string.h>
#include <map>
#include <cmath>
#include "camera.h"

#define RED 1,0,0
#define GREEN 0,1,0
#define M_PI 3.14159265358979323846
#define SW 900
#define SH 600
#define OGGETTO_CERCATO 7
#define DELOREAN 18

// pointer to scene object, to instantiate single .obj indipendently

Scene *piazza = NULL, *deloreanScene = NULL, *barraUranio1=NULL, *barraUranio2=NULL, *barraUranio3=NULL, *barraUranio4=NULL, *barraUranio5=NULL, *barraUranio6=NULL;
Camera *camera = NULL;

// paths to .obj files
static const std::string pathscene_1 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\hill_valley_def_blender_render.obj";
static const std::string pathscene_2 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\hill_valley_def_blender_render.obj";
static const std::string path_delorean = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\delorean.obj";
static const std::string path_barra1 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio.obj";
static const std::string path_barra2 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio2.obj";
static const std::string path_barra3 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio3.obj";
static const std::string path_barra4 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio4.obj";
static const std::string path_barra5 = "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio5.obj";
static const std::string path_barra6= "C:\\Users\\Gabriele\\source\\repos\\ProjectAssimp2\\Debug\\barraUranio6.obj";

//variables for display lists
GLuint scene_list = 0;
GLubyte lists[10];
int level = 1;
int t = 0;

//booleans for the game
bool objectFound[6] = {false, false, false, false, false, false};
bool departure = false;

int menu;

// identifiers for the initial tutorial, known as state variables to select screen text for the user
enum BUTTONS {NO_BUTTON, W, S, A, D, MOUSE};
// tutorial variable, first value is NO_BUTTON
int lastKeyPressed = 0;
// boolean for the first mouseMovement call, so that it won't change psi and theta
bool firstTimeCursor = true;
//variables for the game time
float time = 0;
float delta = 0;

//data structure to handle collisions, it is an immaginary cube around the object
typedef struct Box {
	float min_x;
	float min_y;
	float min_z;
	float max_x;
	float max_y;
	float max_z;
}Box;

Box delorean = { -100, -100, 1.4,
				-2.45,  100, 3.3};
Box marciapiedeCinema = {4.8, -100, -4.5,
						 100, +100, 3.45};
Box texaco = {1.2, -100, 5.8,
			  2.9, +100, +100};
Box marciapiedeLou = {-1.8, -100, 5.75,
					  0.8, +100, +100};
Box marciapiedeFondo = {-100, -100, -100, 
						+100,+100, -6 };
Box marciapiedeBanca = {-100,-100, -4.6,
						-3.3, +100, 6};
Box municipio = {-1.4, -100, -3.4,
				 2.6, +100, -0.8};

//light variables
GLfloat LightAmbient[] = { 0, 0, 0, 1};
GLfloat LightDiffuse[] = { 0.5, 0.5, 0.5, 1.0f };
GLfloat LightPosition[] = { 0.0f, 15.0f, 0.0f, 1.0f };

//prototypes
void display();
bool inside_box(Box box, float try_x, float try_y, float try_z);
void printValues(int value);
void myreshape(int w, int h);
void printOnScreen(float x, float y, std::string string, float R, float G, float B);
void level2(void);
void level1(void);
void display(void);
int InitGL();
void printValues(int value); //camera debugging function
bool rightPosition(float x, float y, float z);
void processNormalKeys(unsigned char key, int xx, int yy);
bool inside_box(Box box, float try_x, float try_y, float try_z);
bool select();
void mouseMovement(int x, int y);
void mouseClick(int button, int state, int x, int y);

void myreshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(55, (GLfloat)w / (GLfloat)h, 0.1, 1000.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}
// ----------------------------------------------------------------------------


void printOnScreen(float x, float y, std::string string, float R, float G, float B)
{
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, SW, 0.0, SH);
	glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glColor3f(R, G, B);
		glRasterPos2i(x, y);
		
		void * font = GLUT_BITMAP_HELVETICA_18;

		for (std::string::iterator i = string.begin(); i != string.end(); ++i)
		{
			char c = *i;
			//this does nothing, color is fixed for Bitmaps when calling glRasterPos
			//glColor3f(1.0, 0.0, 1.0); 
			glutBitmapCharacter(font, c);
		}
		glEnable(GL_LIGHTING);
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
//----------------------------------------------------------------------------------------
void level2(void)
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//during the delorean departure, camera keeps pointed to a certain frame
	if (departure == true && t<100){
		camera->setX(-1.978147);
		camera->setY(1.600000);
		camera->setZ(-4.610109);
		float psiRad = 108.861981180 / 180 * M_PI;
		camera->setPsi(psiRad);
		float thetaRad = 342.811316 / 180 * M_PI;
		camera->setTheta(thetaRad);
		firstTimeCursor = true;
	}
	gluLookAt(camera->getX(), camera->getY(),  camera->getZ(),  
		camera->getSightX() , camera->getSightY(), camera->getSightZ(), 
	    0,1,0);

	//during the first display call, display lists are written
	if (scene_list == 0) {
		scene_list = glGenLists(2);
		//square list
		glNewList(scene_list, GL_COMPILE);
		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
			glPushMatrix();
			piazza->recursive_render(piazza->getRootNode(), 1.0);
			glPopMatrix();
		glEndList();
		// delorean list
		glNewList(scene_list + 1, GL_COMPILE);
			glPushMatrix();
			deloreanScene->recursive_render(deloreanScene->getRootNode(), 1.0);
			glPopMatrix();
		glEndList();

		//uran bars list
		glNewList(scene_list + 2, GL_COMPILE);
			glPushMatrix();
			barraUranio4->recursive_render(barraUranio4->getRootNode(), 1.0);
			glPopMatrix();
		glEndList();

		glNewList(scene_list + 3, GL_COMPILE);
		glPushMatrix();
		barraUranio5->recursive_render(barraUranio5->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();

		glNewList(scene_list + 4, GL_COMPILE);
			glPushMatrix();
			barraUranio6->recursive_render(barraUranio6->getRootNode(), 1.0);
			glPopMatrix();
		glEndList();

		// array of lists
		lists[0] = 0; 
		lists[1] = 1;
		lists[2] = 2;
		lists[3] = 3;
		lists[4] = 4;
		glListBase(scene_list);
	}
	// when you find an uran bar, flag turns to TRUE and the next display call will not show it
	if (objectFound[3] == true) {
		glNewList(scene_list + 2, GL_COMPILE);
		glEndList();
	}
	if (objectFound[4] == true) {
		glNewList(scene_list + 3, GL_COMPILE);
		glEndList();
	}
	if (objectFound[5] == true) {
		glNewList(scene_list + 4, GL_COMPILE);
		glEndList();
	}
	//car movement
	if ( departure == true && t<50) {
		float acceleration = 0.05;
		
		glNewList(scene_list + 1, GL_COMPILE);
			glPushMatrix();
			//delorean moves with an accelerated uniform motion
			glTranslatef(0, 0, -(0.5*acceleration*(t*t)));
			deloreanScene->recursive_render(deloreanScene->getRootNode(), 1.0);
			glPopMatrix();
		glEndList();
		t++;
	}

	//call the visualisation lists
	glCallLists(5, GL_UNSIGNED_BYTE, lists);
	
	// cursor
	if (departure == false) {
		glPushMatrix();
			glLoadIdentity();
			glDisable(GL_LIGHTING);
			glTranslatef(0, 0, -0.5);
			glColor3f(1, 1, 1);
			glutWireSphere(0.025, 2, 2);
			glRotatef(90, 0, 0, 1);
			glutWireSphere(0.025, 2, 2);
			glEnable(GL_LIGHTING);
		glPopMatrix();
	}
	//print the final time and final congratulations message 
	if (t == 50) {
		int secondi = delta / 1000;
		int minuti = secondi / 60;
		int sec_rimasti = secondi % 60;
		std::string str_minuti = std::to_string(minuti);
		std::string str_secondi = std::to_string(sec_rimasti);
		std::string risultato = "Hai impiegato "+str_minuti+" minuti e "+str_secondi+" secondi!";
		printOnScreen(SW / 2 - 125 + 50, SH / 2 + 30, "CONGRATULAZIONI!!!", RED);
		printOnScreen(SW / 2 - 125, SH / 2, "Marty e' partito per il futuro!!!", RED );
		printOnScreen(SW / 2 - 125, SH / 2 - 30, risultato, RED);
		printOnScreen(SW / 2 - 125 + 40, SH / 2 - 60, "Premi ESC per uscire.", RED);
	}
	//print when the player finds a bar
	if (departure == false) {
		int found = 0;
		for (int i = 3; i < 6; i++) {
			found += (int)objectFound[i];
		}
		if (found < 3) {
			if (objectFound[3] == true)
				printOnScreen(SW - 220, SH - 30, "Hai preso la barra n 4!!!",	RED);
			if (objectFound[4] == true)
				printOnScreen(SW - 220, SH - 60, "Hai preso la barra n 4!!!", RED);
			if (objectFound[5] == true)
				printOnScreen(SW - 220, SH - 90, "Hai preso la barra n 5!!!", RED);
		}
		else
			printOnScreen(SW - 220, SH - 30, "Clicca sull'auto per partire!", RED);
		
	}

	printOnScreen(10, SH-30, "LIVELLO 2", RED);
	printOnScreen(10, 10, "Prendi le altre tre barre d'Uranio ROSSE e fai partire Marty McFly!", RED);

	glutSwapBuffers();
}
//-----------------------------------------------------------------------------
void level1(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(camera->getX(), camera->getY(), camera->getZ(),
		camera->getSightX(), camera->getSightY(), camera->getSightZ(),
		0, 1, 0);

	// if the display list has not been made yet, create a new one and
	// fill it with scene contents
	if (scene_list == 0) {
		scene_list = glGenLists(2);
		//lista con la piazza
		glNewList(scene_list, GL_COMPILE);
		// now begin at the root node of the imported data and traverse
		// the scenegraph by multiplying subsequent local transforms
		// together on GL's matrix stack.
		glPushMatrix();
		piazza->recursive_render(piazza->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();
		// lista con la delorean
		glNewList(scene_list + 1, GL_COMPILE);
		glPushMatrix();
		deloreanScene->recursive_render(deloreanScene->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();

		//lista per barra uranio
		glNewList(scene_list + 2, GL_COMPILE);
		glPushMatrix();
		barraUranio1->recursive_render(barraUranio1->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();

		glNewList(scene_list + 3, GL_COMPILE);
		glPushMatrix();
		barraUranio2->recursive_render(barraUranio2->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();

		glNewList(scene_list + 4, GL_COMPILE);
		glPushMatrix();
		barraUranio3->recursive_render(barraUranio3->getRootNode(), 1.0);
		glPopMatrix();
		glEndList();

		// array of lists
		lists[0] = 0;
		lists[1] = 1;
		lists[2] = 2;
		lists[3] = 3;
		lists[4] = 4;
		glListBase(scene_list);
	}
	//when a player finds an uran bar, the flag turns to true and the next display call will not draw it 
	if (objectFound[0] == true) {
		glNewList(scene_list + 2, GL_COMPILE);
		glEndList();
	}
	if (objectFound[1] == true) {
		glNewList(scene_list + 3, GL_COMPILE);
		glEndList();
	}
	if (objectFound[2] == true) {
		glNewList(scene_list + 4, GL_COMPILE);
		glEndList();
	}
	//viualisation list call
	glCallLists(5, GL_UNSIGNED_BYTE, lists);

	// draw the cursor
	if (departure == false) {
		glPushMatrix();
		glLoadIdentity();
		glDisable(GL_LIGHTING);
		glTranslatef(0, 0, -0.5);
		glColor3f(1, 1, 1);
		glutWireSphere(0.025, 2, 2);
		glRotatef(90, 0, 0, 1);
		glutWireSphere(0.025, 2, 2);
		glEnable(GL_LIGHTING);
		glPopMatrix();
	}

	//tutorial for keyboard and mouse
	if (lastKeyPressed == NO_BUTTON) {
		printOnScreen(SW / 2 - 125 + 20, SH / 2 + 30, "Premi W per andare avanti", GREEN);
	}
	else if (lastKeyPressed == W) {
		printOnScreen(SW / 2 - 125 + 20, SH / 2 + 30, "Premi S per andare indietro", GREEN);
	}
	else if (lastKeyPressed == S) {
		printOnScreen(SW / 2 - 125 + 20, SH / 2 + 30, "Premi A per andare a sinistra", GREEN);
	}
	else if (lastKeyPressed == A) {
		printOnScreen(SW / 2 - 125 + 30, SH / 2 + 30, "Premi D per andare a destra", GREEN);
	}
	else if (lastKeyPressed == D) {
		printOnScreen(SW / 2 - 125 - 30, SH / 2 + 30, "Muovi il mouse per ruotare la telecamera", GREEN);
	}

	//stampo a schermo quando prendo la barra d'uranio
	if (departure == false) {
		int found = 0;
		for (int i = 0; i < 3; i++) {
			found += (int)objectFound[i];
		}
		if (found < 3) {
			if (objectFound[0] == true)
				printOnScreen(SW - 220, SH - 30, "Hai preso la barra n 1!!!", GREEN);
			if (objectFound[1] == true)
				printOnScreen(SW - 220, SH - 60, "Hai preso la barra n 2!!!", GREEN);
			if (objectFound[2] == true)
				printOnScreen(SW - 220, SH - 90, "Hai preso la barra n 3!!!", GREEN);
		}
		else {
			scene_list = 0;
			level = 2;
		}
			

	}
	printOnScreen(10, SH - 30, "LIVELLO 1", GREEN);
	printOnScreen(10, 10, "Prendi le tre barre d'Uranio VERDI per passare al livello 2!", GREEN);

	glutSwapBuffers();
}
//-----------------------------------------------------------------------------
void display(void) {
	if (level == 1)
		level1();
	else level2();
}
// ----------------------------------------------------------------------------


int InitGL()					 // All Setup For OpenGL goes here
{
	
	if (!piazza->LoadGLTextures() || !deloreanScene->LoadGLTextures() || !barraUranio1->LoadGLTextures() || !barraUranio2->LoadGLTextures() || !barraUranio3->LoadGLTextures())
	{
		fprintf(stderr, "Error loading gl textures.\n");
		return FALSE;
	}

	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	glClearColor(0.369, 0.807, 1, 0.8); //sky color
	glClearDepth(1.0f);				// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);		// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);			// The Type Of Depth Test To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculation

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);    // Uses default lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glEnable(GL_NORMALIZE);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHT1);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	return TRUE;					// Initialization Went OK
}

//--------------------------------------------------------------

void printValues(int value) {
	switch (value) {
	case 1: //print variables on the console
		printf("lx = %f\n", camera->getSightX());
		printf("ly = %f\n", camera->getSightY());
		printf("lz = %f\n", camera->getSightZ());
		printf("x = %f\n", camera->getX());
		printf("y = %f\n", camera->getY());
		printf("z = %f\n", camera->getZ());
		printf("psi = %f\n", camera->getPsi()*180/M_PI);
		printf("theta = %f\n", camera->getTheta()*180/M_PI);
		firstTimeCursor = true;
		break;
	default:
		break;
		return;
	}
}
//-----------------------------------------------------------------------------

bool rightPosition(float x, float y, float z) {
	//check inside the square

	if (!piazza->inside(x,y,z)) return false;

	//check inside the municipality
	if (inside_box(municipio, x,y,z)) return false;
	

	//controllo inside bank sidewalk
	if (inside_box(marciapiedeBanca, x,y,z)) return false;
	//if (x<-3.3 &&(z<6 && z>-4.6)) return false;

	//check sidewalk behind the bank
	if (inside_box(marciapiedeFondo,x,y,z)) return false;
	//if (z < -6) return false;

	//check Lou sidewalk
	if (inside_box(marciapiedeLou,x,y,z)) return false;

	//check inside texaco
	if (inside_box(texaco, x, y, z)) return false;
	

	//check schwinn bike -> ok
	float x1 = 3.9, z1 = 5.2, x2 = 5.2, z2=4.44;
	float m = (z2 - z1) / (x2 - x1);
	float q = (x2*z1 - x1 * z2) / (x2-x1);
	if (z > m*x + q) return false;

	// check cinema sidewalk
	if (inside_box(marciapiedeCinema, x,y,z)) return false;

	// check delorean
	if (inside_box(delorean, x, y, z)) return false;
	

	return true;
}
//keyboard function
void processNormalKeys(unsigned char key, int xx, int yy)
{
	float try_x, try_z;
	float fraction = 0.1f;
	unsigned char character = tolower(key);

	switch (character) {
	case 'q':
		camera->goUp();
		glutPostRedisplay();
		break;
	case 'z':
		camera->goDown();
		glutPostRedisplay();
		break;
	case 'w':
		if (lastKeyPressed == NO_BUTTON)
			lastKeyPressed = W;
		camera->tryForward(&try_x, &try_z);
		if (rightPosition(try_x, camera->getY(), try_z)) {
			camera->goForward();
		}
		glutPostRedisplay();
		break;
	case 's':
		if (lastKeyPressed == W)
			lastKeyPressed = S;
		camera->tryBackward(&try_x, &try_z);
		if (rightPosition(try_x, camera->getY(), try_z)) {
			camera->goBackward();
		}
		glutPostRedisplay();
		break;
	case 'a':
		if (lastKeyPressed == S)
			lastKeyPressed = A;
		camera->tryLeft(&try_x, &try_z);
		if (rightPosition(try_x, camera->getY(), try_z)) {
			camera->goLeft();
		}
		glutPostRedisplay();
		break;
	case 'd':
		if (lastKeyPressed == A)
			lastKeyPressed = D;
		camera->tryRight(&try_x, &try_z);
		if (rightPosition(try_x, camera->getY(), try_z)) {
			camera->goRight();
		}
		glutPostRedisplay();
		break;
	case 27:
		exit(1);
	default:
		break;
	}
}
bool inside_box(Box box, float try_x, float try_y, float try_z) {
	if ((try_x > box.min_x && try_x < box.max_x) && (try_y >box.min_y &&  try_y < box.max_y ) && (try_z > box.min_z && try_z < box.max_z))
		return true;
	return false;
}
bool select() {
	//temps for imaginary ray
	float z_cam, x_cam, y_cam, fraction = 0.1;
	for (int i = 0; i < 100; i++) {
		float r = i * fraction;
		camera->tryForward(&x_cam, &z_cam, &y_cam, r);
		//first level, I must find bars 1,2,3
		if (level == 1) {
			if (objectFound[0] == false || objectFound[1] == false || objectFound[2] == false) {
				if (barraUranio1->inside(x_cam, y_cam, z_cam)) {
					objectFound[0] = true;
					return true;
				}
				if (barraUranio2->inside(x_cam, y_cam, z_cam)) {
					objectFound[1] = true;
					return true;
				}
				if (barraUranio3->inside(x_cam, y_cam, z_cam)) {
					objectFound[2] = true;
					return true;
				}
			}
			//go to level 2
			else
				level = 2;
		} 
		else {
			//level 2, I must find bars 4,5,6
			if (objectFound[3] == false || objectFound[4] == false || objectFound[5] == false) {
				if (barraUranio4->inside(x_cam, y_cam, z_cam)) {
					objectFound[3] = true;
					return true;
				}
				if (barraUranio5->inside(x_cam, y_cam, z_cam)) {
					objectFound[4] = true;
					return true;
				}
				if (barraUranio6->inside(x_cam, y_cam, z_cam)) {
					objectFound[5] = true;
					return true;
				}
			}
			else {
				//if I find the three bars, I check if I am clicking to the delorean
				if (inside_box(delorean, x_cam, y_cam, z_cam)) {
					//animation
					departure = true;
					delta = glutGet(GLUT_ELAPSED_TIME) - time;
					return true;
				}
			}
		}
	}
	return false;
	
}
//-----------------------------------------------------------------------------
void mouseMovement(int x, int y) {
	if (firstTimeCursor) {
		camera->setMouse(x, y);
		firstTimeCursor = false;
	}
	if(lastKeyPressed == D)
		lastKeyPressed = MOUSE;
	// update psi and theta
	camera->rotation(x, y);
}
//------------------------------------------------------------------------------
void mouseClick(int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON)&& (state == GLUT_DOWN))
	{
		if (select()) printf("Object catched!");
	}
}
// ----------------------------------------------------------------------------
int main(int argc, char **argv)
{
	struct aiLogStream stream;
	
	glutInitWindowSize(SW, SH);
	glutInitWindowPosition(100, 100);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutCreateWindow("Ritorno al futuro");
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(myreshape);
	glutMouseFunc(mouseClick);

	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It will be active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.

	stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT, NULL);
	aiAttachLogStream(&stream);

	// ... exactly the same, but this stream will now write the
	// log file to assimp_log.txt
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE, "assimp_log.txt");
	aiAttachLogStream(&stream);

	//scene object and camera istantiation
	piazza = new Scene(pathscene_1.c_str());
	deloreanScene = new Scene(path_delorean.c_str());
	barraUranio1 = new Scene(path_barra1.c_str());
	barraUranio2 = new Scene(path_barra2.c_str());
	barraUranio3 = new Scene(path_barra3.c_str());
	barraUranio4 = new Scene(path_barra4.c_str());
	barraUranio5 = new Scene(path_barra5.c_str());
	barraUranio6 = new Scene(path_barra6.c_str());
	camera = new Camera(0, 0.3, 0, M_PI / 2, M_PI / 2, 0.05, 0.1, SW, SH);

	if (!InitGL())
	{
		fprintf(stderr, "Initialization failed");
		return FALSE;
	}

	//mouse function
	glutPassiveMotionFunc(mouseMovement);
	//keyboard function
	glutKeyboardFunc(processNormalKeys);

	//menu function for debug
	/*menu = glutCreateMenu(printValues);
	glutAddMenuEntry("Print camera variables", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);*/

	PlaySound(TEXT("BackToTheFuture.wav"), NULL, SND_FILENAME|SND_ASYNC|SND_LOOP);
	time = glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	float delta = glutGet(GLUT_ELAPSED_TIME) - time;
	PlaySound(NULL, 0,0);

	// memory release
	// aiReleaseImport called inside the destructor
	piazza->~Scene();
	deloreanScene->~Scene();
	barraUranio1->~Scene();
	barraUranio2->~Scene();
	barraUranio3->~Scene();
	camera->~Camera();

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();

	return 0;
}
