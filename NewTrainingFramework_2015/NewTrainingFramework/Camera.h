#pragma once
#include "stdafx.h"
#include "../Utilities/utilities.h"

class Camera
{
	Vector3 target;
	Vector3 up;
	Vector3 xAxis, yAxis, zAxis;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat _near;
	GLfloat _far;
	GLfloat fov;
	GLfloat deltaTime;
	Matrix worldMatrix, viewMatrix;

public:
	Vector3 position;

	void set_deltaTime(GLfloat);
	void setWorldMatrix(Matrix);
	Matrix getWorldMatrix();
	Matrix get_viewMatrix();
	GLfloat get_fov();
	GLfloat get_near();
	GLfloat get_far();

	void moveOx(int);
	void moveOy(int);
	void moveOz(int);
	void rotateOx(int);
	void rotateOy(int);
	void rotateOz(int);
	void updateWorldView();
	Camera();
};