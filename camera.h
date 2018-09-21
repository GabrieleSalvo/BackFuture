#pragma once

#include <cmath>
#include <map>

class Camera {

public:

	Camera(float p_X, float p_Y, float p_Z, float p_psi, float p_theta, float p_rotation_speed, float p_translation_speed, int p_window_width, int p_window_height);
	~Camera();

	int   getMouseX();
	int   getMouseY();
	float getSightX();
	float getSightY();
	float getSightZ();
	float getX();
	float getY();
	float getZ();
	float getPsi();
	float getTheta();
	void setMouse(int x, int y);
	void setX(float p_X);
	void setY(float p_Y);
	void setZ(float p_Z);
	void setPsi(float psiNuovo);
	void setTheta(float thetaNuovo);

	void rotation(int, int);
	void goForward();
	void tryForward(float* try_x, float* try_z, float *try_y = NULL, float r=1);
	void goBackward();
	void tryBackward(float* try_x, float* try_z);
	void goLeft();
	void tryLeft(float* try_x, float* try_z);
	void goRight();
	void tryRight(float* try_x, float* try_z);
	void goUp();
	void goDown();

private:
	float    x;                                     /* sphere center x axis */
	float    y;                                     /* sphere center y axis */
	float    z;                                     /* sphere center z axis */
	int      mouse_x;                               /* mouse x position */
	int      mouse_y;                               /* mouse y position */
	float    psi;                                   /* psi angle in spherical coordinates */
	float    theta;                                 /* theta angle in spherical coordinates */
	float    rotation_speed;                        /* equivalent to mouse sensibility */
	float    translation_speed;                     /* equivalent to keyboard sensibility */
	int      time;                                  /* to keep track of the time between two function calls */

};

