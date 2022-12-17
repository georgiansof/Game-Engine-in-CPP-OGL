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


enum {SCREEN_LEFT,SCREEN_RIGHT} screenSide;

GLuint vboId;
Shaders myShaders;

GLuint lvboId;
Shaders whiteline;

GLuint model_vboId;
GLuint indices_vboId;
Shaders model_shader;

Camera cam;
Matrix mPerspective;
float unghi;

float prag=0.016f; /// 60 fps
float acumm;

bool rotating = false;

std::vector<ModelVertex> mdl;
std::vector<Vector3_uhint> indices;

int Init(ESContext* esContext)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Vertex whitelineData[2];

	whitelineData[0].pos.x = 0.0f; whitelineData[0].pos.y = 1.0f; whitelineData[0].pos.z = 0.0f;
	whitelineData[1].pos.x = 0.0f; whitelineData[1].pos.y = -1.0f; whitelineData[1].pos.z = 0.0f;

	Vertex verticesData[12];

	// left

	verticesData[0].pos.x = -0.5f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[1].pos.x = 0.0f;  verticesData[1].pos.y = 0.0f;  verticesData[1].pos.z = 0.0f;
	verticesData[2].pos.x = -0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;

	// right

	verticesData[3].pos.x = 0.5f;  verticesData[3].pos.y = 0.5f;  verticesData[3].pos.z = 0.0f;
	verticesData[4].pos.x = 0.0f;  verticesData[4].pos.y = 0.0f;  verticesData[4].pos.z = 0.0f;
	verticesData[5].pos.x = 0.5f;  verticesData[5].pos.y = -0.5f;  verticesData[5].pos.z = 0.0f;

	// down

	verticesData[6].pos.x = 0.5f;  verticesData[6].pos.y = -0.5f;  verticesData[6].pos.z = 0.0f;
	verticesData[7].pos.x = 0.0f;  verticesData[7].pos.y = 0.0f;  verticesData[7].pos.z = 0.0f;
	verticesData[8].pos.x = -0.5f;  verticesData[8].pos.y = -0.5f;  verticesData[8].pos.z = 0.0f;

	// up

	verticesData[9].pos.x = -0.5f;  verticesData[9].pos.y = 0.5f;  verticesData[9].pos.z = 0.0f;
	verticesData[10].pos.x = 0.0f;  verticesData[10].pos.y = 0.0f;  verticesData[10].pos.z = 0.0f;
	verticesData[11].pos.x = 0.5f;  verticesData[11].pos.y = 0.5f;  verticesData[11].pos.z = 0.0f;

	// culori

	//left

	verticesData[0].color.r = 1.0f;  verticesData[0].color.g = 0.0f;  verticesData[0].color.b = 0.0f;
	verticesData[1].color.r = 0.0f;  verticesData[1].color.g = 0.0f;  verticesData[1].color.b = 0.0f;
	verticesData[2].color.r = 0.0f;  verticesData[2].color.g = 1.0f;  verticesData[2].color.b = 0.0f;

	// right

	verticesData[3].color.r = 0.0f; verticesData[3].color.g = 0.0f; verticesData[3].color.b = 1.0f;
	verticesData[4].color.r = 0.0f; verticesData[4].color.g = 0.0f; verticesData[4].color.b = 0.0f;
	verticesData[5].color.r = 1.0f; verticesData[5].color.g = 1.0f; verticesData[5].color.b = 0.0f;

	// down

	verticesData[6].color.r = 1.0f; verticesData[6].color.g = 1.0f; verticesData[6].color.b = 0.0f;
	verticesData[7].color.r = 0.0f; verticesData[7].color.g = 0.0f; verticesData[7].color.b = 0.0f;
	verticesData[8].color.r = 0.0f; verticesData[8].color.g = 1.0f; verticesData[8].color.b = 0.0f;

	// up

	verticesData[9].color.r = 1.0f; verticesData[9].color.g = 0.0f; verticesData[9].color.b = 0.0f;
	verticesData[10].color.r = 0.0f; verticesData[10].color.g = 0.0f; verticesData[10].color.b = 0.0f;
	verticesData[11].color.r = 0.0f; verticesData[11].color.g = 0.0f; verticesData[11].color.b = 1.0f;

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesData), verticesData, GL_STATIC_DRAW);
	glGenBuffers(1, &lvboId);
	glBindBuffer(GL_ARRAY_BUFFER, lvboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(whitelineData), whitelineData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	/***********************/

	//creation of shaders and program 
	int ret = myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs");
	int ret2 = whiteline.Init("../Resources/Shaders/WhitelineShaderVS.vs", "../Resources/Shaders/WhitelineShaderFS.fs");
	int retmdl = model_shader.Init("../Resources/Shaders/ModelShaderVS.vs", "../Resources/Shaders/ModelShaderFS.fs");
	ret = ret || ret2 || retmdl;
	return ret;

}

