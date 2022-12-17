// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

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

#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include


GLuint model_vboId;
GLuint indices_vboId;
Shaders model_shader;

Camera cam;
Matrix mPerspective;

float prag=0.016f; /// 60 fps
float acumm;

bool rotating = false;

std::vector<ModelVertex> mdl;
std::vector<Vector3_uhint> indices;

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	glGenBuffers(1, &indices_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vboId);

	int n;
	n = indices.size();
	Vector3_uhint(*indices_data) = new Vector3_uhint[n];
	for (int i = 0; i < n; ++i)
		indices_data[i] = indices[i];
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector3_uhint) * n, indices_data, GL_STATIC_DRAW);
	delete indices_data;

	glGenBuffers(1, &model_vboId);
	glBindBuffer(GL_ARRAY_BUFFER, model_vboId);

	n = mdl.size();
	ModelVertex (*mdl_data) = new ModelVertex[n];
	for (int i = 0; i < n; ++i)
		mdl_data[i] = mdl[i];

	glBufferData(GL_ARRAY_BUFFER, sizeof(ModelVertex) * n, mdl_data , GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete mdl_data;

	//creation of shaders and program 
	int retmdl = model_shader.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	int ret = !!retmdl;
	return ret;

}

void Draw ( ESContext *esContext )
{
	Matrix m_cam = cam.get_viewMatrix();
	Matrix mvp = m_cam * mPerspective;
	glClear(GL_COLOR_BUFFER_BIT);

	////////
	glUseProgram(model_shader.program);
	glBindBuffer(GL_ARRAY_BUFFER, model_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vboId);

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
		glVertexAttribPointer(model_shader.uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex),(GLvoid*) (5*sizeof(Vector3)));
	}

	glDrawElements(GL_TRIANGLES, 3*indices.size(), GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	eglSwapBuffers ( esContext->eglDisplay, esContext->eglSurface );
	
}

void Update ( ESContext *esContext, float deltaTime )
{
	acumm += deltaTime;
	if (acumm >= prag) {
		cam.set_deltaTime(acumm);
		acumm = 0;
		//if (rotating)
		//	if (screenSide == SCREEN_LEFT) 
		//		cam.rotateOy(-1);
		//	else 
		//		cam.rotateOy(1);
		/// render
		//cam.rotateOy(1);
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
	if(bIsPressed)
		switch (key)
		{
		case 'A':
			cam.moveOx(-1);
			break;
		case 'D':
			cam.moveOx(1);
			break;
		case 'S':
			cam.moveOz(1);
			break;
		case 'W':
			cam.moveOz(-1);
			break;
		case 'Q':
			cam.moveOy(1);
			break;
		case 'E':
			cam.moveOy(-1);
			break;
		case '1':
			cam.rotateOz(1);
			break;
		case '3':
			cam.rotateOz(-1);
			break;
		case '2':
			cam.rotateOx(-1);
			break;
		case 'X':
			cam.rotateOx(1);
			break;
		case 'Z':
			cam.rotateOy(1);
			break;
		case 'C':
			cam.rotateOy(-1);
			break;
		}

}

void CleanUp()
{
	glDeleteBuffers(1, &model_vboId);
}

int _tmain(int argc, _TCHAR* argv[])
{
	//identifying memory leaks
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 

	ESContext esContext;

    esInitContext ( &esContext );

	esCreateWindow ( &esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	
	mPerspective = mPerspective.SetPerspective(cam.get_fov(),(GLfloat) Globals::screenWidth / Globals::screenHeight, cam.get_near(), cam.get_far());

	///DEBUG PARSARE MODEL

	std::string model_folder = "..\\..\\NewResourcesPacket\\Models";
	std::string model_file = "Croco.nfg";

	NFG_Parser::Parse(model_folder + "\\" + model_file, mdl, indices);


	/*int i = 0;
	for (ModelVertex x : mdl)
		printf("Vertex [%d]:\npos > %f %f %f\nnorm > %f %f %f\nbinorm > %f %f %f\ntgt > %f %f %f\nuv > %f %f\n\n",i++, x.pos.x, x.pos.y, x.pos.z, x.norm.x, x.norm.y, x.norm.z, x.binorm.x, x.binorm.y, x.binorm.z, x.tgt.x, x.tgt.y, x.tgt.z, x.uv.x, x.uv.y);
	i = 0;
	for (auto x : indices)
		printf("Index [%d]: %d, %d, %d\n", i++, x.x, x.y, x.z);*/

	

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

