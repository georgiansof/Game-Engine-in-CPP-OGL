#include <stdafx.h>
#include <iostream>
#include "Shaders.h"

using std::string;

int Shader::Init(int nr_textures) {
	if (fileVS[0] == NULL || fileFS[0] == NULL)
		return -1;
	Init(this->fileVS, this->fileFS, nr_textures);
}

int Shader::Init(char * fileVertexShader, char * fileFragmentShader, int nr_textures)
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
	uv2Attribute = glGetAttribLocation(program, "a_uv2");
	colorAttribute = glGetAttribLocation(program, "a_color");
	cam_matrixUniform = glGetUniformLocation(program, "m_camera");

	textureUniforms.resize(nr_textures);
	int i = 0;
	for(std::vector<GLint>::iterator it = textureUniforms.begin(); it!=textureUniforms.end(); ++it, ++i)
		*it = glGetUniformLocation(program, (string("u_texture_") + std::to_string(i)).c_str());

	fogColorUniform = glGetUniformLocation(program, "u_fogColor");
	camPosition = glGetUniformLocation(program, "u_camPosition");
	smallRadius = glGetUniformLocation(program, "u_smallRadius");
	bigRadius = glGetUniformLocation(program, "u_bigRadius");
	Model_matrixUniform = glGetUniformLocation(program, "m_modelMatrix");
	View_matrixUniform = glGetUniformLocation(program, "m_viewMatrix");
	Perspective_matrixUniform = glGetUniformLocation(program, "m_perspectiveMatrix");
	cubeTextureUniform = glGetUniformLocation(program, "u_cube_texture");
	
	timeUniform = glGetUniformLocation(program, "u_time");
	dispMaxUniform = glGetUniformLocation(program, "u_dispMax");
	terrainHeightUniform = glGetUniformLocation(program, "u_height");

	return 0;
}

Shader::~Shader()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader() {
	program = -1, vertexShader = -1, fragmentShader = -1;
	fileVS[0] = NULL;
	fileFS[0] = NULL;
	positionAttribute = -1;
	colorAttribute = -1;
	matrixUniform = -1;
	cam_matrixUniform = -1;
	normalAttribute = -1;
	binormalAttribute = -1;
	tangentAttribute = -1;
	uvAttribute = -1;
	fogColorUniform = -1;
	camPosition = -1;
	smallRadius = -1;
	bigRadius = -1;
	uv2Attribute = -1;
	timeUniform = -1;
	dispMaxUniform = -1;
	terrainHeightUniform = -1;
}

Shader::Shader(string FSpath, string VSpath) {
	Shader();
	strcpy(fileFS, FSpath.c_str());
	strcpy(fileVS, VSpath.c_str());
}