#pragma once
#include <string>
#include "../Utilities/utilities.h"

class Shader
{
public:
	GLuint program, vertexShader, fragmentShader;
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
	GLint textureUniform;
	GLint cubeTextureUniform;
	GLint fogColorUniform;
	GLint camPosition;
	GLint smallRadius;
	GLint bigRadius;


	int Init(char * fileVertexShader, char * fileFragmentShader);
	int Init();
	Shader();
	Shader(std::string FSpath, std::string VSpath);
	~Shader();
};