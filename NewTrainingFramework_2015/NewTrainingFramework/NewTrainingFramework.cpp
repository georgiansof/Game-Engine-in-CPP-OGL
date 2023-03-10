// NewTrainingFramework.cpp : Defines the entry point for the console application.
/** 
	ISSUES: wired?, fullscreen
	target nu afecteaza camera
	glclearcolor alpha nu merge?
**/

/***
HIGHLIGHTS:
	more actions per key or more keys per action

***/

#include "stdafx.h"
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <iostream>
#include "Model.h"
#include <vector>
#include <string>
#include <unordered_map>

/*#include "../Utilities/rapidxml.hpp"
#include "../Utilities/rapidxml_iterators.hpp"
#include "../Utilities/rapidxml_print.hpp"
#include "../Utilities/rapidxml_utils.hpp"*/

#include "Managers.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include

GLuint model_vboId;
GLuint texture_vboId;
GLuint indices_vboId;
Shaders model_shader;

Camera *activeCamera;
Matrix mPerspective;
unsigned char* pixels_array;

float prag=0.016f; /// 60 fps
float acumm;

bool rotating = false;

std::vector<ModelVertex> mdl;
std::vector<Vector3_uhint> indices;

int Init(ESContext* esContext)
{
	Vector4 bgcolor = SceneManager::getInstance()->getBgColor();
	glClearColor(bgcolor.x, bgcolor.y, bgcolor.z, bgcolor.w);
	glEnable(GL_DEPTH_TEST);
	glGenBuffers(1, &indices_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vboId);

	int n;
	n = indices.size();
	Vector3_uhint(*indices_data) = new Vector3_uhint[n];
	for (int i = 0; i < n; ++i)
		indices_data[i] = indices[i];
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3_uhint) * n, indices_data, GL_STATIC_DRAW);
	delete[] indices_data;

	glGenBuffers(1, &model_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, model_vboId);

	n = mdl.size();
	ModelVertex (*mdl_data) = new ModelVertex[n];
	for (int i = 0; i < n; ++i) {
		mdl_data[i] = mdl[i];
		mdl_data[i].pos.z = mdl[i].pos.z + 200;
		mdl_data[i].pos.x = mdl[i].pos.x + 75;
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex) * n, mdl_data , GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenTextures(1, &texture_vboId);
	glBindTexture(GL_TEXTURE_2D, texture_vboId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int texture_width=0, texture_height=0, bpp=0;
	pixels_array = (unsigned char*) LoadTGA("..\\..\\NewResourcesPacket\\Textures\\Croco.tga", &texture_width, &texture_height, &bpp);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_array);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE0, 0);

	delete mdl_data;

	//creation of shaders and program 
	int retmdl = model_shader.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	int ret = !!retmdl;
	return ret;

}

void Draw ( ESContext *esContext )
{
	Matrix m_cam = activeCamera->get_viewMatrix();
	Matrix mvp = m_cam * mPerspective;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	////////
	glUseProgram(model_shader.program);
	glBindBuffer(GL_ARRAY_BUFFER, model_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vboId);

	glBindTexture(GL_TEXTURE0, texture_vboId);

	if (model_shader.positionAttribute != -1) {
		glEnableVertexAttribArray(model_shader.positionAttribute);
		glVertexAttribPointer(model_shader.positionAttribute,3 ,GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);
	}
	if (model_shader.colorAttribute != -1) {
		glEnableVertexAttribArray(model_shader.colorAttribute);
		glVertexAttribPointer(model_shader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*) sizeof(Vector3));
	}
	if (model_shader.cam_matrixUniform != -1) {
		glUniformMatrix4fv(model_shader.cam_matrixUniform, 1, GL_FALSE,(GLfloat*) mvp.m);
	}
	if (model_shader.normalAttribute != -1) {
		glEnableVertexAttribArray(model_shader.normalAttribute);
		glVertexAttribPointer(model_shader.normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*) (2*sizeof(Vector3)));
	}
	if (model_shader.binormalAttribute != -1) {
		glEnableVertexAttribArray(model_shader.binormalAttribute);
		glVertexAttribPointer(model_shader.binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*) (3*sizeof(Vector3)));
	}
	if (model_shader.tangentAttribute != -1) {
		glEnableVertexAttribArray(model_shader.tangentAttribute);
		glVertexAttribPointer(model_shader.tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),(GLvoid*) (4*sizeof(Vector3)));
	}
	if (model_shader.uvAttribute != -1) {
		glEnableVertexAttribArray(model_shader.uvAttribute);
		glVertexAttribPointer(model_shader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex) ,(GLvoid*) (5*sizeof(Vector3)));
	}
	if (model_shader.textureUniform != -1)
		glUniform1i(model_shader.textureUniform, 0);

	glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE0, 0);
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
	
}

void Update ( ESContext *esContext, float deltaTime )
{
	acumm += deltaTime;
	if (acumm >= prag) {
		activeCamera->set_deltaTime(acumm);
		acumm = 0;
		//if (rotating)
		//	if (screenSide == SCREEN_LEFT) 
		//		activeCamera->rotateOy(-1);
		//	else 
		//		activeCamera->rotateOy(1);
		/// render
		//activeCamera->rotateOy(1);
	}
}

void MouseEvent(ESContext* esContext, MouseButton button, MouseEventType eventType, float mouseX, float mouseY) {		
	switch (eventType)
	{
	//case M_CLICK:
	//	rotating = true;
	//	screenSide = mouseX < (float) Globals::screenWidth / 2 ? SCREEN_LEFT : SCREEN_RIGHT;
	//	break;
	//case M_RELEASE:
	//	rotating = false;
	//	break;

	case M_MOVE:
		
		break;
	}
}

