#include <stdafx.h>
#include "Shaders.h"

int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if (vertexShader == 0)
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");
	normalAttribute = glGetAttribLocation(program, "a_normal");
	binormalAttribute = glGetAttribLocation(program, "a_binormal");
	tangentAttribute = glGetAttribLocation(program, "a_tangent");
	uvAttribute = glGetAttribLocation(program, "a_uv");
	colorAttribute = glGetAttribLocation(program, "a_color");
	cam_matrixUniform = glGetUniformLocation(program, "m_camera");
	textureUniform = glGetUniformLocation(program, "u_texture");
	return 0;
}

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}