#pragma once

#include "stdafx.h"
#include "../Utilities/rapidxml.hpp"
#include "../Utilities/rapidxml_iterators.hpp"
#include "../Utilities/rapidxml_print.hpp"
#include "../Utilities/rapidxml_utils.hpp"
#include "../Utilities/Math.h"
#include <vector>
#include <string>
#include <unordered_map>
#include "Model.h"
#include "Camera.h"

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
	GLuint textureId;
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
	GLuint progId;
	GLuint vsId, fsId;
public:
	Shader();
	Shader(std::string FSpath, std::string VSpath);
	int Load(std::string FSpath, std::string VSpath);
	int Load();
	int Unload(); /// vechiul progId la succes, -1 la esec
	~Shader();
};

class ResourceManager;

class ResourceManager {
private:
	static ResourceManager* rmInstance;
	std::unordered_map<int, Model*> models;
	std::unordered_map<int, Texture*> textures;
	std::unordered_map<int, Shader*> shaders;
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

class SceneManager;

class SceneObject {
private:
	int id;
	int modelId, shaderId;
	std::vector<int> textureIds;
	Model* model;
	Shader* shader;
	std::vector<Texture*> textures;
	bool depthTest;
	bool wired;
	bool generatedModel;
	enum {NORMAL, TERRAIN} type;
	SceneObject();
public:
	std::string name;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 color;
	~SceneObject();
	SceneObject(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int ShaderId, std::vector<int>& textureIds);
	friend SceneManager;
};

class SceneManager {
public:
	enum action {
		NONE,

		MOVE_CAMERA_POSITIVE_X,
		MOVE_CAMERA_NEGATIVE_X,
		MOVE_CAMERA_POSITIVE_Y,
		MOVE_CAMERA_NEGATIVE_Y,
		MOVE_CAMERA_POSITIVE_Z,
		MOVE_CAMERA_NEGATIVE_Z,

		ROTATE_CAMERA_POSITIVE_X,
		ROTATE_CAMERA_NEGATIVE_X,
		ROTATE_CAMERA_POSITIVE_Y,
		ROTATE_CAMERA_NEGATIVE_Y,
		ROTATE_CAMERA_POSITIVE_Z,
		ROTATE_CAMERA_NEGATIVE_Z

	};
private:
	std::string gameName;
	static SceneManager* smInstance;
	struct ScreenSize {
		int width;
		int height;
	} screenSize;
	bool fullscreen;
	Vector4 bgcolor;
	Camera* activeCamera;
	int activeCameraId;
	std::unordered_map<int,Camera*> cameras;
	std::unordered_multimap<unsigned char, action> keysToControls;
	std::unordered_multimap<action, unsigned char> controlsToKeys;
	struct { Vector3 Ox, Oy, Oz; } objAxesColor, camAxesColor;

	std::unordered_map<int, SceneObject*> sceneObjects;
	void ParseNode(rapidxml::xml_node<>* pNode, std::string additive_relative_path);
	SceneManager();
	~SceneManager();
public:
	std::string getGameName();
	static SceneManager* getInstance();
	int getScreenHeight();
	int getScreenWidth();
	std::unordered_multimap<unsigned char, action> getKeysToControls();
	std::vector<action> getActionsFromKey(unsigned char key);
	Vector4 getBgColor();
	int ParseXML(std::string xml_path);
	int ParseXML(char* xml_path);
	Camera* getCameraWithId(int id);
	Camera* getActiveCamera();
	SceneObject* getSceneObject(int id);
};