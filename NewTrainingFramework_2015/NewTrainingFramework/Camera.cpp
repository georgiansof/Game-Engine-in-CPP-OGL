#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Camera.h"
#include <iostream>

void Camera :: setWorldMatrix(Matrix worldMatrix) {this->worldMatrix = worldMatrix;}
Matrix Camera :: getWorldMatrix() {return worldMatrix;}

Camera::Camera() {
	type = FIRST_PERSON;
	position = Vector3(0,0,-1);
	target = Vector3(0, 0, 1);
	up = Vector3(0, 1, 0);

	moveSpeed = 100;
	rotateSpeed = 1;

	fov = 3.141592f/2.0f; /// 90 de grade

	_near = 0.1f;
	_far = 10000.0f;

	deltaTime = 0;

	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();

	// worldMatrix
	// viewMatrix
	this->updateWorldView();

}

void Camera::set_deltaTime(GLfloat deltaTime) { this->deltaTime = deltaTime; }

void Camera :: moveOx(int dir) {
	Vector3 forward, deplasare;
	forward = xAxis * dir;
	deplasare = forward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;
	updateWorldView();
}

void Camera::moveOy(int dir) {
	Vector3 forward, deplasare;
	forward = yAxis * dir;
	deplasare = forward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;
	updateWorldView();
}

void Camera::moveOz(int dir) {
	Vector3 forward, deplasare;
	forward = - zAxis * dir;
	deplasare = forward * moveSpeed * deltaTime;
	position += deplasare;
	target += deplasare;
	updateWorldView();
}

void Camera::rotateOx(int dir) {
	Matrix m_rotateOx;
	m_rotateOx.SetRotationX(rotateSpeed * deltaTime * dir);
	
	Vector4 localUp(0,1,0,0.0f);
	Vector4 rotatedLocalUp = localUp * m_rotateOx;
	rotatedLocalUp = rotatedLocalUp * worldMatrix;
	up.x = rotatedLocalUp.x;
	up.y = rotatedLocalUp.y;
	up.z = rotatedLocalUp.z;
	up = up.Normalize();

	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * m_rotateOx;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target.x = target4.x;
	target.y = target4.y;
	target.z = target4.z;
	updateWorldView();
}

void Camera::rotateOy(int dir) {
	Matrix m_rotateOy;
	m_rotateOy.SetRotationY(rotateSpeed * deltaTime * dir);
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(),1.0f);
	Vector4 rotatedTarget = localTarget * m_rotateOy;
	Vector4 target4 = rotatedTarget * worldMatrix;
	target.x = target4.x;
	target.y = target4.y;
	target.z = target4.z;
	updateWorldView();
}

void Camera::rotateOz(int dir) {
	Matrix m_rotateOz;
	m_rotateOz.SetRotationZ(rotateSpeed * deltaTime * dir);

	Vector4 localUp(0, 1, 0, 0.0f);
	Vector4 rotatedLocalUp = localUp * m_rotateOz;
	rotatedLocalUp = rotatedLocalUp * worldMatrix;
	up.x = rotatedLocalUp.x;
	up.y = rotatedLocalUp.y;
	up.z = rotatedLocalUp.z;
	up = up.Normalize();

	updateWorldView();
}

void Camera::updateWorldView() {
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	xAxis = zAxis.Cross(yAxis).Normalize();
	Matrix R, T;
	R.m[0][0] = xAxis.x; R.m[0][1] = xAxis.y; R.m[0][2] = xAxis.z; R.m[0][3] = 0;
	R.m[1][0] = yAxis.x; R.m[1][1] = yAxis.y; R.m[1][2] = yAxis.z; R.m[1][3] = 0;
	R.m[2][0] = zAxis.x; R.m[2][1] = zAxis.y; R.m[2][2] = zAxis.z; R.m[2][3] = 0;
	R.m[3][0] = 0;       R.m[3][1] = 0;       R.m[3][2] = 0;       R.m[3][3] = 1;
	
	T.SetTranslation(position);
	worldMatrix = R * T;

	Matrix T_1,Rt = R.Transpose();
	T_1.SetTranslation(-position);
	viewMatrix = T_1 * Rt;
}

Matrix Camera::get_viewMatrix() {return viewMatrix;}

GLfloat Camera::get_fov() { return fov; }
GLfloat Camera::get_far() { return _far; }
GLfloat Camera::get_near() { return _near; }

Vector3 Camera::getPosition() {
	return this->position;
}