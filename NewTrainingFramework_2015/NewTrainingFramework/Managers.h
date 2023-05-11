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
#include <map>
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
	ModelResource* getResource();
	int Load();
	int Unload(); /// -1 la esec
	Model();
	Model(std::string path);
	~Model();
	friend SceneObject;
	friend SceneManager;
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
	int mapId;
	GLenum type;
	std::string path;
	TextureResource* tr;
	GLuint glTextureId;
	GLuint glCubeTextureId;
	filtering_type min_filter, mag_filter;
	wrapping_type wrap_s, wrap_t;
	int bufferNumber;
public:
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

class Fire;

class SceneObject {
public:
	enum objType {NORMAL, TERRAIN, SKYBOX, FIRE} type;
protected:
	int id;
	int modelId, shaderId;
	std::vector<int> textureIds;
	Model* model;
	Shader* shader;
	std::map<int, Texture*> textures;
	bool depthTest;
	bool wired;
	bool generatedModel;
	unsigned short int grid_dimension;
	float grid_width, grid_height;
	bool debug;
	SceneObject();
public:
	unsigned short int getGridDimension();
	float getGridWidth();
	float getGridHeight();
	std::string name;
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 color;
	Vector3 followingCamera;
	Model* getModel();
	Shader* getShader();
	std::map<int,Texture*>& getTextures();
	void generateModel();
	~SceneObject();
	SceneObject(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int shaderId, std::vector<int>& textureIds, objType type, bool debug);
	int Init();
	int Draw(Matrix& vp);
	bool isOnDebug();
	friend SceneManager;
	friend SkyBox;
	friend Fire;
};

class Fire : public SceneObject {
private:
	float dispMax;
	float timer;
	Fire();
public:
	Fire(std::string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int ShaderId, std::vector<int>& textureIds, objType type, bool debug, float dispMax);
	Fire(SceneObject* obj, float dispMax);
	void UpdateTimer(float delta);
	float get_dispMax();
	float get_time();
	friend SceneManager;
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
	Vector3 terrain_heights;
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

	std::map<int, SceneObject*> sceneObjects;
	void ParseNode(rapidxml::xml_node<>* pNode, std::string additive_relative_path);
	SceneManager();
	~SceneManager();
public:
	int textureCount;
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
	SceneObject* getSceneObjectByType(SceneObject::objType type);
	std::map<int, SceneObject*>& getAllSceneObjects();
	float getFogBigRadius();
	float getFogSmallRadius();
	int addCamera(int id, Camera* cam);
	friend SceneObject;
};