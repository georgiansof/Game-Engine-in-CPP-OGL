#pragma once

#include "stdafx.h"
#include "../Utilities/rapidxml.hpp"
#include "../Utilities/rapidxml_iterators.hpp"
#include "../Utilities/rapidxml_print.hpp"
#include "../Utilities/rapidxml_utils.hpp"
#include "../Utilities/Math.h"
#include <vector>
#include <string>
#include <map>
#include "Model.h"

/*class Singleton
{
protected:
	static Singleton* spInstance;
	Singleton();
public:
	void Init();//initializari - pot fi si in constructor
	static Singleton* getInstance();
	void metodaInstanta();//o metoda oarecare, nestatica
	void freeResources();//eliberarea zonelor de memorie alocate dinamic - se poate realiza si in destructor
	~Singleton();
};*/

struct ModelResource {
	std::vector<ModelVertex> vertices;
	std::vector<Vector3_uhint> indices;
};

struct TextureResource {
	unsigned char* pixel_array;
	int texture_width, texture_height;
	int bpp;
};

struct ShaderResource {
	std::string FragmentShader;
	std::string VertexShader;
};

class Model {
private:
	std::string path;
	ModelResource* mr;
	GLuint iboId;
	GLuint wiredIboId;
	GLuint vboId;
	int nrIndices, nrIndicesWired;
public:
	int Load();
	int Unload(); /// -1 la esec
	Model();
	Model(std::string path);
	~Model();
};

class Texture {
public:
	enum filtering_type {
		NEAREST,
		LINEAR,
		NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR,
		UNSETF
	};
	enum wrapping_type {
		CLAMP_TO_EDGE,
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_BORDER,
		MIRROR_CLAMP_TO_EDGE,
		UNSETW
	};
private:
	std::string path;
	TextureResource* tr;
	int textureId;
	filtering_type min_filter, mag_filter;
	wrapping_type wrap_s, wrap_t;
public:
	int GetTextureResource(TextureResource *res);
	int Load();
	int Unload(); 
	std::string getTGA();
	int getTextureId();
	Texture();
	Texture(int textureId, std::string path, filtering_type min_filter, filtering_type mag_filter, wrapping_type wrap_s, wrapping_type wrap_t);
	~Texture();
};

class Shader {
private:
	ShaderResource* sr;
	int progId;
public:
	Shader();
	Shader(std::string FSpath, std::string VSpath);
	int Load(std::string FSpath, std::string VSpath);
	int Unload(); /// vechiul progId la succes, -1 la esec
	~Shader();
};

class ResourceManager;

class ResourceManager {
private:
	static ResourceManager* rmInstance;
	std::map<int, Model*> models;
	std::map<int, Texture*> textures;
	std::map<int, Shader*> shaders;
	ResourceManager();
	void ParseNode(rapidxml::xml_node<>*,std::string path);
	~ResourceManager();
public:
	Model* getModel(int id);
	Texture* getTexture(int id);
	Shader* getShader(int id);
	int ParseXML(std::string xml_path);
	int ParseXML(char* xml_path);
	static ResourceManager* getInstance();
};