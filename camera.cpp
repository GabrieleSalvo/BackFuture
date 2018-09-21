
#include <stdlib.h>
#include <GL/glut.h>

#include "camera.h"

#define M_PI 3.14159265358979323846

Camera::Camera(float p_X, float p_Y, float p_Z, float p_psi, float p_theta, float p_rotation_speed, float p_translation_speed, int p_window_width, int p_window_height)
{
	x = p_X;
	y=p_Y;
	z = p_Z;
	mouse_x=p_window_width / 2;
	mouse_y = p_window_height / 2;
	psi=p_psi;
	theta = p_theta;
	rotation_speed = p_rotation_speed;
	translation_speed=p_translation_speed;
	time = 0;

}

Camera::~Camera()
{
}

// new values calculated with the mouse difference
void Camera::rotation(int x, int y) {
	
	theta -= static_cast<float>(x - mouse_x) * rotation_speed;
	if (theta < 0)
		theta = M_PI * 2;
	else if (theta > M_PI * 2)
		theta = 0;
	psi += static_cast<float>(y - mouse_y) * rotation_speed;
	if (psi <= 0.1)            psi = 0.1;
	else if (psi >= 2) psi = 2;
	mouse_x = x;
	mouse_y = y;
}
// trace a sphere ray, where r = translation_speed
//update x and z with the point sphere projection on x and z

void Camera::goForward()
{
	x += sin(theta)*sin(psi) * translation_speed;
	z += cos(theta)*sin(psi) * translation_speed;
}

void Camera::tryForward(float * try_x, float * try_z, float *try_y, float r)
{
	if (try_y != NULL) {
		float prev_tr = translation_speed;
		translation_speed = 1;
		*try_x = x + r * sin(theta)*sin(psi) * translation_speed;
		*try_z = z + r * cos(theta)*sin(psi) * translation_speed;
		*try_y = y + r * cos(psi)*translation_speed;
		translation_speed = prev_tr;
	}
	else {
		*try_x = x + r * sin(theta)*sin(psi) * translation_speed;
		*try_z = z + r * cos(theta)*sin(psi) * translation_speed;
	}	
}

void Camera::goBackward()
{
	x -= sin(theta)*sin(psi) * translation_speed;
	z -= cos(theta)*sin(psi) * translation_speed;
}

void Camera::tryBackward(float * try_x, float* try_z)
{
	*try_x = x- sin(theta)*sin(psi) * translation_speed;
	*try_z = z- cos(theta)*sin(psi) * translation_speed;
}

void Camera::goLeft()
{
	x += sin(theta + M_PI / 2)*sin(psi) * translation_speed;
	z += cos(theta + M_PI / 2)*sin(psi) * translation_speed;
}

void Camera::tryLeft(float * try_x, float * try_z)
{
	*try_x = x+ sin(theta + M_PI / 2)*sin(psi) * translation_speed;
	*try_z = z+ cos(theta + M_PI / 2)*sin(psi) * translation_speed;
}

void Camera::goRight()
{
	x -= sin(theta + M_PI / 2)*sin(psi) * translation_speed;
	z -= cos(theta + M_PI / 2)*sin(psi) * translation_speed;
}

void Camera::tryRight(float * try_x, float * try_z)
{
	*try_x = x- sin(theta + M_PI / 2)*sin(psi) * translation_speed;
	*try_z = z- cos(theta + M_PI / 2)*sin(psi) * translation_speed;
}

void Camera::goUp()
{
	y += 0.1;
}


void Camera::goDown()
{
	y -= 0.1;
}

int   Camera::getMouseX() {return mouse_x; }
int   Camera::getMouseY() { return mouse_y; }
float Camera::getSightX() { return x + sin(theta)*sin(psi); }
float Camera::getSightY() { return y + cos(psi); }
float Camera::getSightZ() { return z + cos(theta)*sin(psi); }
float Camera::getX() { return x; }
float Camera::getY() { return y; }
float Camera::getZ() { return z; }
float Camera::getPsi(){ return psi; }
float Camera::getTheta(){ return theta; }
void Camera::setMouse(int x, int y) { mouse_x = x; mouse_y = y; }
void Camera::setX(float p_X) { x = p_X; }
void Camera::setY(float p_Y) { y = p_Y; }
void Camera::setZ(float p_Z) { z = p_Z; }

void Camera::setPsi(float psiNuovo)
{
	psi = psiNuovo;
}

void Camera::setTheta(float thetaNuovo)
{
	theta = thetaNuovo;
}
