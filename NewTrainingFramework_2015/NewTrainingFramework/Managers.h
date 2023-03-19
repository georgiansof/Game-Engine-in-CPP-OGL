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
#include "Shaders.h"

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


class SceneObject;

class Model {
private:
	ModelResource* mr;
	GLuint iboId;
	GLuint wiredIboId;
	GLuint vboId;
	int nrIndices, nrIndicesWired;
	bool generated;
public:
	std::string path;
	int Load();
	int Unload(); /// -1 la esec
	Model();
	Model(std::string path);
	~Model();
	friend SceneObject;
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
		UNSETW
	};
private:
	GLenum type;
	std::string path;
	TextureResource* tr;
	GLuint glTextureId;
	GLuint glCubeTextureId;
	filtering_type min_filter, mag_filter;
	wrapping_type wrap_s, wrap_t;
public:
	int GetTextureResource(TextureResource *res);
	int Load();
	int Unload(); 
	std::string getTGA();
	int getTextureId();
	Texture();
	Texture(int textureId, std::string path, filtering_type min_filter, filtering_type mag_filter, wrapping_type wrap_s, wrapping_type wrap_t, GLenum type);
	~Texture();
	friend SceneObject;
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

class SkyBox;

class SceneObject {
private:
	int id;
	int modelId, shaderId;
	std::vector<int> textureIds;
	Model* model;
	Shader* shader;
	std::unordered_map<int, Texture*> textures;
	bool depthTest;
	bool wired;
	bool generatedModel;
	enum objType {NORMAL, TERRAIN, SKYBOX} type;
	unsigned short int grid_dimension;
	unsigned short int grid_width, grid_height;
protected:
	SceneObject();
public:
	std::string name;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 color;
	Vector3 followingCamera;
	Model* getModel();
	Shader* getShader();
	std::unordered_map<int,Texture*>& getTextures();
	void generateModel();
	~SceneObject();
	SceneObject(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int ShaderId, std::vector<int>& textureIds, objType type);
	int Init();
	int Draw(Matrix& vp);
	friend SceneManager;
	friend SkyBox;
};

class SkyBox : public SceneObject {
public:
	SkyBox(SceneObject* sobj, float offsetY);
	SkyBox();
	friend SceneObject;
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
	struct {
		float smallRadius, bigRadius;
		Vector3 color;
		bool blendToSkybox;
	} fog;
	struct { Vector3 Ox, Oy, Oz; } objAxesColor, camAxesColor;

	std::unordered_map<int, SceneObject*> sceneObjects;
	void ParseNode(rapidxml::xml_node<>* pNode, std::string additive_relative_path);
	SceneManager();
	~SceneManager();
public:
	Vector3 getFogColor();
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
	int setActiveCamera(Camera* cam);
	SceneObject* getSceneObject(int id);
	std::unordered_map<int, SceneObject*>& getAllSceneObjects();
	float getFogBigRadius();
	float getFogSmallRadius();
	int addCamera(int id, Camera* cam);
};