void Key ( ESContext *esContext, unsigned char key, bool bIsPressed)
{	
	std::unordered_multimap<unsigned char,SceneManager::action> controls = SceneManager::getInstance()->getKeysToControls();
	auto range = controls.equal_range(key);
	for (auto p = range.first; p != range.second; ++p) 
		if (p->first == key) {
			if (p->second == SceneManager::MOVE_CAMERA_POSITIVE_X)
				activeCamera->moveOx(1);
			if (p->second == SceneManager::MOVE_CAMERA_POSITIVE_Y)
				activeCamera->moveOy(1);
			if (p->second == SceneManager::MOVE_CAMERA_POSITIVE_Z)
				activeCamera->moveOz(1);
			if (p->second == SceneManager::MOVE_CAMERA_NEGATIVE_X)
				activeCamera->moveOx(-1);
			if (p->second == SceneManager::MOVE_CAMERA_NEGATIVE_Y)
				activeCamera->moveOy(-1);
			if (p->second == SceneManager::MOVE_CAMERA_NEGATIVE_Z)
				activeCamera->moveOz(-1);
			if (p->second == SceneManager::ROTATE_CAMERA_POSITIVE_X)
				activeCamera->rotateOx(1);
			if (p->second == SceneManager::ROTATE_CAMERA_POSITIVE_Y)
				activeCamera->rotateOy(1);
			if (p->second == SceneManager::ROTATE_CAMERA_POSITIVE_Z)
				activeCamera->rotateOz(1);
			if (p->second == SceneManager::ROTATE_CAMERA_NEGATIVE_X)
				activeCamera->rotateOx(-1);
			if (p->second == SceneManager::ROTATE_CAMERA_NEGATIVE_Y)
				activeCamera->rotateOy(-1);
			if (p->second == SceneManager::ROTATE_CAMERA_NEGATIVE_Z)
				activeCamera->rotateOz(-1);
		}
	
	/*if (bIsPressed)
		switch (key)
		{
		case 'A':
			activeCamera->moveOx(-1);
			break;
		case 'D':
			activeCamera->moveOx(1);
			break;
		case 'S':
			activeCamera->moveOz(1);
			break;
		case 'W':
			activeCamera->moveOz(-1);
			break;
		case 'Q':
			activeCamera->moveOy(1);
			break;
		case 'E':
			activeCamera->moveOy(-1);
			break;
		case '1':
			activeCamera->rotateOz(1);
			break;
		case '3':
			activeCamera->rotateOz(-1);
			break;
		case '2':
			activeCamera->rotateOx(-1);
			break;
		case 'X':
			activeCamera->rotateOx(1);
			break;
		case 'Z':
			activeCamera->rotateOy(1);
			break;
		case 'C':
			activeCamera->rotateOy(-1);
			break;
		}
		*/
}

void CleanUp()
{
	glDeleteBuffers(1, &model_vboId);
	glDeleteBuffers(1, &indices_vboId);
	glDeleteBuffers(1, &texture_vboId);
	delete[] pixels_array;
}


int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	///DEBUG PARSARE MODEL

	std::string model_folder = "..\\..\\NewResourcesPacket\\Models";
	std::string model_file = "Croco.nfg";

	NFG_Parser::Parse(model_folder + "\\" + model_file, mdl, indices);

	ResourceManager::getInstance()->ParseXML("resourceManager.XML");
	SceneManager::getInstance()->ParseXML("sceneManager.XML");
	activeCamera = SceneManager::getInstance()->getActiveCamera();

	mPerspective = mPerspective.SetPerspective(activeCamera->get_fov(), (GLfloat)Globals::screenWidth / Globals::screenHeight, activeCamera->get_near(), activeCamera->get_far());

	std::vector<SceneManager::action> zactions = SceneManager::getInstance()->getActionsFromKey('Z');
	std::cout << "Z actions:\n";
	for (auto x : zactions) {
		std::cout << x << ' ';
	}
	std::cout << '\n';


	//Texture* tmpTxt = ResourceManager::getInstance()->getTexture(4);
	//tmpTxt->Load();
	//TextureResource tmpTxtRes;
	//tmpTxt->GetTextureResource(&tmpTxtRes);
	

	/*int i = 0;
	for (ModelVertex x : mdl)
		printf("Vertex [%d]:\npos > %f %f %f\nnorm > %f %f %f\nbinorm > %f %f %f\ntgt > %f %f %f\nuv > %f %f\n\n",i++, x.pos.x, x.pos.y, x.pos.z, x.norm.x, x.norm.y, x.norm.z, x.binorm.x, x.binorm.y, x.binorm.z, x.tgt.x, x.tgt.y, x.tgt.z, x.uv.x, x.uv.y);
	i = 0;
	for (auto x : indices)
		printf("Index [%d]: %d, %d, %d\n", i++, x.x, x.y, x.z);*/

	
	ESContext esContext;
	esInitContext(&esContext);

	GLuint windowflags = (windowflags ^ windowflags) | ES_WINDOW_RGB | ES_WINDOW_DEPTH;
	esCreateWindow(&esContext, SceneManager::getInstance()->getGameName().c_str(), SceneManager::getInstance()->getScreenWidth(), SceneManager::getInstance()->getScreenHeight(), windowflags);

	if ( Init ( &esContext ) != 0 )
		return 0;

	esRegisterDrawFunc ( &esContext, Draw );
	esRegisterUpdateFunc ( &esContext, Update );
	esRegisterKeyFunc ( &esContext, Key);
	esRegisterMouseEventFunc( &esContext, MouseEvent);

	esMainLoop ( &esContext );

	//releasing OpenGL resources
	CleanUp();

	

	printf("Press any key...\n");
	_getch();

	
	return 0;
}