void Draw ( ESContext *esContext )
{
	Matrix mTransformare;
	unghi += 0.01f;
	mTransformare.SetRotationZ(unghi);
	Matrix m_cam = cam.get_viewMatrix();
	Matrix mvp = m_cam * mPerspective;
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(myShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	if(myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*) sizeof(Vector3));
	}
	if (myShaders.matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (GLfloat*) mTransformare.m);
	}
	if (myShaders.cam_matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.cam_matrixUniform, 1, GL_FALSE, (GLfloat*)mvp.m);
	}
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 3, 3);
	glDrawArrays(GL_TRIANGLES, 6, 3);
	glDrawArrays(GL_TRIANGLES, 9, 3);

	glUseProgram(whiteline.program);
	glBindBuffer(GL_ARRAY_BUFFER, lvboId);
	if (whiteline.positionAttribute != -1) {
		glEnableVertexAttribArray(whiteline.positionAttribute);
		glVertexAttribPointer(whiteline.positionAttribute,2 , GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (model_shader.positionAttribute != -1) {
		glEnableVertexAttribArray(model_shader.positionAttribute);
		glVertexAttribPointer(model_shader.positionAttribute,3 ,GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);
	}
	if (model_shader.colorAttribute != -1) {
		glEnableVertexAttribArray(model_shader.colorAttribute);
		glVertexAttribPointer(model_shader.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*) sizeof(Vector3));
	}
	if (model_shader.matrixUniform != -1) {
		glUniformMatrix4fv(model_shader.matrixUniform, 1, GL_FALSE, (GLfloat*) mTransformare.m);
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
	glBindBuffer(GL_ARRAY_BUFFER, model_vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_vboId);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);

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
		unghi += 0.01f;
		if (rotating)
			if (screenSide == SCREEN_LEFT) 
				cam.rotateOy(-1);
			else 
				cam.rotateOy(1);
		/// render
		//cam.rotateOy(1);
	}
}

void MouseEvent(ESContext* esContext, MouseButton button, MouseEventType eventType, float mouseX, float mouseY) {		
	switch (eventType)
	{
	case M_CLICK:
		rotating = true;
		screenSide = mouseX < (float) Globals::screenWidth / 2 ? SCREEN_LEFT : SCREEN_RIGHT;
		break;
	case M_RELEASE:
		rotating = false;
		break;
	case M_MOVE:
		screenSide = mouseX < (float)Globals::screenWidth / 2 ? SCREEN_LEFT : SCREEN_RIGHT;
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
			cam.moveOy(-1);
			break;
		case 'W':
			cam.moveOy(1);
			break;
		case '1':
			cam.rotateOx(1);
			break;
		case '2':
			cam.rotateOy(1);
			break;
		case '3':
			cam.rotateOz(1);
			break;
		case 'Q':
			cam.moveOz(1);
			break;
		case 'E':
			cam.moveOz(-1);
		}

}

void CleanUp()
{
	glDeleteBuffers(1, &vboId);
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

	Parser::Parse(model_folder + "\\" + model_file, mdl, indices);

	/*int i = 0;
	for (ModelVertex x : mdl)
		printf("Vertex [%d]:\npos > %f %f %f\nnorm > %f %f %f\nbinorm > %f %f %f\ntgt > %f %f %f\nuv > %f %f\n\n",i++, x.pos.x, x.pos.y, x.pos.z, x.norm.x, x.norm.y, x.norm.z, x.binorm.x, x.binorm.y, x.binorm.z, x.tgt.x, x.tgt.y, x.tgt.z, x.uv.x, x.uv.y);
	i = 0;
	for (Vector3uh_int x : indices)
		printf("Index [%d]: %d, %d, %d\n", i++, x.x, x.y, x.z);

	*/

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

