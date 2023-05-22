#include "stdafx.h"
#include "Fire.h"

using namespace std;

Fire::Fire() : SceneObject() {
	dispMax = 0;
	timer = 0;
}

Fire::Fire(SceneObject* obj, float dispMax)
		: SceneObject(obj->name,obj->position,obj->rotation,obj->scale,obj->depthTest, obj->modelId, obj->shaderId, obj->textureIds, obj->type, obj->debug) {
	this->dispMax = dispMax;
	timer = 0;
	this->id = obj->id;
	this->wired = obj->wired;
	this->generatedModel = obj->generatedModel;
	this->followingCamera = obj->followingCamera;
	this->color = obj->color;
	this->grid_dimension = obj->grid_dimension;
	this->grid_height = obj->grid_height;
	this->grid_width = obj->grid_width;
}

void Fire::UpdateTimer(float delta) {
	this->timer += delta;
	timer -= int(timer);
}

float Fire::get_dispMax() {
	return dispMax;
}

float Fire::get_time() {
	return timer;
}