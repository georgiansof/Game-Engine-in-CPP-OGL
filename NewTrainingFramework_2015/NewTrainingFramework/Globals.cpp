#include "stdafx.h"
#include "Globals.h"

const float math::pi = 3.141592f;

float math::toRadians(float angle) {
	return angle / 180.0 * pi;
}