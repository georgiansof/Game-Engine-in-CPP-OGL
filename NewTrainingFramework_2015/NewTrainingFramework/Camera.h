#pragma once
#include "stdafx.h"
#include <string>
#include "../Utilities/utilities.h"

class ResourceManager;

class Camera
{
	Vector3 position;
	
	Vector3 up;
	Vector3 xAxis, yAxis, zAxis;
	GLfloat moveSpeed;
	GLfloat rotateSpeed;
	GLfloat _near;
	GLfloat _far;
	GLfloat fov;
	GLfloat deltaTime;
	Matrix worldMatrix, viewMatrix;
	enum {FIRST_PERSON,THIRD_PERSON} type; /// default: first person
public:
	Vector3 target;
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
	Vector3 getPosition();
	Camera();
	friend class SceneManager;
};