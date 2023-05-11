#pragma once
#include <string>
#include <vector>
#include "Globals.h"
#include "../Utilities/utilities.h"

class Shader
{
public:
	GLint program;
	GLuint vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;
	GLint colorAttribute;
	GLint matrixUniform;
	GLint cam_matrixUniform;
	GLint Model_matrixUniform;
	GLint View_matrixUniform;
	GLint Perspective_matrixUniform;
	GLint normalAttribute;
	GLint binormalAttribute;
	GLint tangentAttribute;
	GLint uvAttribute;
	std::vector<GLint> textureUniforms;
	GLint cubeTextureUniform;
	GLint fogColorUniform;
	GLint camPosition;
	GLint smallRadius;
	GLint bigRadius;
	GLint uv2Attribute;
	GLint timeUniform;
	GLint dispMaxUniform;
	GLint terrainHeightUniform;

	int Init(char * fileVertexShader, char * fileFragmentShader, int nr_textures);
	int Init(int nr_textures);
	Shader();
	Shader(std::string FSpath, std::string VSpath);
	~Shader();
};