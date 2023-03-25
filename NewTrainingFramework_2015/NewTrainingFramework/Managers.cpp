#include "stdafx.h"
#include "Managers.h"
#include <iostream>
#include "Model.h"
#include "Globals.h"
#include <cmath>

/*Singleton::Singleton() {
	this->Init();
}
void Singleton::Init() {
	spInstance = NULL;
}
Singleton* Singleton::getInstance() {
	if (spInstance == NULL)
		spInstance = new Singleton;
	return spInstance;
}
void Singleton::metodaInstanta() {

}

void Singleton::freeResources() {
	delete spInstance;
}
Singleton::~Singleton() {
	this->freeResources();
}*/

using namespace rapidxml;
using namespace std;

ResourceManager* ResourceManager::rmInstance = NULL;
SceneManager* SceneManager::smInstance = NULL;

ResourceManager::ResourceManager() {
}

ResourceManager* ResourceManager::getInstance() {
	if (rmInstance == NULL)
		rmInstance = new ResourceManager;
	return rmInstance;
}

ResourceManager::~ResourceManager() {
	delete rmInstance;
	shaders.clear();
	textures.clear();
	models.clear();
}

int ResourceManager::ParseXML(string xml_path) {
	file<> xmlFile(xml_path.c_str());
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* pRoot = doc.first_node();
	if (strcmp(_strlwr(pRoot->name()), "resourcemanager") == 0)
		ParseNode(pRoot, string(""));
	else
		return -1; /// eroare

	return 0; /// succes
}

int ResourceManager::ParseXML(char* xml_path) {
	file<> xmlFile(xml_path);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* pRoot = doc.first_node();
	if (strcmp(_strlwr(pRoot->name()), "resourcemanager") == 0)
		ParseNode(pRoot, string(""));
	else
		return -1; /// eroare

	return 0; /// succes
}

void ResourceManager::ParseNode(xml_node<>* pNode, string path) {
	bool bypass;
	for (xml_node<>* p = pNode->first_node(); p; p = p->next_sibling()) {
		bypass = 0;
		string child_path(path);
		if (child_path.length() && child_path[child_path.length() - 1] != '/')
			child_path += string("/");

		for (xml_attribute<>* attr = p->first_attribute(); attr; attr = attr->next_attribute())
			if (strcmp(_strlwr(attr->name()), "path") == 0)
				child_path += string(attr->value());
			else
				if (strcmp(_strlwr(attr->name()), "id") == 0) {
					if (strcmp(_strlwr(p->name()), "model") == 0) {
						/// model
						string filepath(child_path);
						filepath += string(p->first_node("file")->value());
						this->models.insert(make_pair(atoi(attr->value()), new Model(filepath)));
					}
					if (strcmp(_strlwr(p->name()), "texture") == 0) {
						/// textura
						GLenum type = GL_TEXTURE_2D;
						for (xml_attribute<>* textattr = p->first_attribute(); textattr; textattr = textattr->next_attribute())
							if (strcmp(_strlwr(textattr->name()), "type") == 0)
								if (strcmp(_strlwr(textattr->value()), "2d") == 0)
									type = GL_TEXTURE_2D;
								else
									if (strcmp(_strlwr(textattr->value()), "cube") == 0) {
										type = GL_TEXTURE_CUBE_MAP;
									}
								

						string filepath(child_path);
						Texture::filtering_type min_filter = Texture::UNSETF, mag_filter = Texture::UNSETF;
						Texture::wrapping_type wrap_s = Texture::UNSETW, wrap_t = Texture::UNSETW;

						for (xml_node<>* pt = p->first_node(); pt; pt = pt->next_sibling())
							if (strcmp(_strlwr(pt->name()), "file") == 0)
								filepath += string(pt->value());
							else
								if (strcmp(_strlwr(pt->name()), "min_filter") == 0) {
									int i = 0;
									vector<string> tmp({ "NEAREST",
													 "LINEAR",
													 "NEAREST_MIPMAP_NEAREST",
													 "LINEAR_MIPMAP_NEAREST",
													 "NEAREST_MIPMAP_LINEAR",
													 "LINEAR_MIPMAP_LINEAR" });
									for (auto x : tmp) {
										if (strcmp(_strupr(pt->value()), x.c_str()) == 0) {
											switch (i) {
											case 0:
												min_filter = Texture::NEAREST;
												break;
											case 1:
												min_filter = Texture::LINEAR;
												break;
											case 2:
												min_filter = Texture::NEAREST_MIPMAP_NEAREST;
												break;
											case 3:
												min_filter = Texture::LINEAR_MIPMAP_NEAREST;
												break;
											case 4:
												min_filter = Texture::NEAREST_MIPMAP_LINEAR;
												break;
											case 5:
												min_filter = Texture::LINEAR_MIPMAP_LINEAR;
												break;
											}
											break;
										}
										++i;
									}
								}
								else
									if (strcmp(_strlwr(pt->name()), "mag_filter") == 0)  {
										int i = 0;
										vector<string> tmp({ "NEAREST",
														 "LINEAR",
														 "NEAREST_MIPMAP_NEAREST",
														 "LINEAR_MIPMAP_NEAREST",
														 "NEAREST_MIPMAP_LINEAR",
														 "LINEAR_MIPMAP_LINEAR" });
										for (auto x : tmp) {
											if (strcmp(_strupr(pt->value()), x.c_str()) == 0) {
												switch (i) {
												case 0:
													mag_filter = Texture::NEAREST;
													break;
												case 1:
													mag_filter = Texture::LINEAR;
													break;
												case 2:
													mag_filter = Texture::NEAREST_MIPMAP_NEAREST;
													break;
												case 3:
													mag_filter = Texture::LINEAR_MIPMAP_NEAREST;
													break;
												case 4:
													mag_filter = Texture::NEAREST_MIPMAP_LINEAR;
													break;
												case 5:
													mag_filter = Texture::LINEAR_MIPMAP_LINEAR;
													break;
												}
												break;
											}
											++i;
										}
									}
									else
										if (strcmp(_strlwr(pt->name()), "wrap_s") == 0) {
											int i = 0;
											vector<string> tmp({ "CLAMP_TO_EDGE",
																 "REPEAT",
														 		 "MIRRORED_REPEAT" });
											for (auto x : tmp) {
												if (strcmp(_strupr(pt->value()), x.c_str()) == 0) {
													switch (i) {
													case 0:
														wrap_s = Texture::CLAMP_TO_EDGE;
														break;
													case 1:
														wrap_s = Texture::REPEAT;
														break;
													case 2:
														wrap_s = Texture::MIRRORED_REPEAT;
														break;
													}
													break;
												}
												++i;
											}
										}
										else
											if (strcmp(_strlwr(pt->name()), "wrap_t") == 0) {
												int i = 0;
												vector<string> tmp({ "CLAMP_TO_EDGE",
																	 "REPEAT",
																	 "MIRRORED_REPEAT",
																	 "CLAMP_TO_BORDER",
																	 "MIRROR_CLAMP_TO_EDGE" });
												for (auto x : tmp) {
													if (strcmp(_strupr(pt->value()), x.c_str()) == 0) {
														switch (i) {
														case 0:
															wrap_t = Texture::CLAMP_TO_EDGE;
															break;
														case 1:
															wrap_t = Texture::REPEAT;
															break;
														case 2:
															wrap_t = Texture::MIRRORED_REPEAT;
															break;
														}
														break;
													}
													++i;
												}
											}
						int id = atoi(attr->value());
;						this->textures.insert(make_pair(id, new Texture(id,filepath, min_filter, mag_filter, wrap_s, wrap_t,type)));
					}
					if (strcmp(_strlwr(p->name()), "shader") == 0) {
						/// shader
						string FSpath(child_path), VSpath(child_path);
						for (xml_node<>* ps = p->first_node(); ps; ps = ps->next_sibling())
							if (strcmp(_strlwr(ps->name()), "fs") == 0)
								FSpath += string(ps->value());
							else
								if (strcmp(_strlwr(ps->name()), "vs") == 0)
									VSpath += string(ps->value());

						this->shaders.insert(make_pair(atoi(attr->value()), new Shader(FSpath, VSpath)));
					}
					bypass=1;
				}
		if(!bypass)
			ParseNode(p, child_path);
	}
}

Model* ResourceManager::getModel(int id) {
	auto x = (this->models.find(id));
	if (x != this->models.end())
		return x->second;
	return NULL;
}

Texture* ResourceManager::getTexture(int id) {
	auto x = (this->textures.find(id));
	if (x != this->textures.end())
		return x->second;
	return NULL;
}

Shader* ResourceManager::getShader(int id) { 
	auto x = (this->shaders.find(id));
	if (x != this->shaders.end())
		return x->second;
	return NULL;
}

int Model::Load() {
	if (!generated && path.length() == 0) {
		cout << "Model path length is zero!\n";
		return -1;
	}
	if (mr)
		return 0;
	if (!generated) {
		mr = new ModelResource;
		NFG_Parser::Parse(path, mr->vertices, mr->indices);
		nrIndices = mr->indices.size();
		///nrIndicesWired = ???
	}
	return 0;
}

int Model::Unload() {
	if (mr) {
		mr->indices.clear();
		mr->vertices.clear();
		delete mr;

		return 0;
	}
	if(iboId != 0) glDeleteBuffers(1, &iboId);
	if(vboId != 0) glDeleteBuffers(1, &vboId);
	iboId = 0;
	wiredIboId = 0;
	vboId = 0;
	return -1;
}

Model::Model() {
	mr = NULL;
	iboId = 0;
	wiredIboId = 0;
	vboId = 0;
	nrIndices = -1;
	nrIndicesWired = -1;
	generated = false;
}

Model::Model(string path) {
	mr = NULL;
	iboId = 0;
	wiredIboId = 0;
	vboId = 0;
	nrIndices = -1;
	nrIndicesWired = -1;
	this->path.assign(path.c_str());
	generated = false;
}

Model::~Model() {
	this->Unload();
}

int Texture::getTextureId() { return glTextureId; }

int Texture::Load() {
	if (tr)
		return 0;
	tr = new TextureResource;
	tr->pixel_array = (unsigned char*) LoadTGA(path.c_str(), &tr->texture_width, &tr->texture_height, &tr->bpp);
	if (tr->pixel_array == NULL) {
		delete tr;
		return -1;
	}
	if (this->type == GL_TEXTURE_2D) {
		if (this->glTextureId == 0)
			glGenTextures(1, &this->glTextureId); 
		glBindTexture(GL_TEXTURE_2D, this->glTextureId);
		switch (this->min_filter) {
		case Texture::LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		case Texture::NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		case Texture::NEAREST_MIPMAP_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case Texture::LINEAR_MIPMAP_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case Texture::NEAREST_MIPMAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case Texture::LINEAR_MIPMAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		switch (this->mag_filter) {
		case Texture::LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case Texture::NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case Texture::NEAREST_MIPMAP_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
			break;
		case Texture::LINEAR_MIPMAP_NEAREST:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			break;
		case Texture::NEAREST_MIPMAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
			break;
		case Texture::LINEAR_MIPMAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
		switch (this->wrap_s) {
		case Texture::CLAMP_TO_EDGE:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			break;
		case Texture::REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			break;
		case Texture::MIRRORED_REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			break;
		}
		switch (this->wrap_t) {
		case Texture::CLAMP_TO_EDGE:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			break;
		case Texture::REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;
		case Texture::MIRRORED_REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			break;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->tr->texture_width, this->tr->texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->tr->pixel_array);
		glBindTexture(GL_TEXTURE0 + SceneManager::getInstance()->textureCount, 0);
		this->bufferNumber = SceneManager::getInstance()->textureCount;
		++(SceneManager::getInstance()->textureCount);
	}
	else
		if (this->type == GL_TEXTURE_CUBE_MAP) {
			if (this->glCubeTextureId != 0)
				glGenTextures(1, &this->glCubeTextureId);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->glCubeTextureId);
			switch (this->min_filter) {
			case Texture::LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;
			case Texture::NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			case Texture::NEAREST_MIPMAP_NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				break;
			case Texture::LINEAR_MIPMAP_NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				break;
			case Texture::NEAREST_MIPMAP_LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				break;
			case Texture::LINEAR_MIPMAP_LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
			}
			switch (this->mag_filter) {
			case Texture::LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				break;
			case Texture::NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				break;
			case Texture::NEAREST_MIPMAP_NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				break;
			case Texture::LINEAR_MIPMAP_NEAREST:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				break;
			case Texture::NEAREST_MIPMAP_LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				break;
			case Texture::LINEAR_MIPMAP_LINEAR:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
			}
			switch (this->wrap_s) {
			case Texture::CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				break;
			case Texture::REPEAT:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
				break;
			case Texture::MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				break;
			}
			switch (this->wrap_t) {
			case Texture::CLAMP_TO_EDGE:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			case Texture::REPEAT:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case Texture::MIRRORED_REPEAT:
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			}
			for (int j = 0; j < 6; ++j) {
				int bpp = this->tr->bpp;
				int H = this->tr->texture_height;
				int W = this->tr->texture_width * (bpp / 8);
				unsigned char* px_array = this->tr->pixel_array;

				int targetH_start, targetH_end, targetW_start, targetW_end;

				switch (j) {
				case 0:
					targetH_start = H / 3;
					targetH_end = 2 * H / 3;

					targetW_start = W / 2;
					targetW_end = 3 * W / 4;
					break;
				case 1:
					targetH_start = H / 3;
					targetH_end = 2 * H / 3;

					targetW_start = 0;
					targetW_end = W / 4;
					break;
				case 2:
					targetH_start = 0;
					targetH_end = H / 3;

					targetW_start = W / 4;
					targetW_end = W / 2;
					break;
				case 3:
					targetH_start = 2 * H / 3;
					targetH_end = H;

					targetW_start = W / 4;
					targetW_end = W / 2;
					break;
				case 4:
					targetH_start = H / 3;
					targetH_end = 2 * H / 3;

					targetW_start = W / 4;
					targetW_end = W / 2;
					break;
				case 5:
					targetH_start = H / 3;
					targetH_end = 2 * H / 3;

					targetW_start = 3 * W / 4;
					targetW_end = W;
					break;
				}

				unsigned char* trimmed_array = new unsigned char[(H / 3) * (W / 4)];
				int k = 0;
				int offset = targetH_start * W;
				for (int z = targetH_start; z < targetH_end; ++z)
					for (int q = targetW_start; q < targetW_end; ++q)
						//trimmed_array[k++] = px_array[offset + W * z + q + (z-targetH_start) * W];
						trimmed_array[k++] = px_array[W * z + q];

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_RGB, this->tr->texture_width / 4, this->tr->texture_height / 3, 0, GL_RGB, GL_UNSIGNED_BYTE, trimmed_array);

				delete[] trimmed_array;
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
	return 0;
}

int Texture::GetTextureResource(TextureResource *res) {
	if (!tr)
		if (Texture::Load()==-1)
			return -1;
	*res = *tr;
	return 0;
}

string Texture::getTGA() {return this->path;}

int Texture::Unload() {
	if (tr != NULL) {
		if (tr->pixel_array != NULL)
			delete tr->pixel_array;
		delete tr;
		return 0;
	}
	glTextureId = 0;
	return -1;
}
Texture::Texture() {
	tr = NULL;
	type = GL_TEXTURE_2D;
	glTextureId = 0;
	glCubeTextureId = 0;
	this->min_filter = Texture::UNSETF;
	this->mag_filter = Texture::UNSETF;
	this->wrap_s = Texture::UNSETW;
	this->wrap_t = Texture::UNSETW;
	bufferNumber = -1;
}
Texture::Texture(int textureId, std::string path, Texture::filtering_type min_filter, Texture::filtering_type mag_filter, Texture::wrapping_type wrap_s, Texture::wrapping_type wrap_t, GLenum type) {
	tr = NULL;
	this->type = type;
	this->glTextureId = 0;
	this->mapId = textureId;
	this->path.assign(path.c_str());
	this->min_filter=min_filter;
	this->mag_filter=mag_filter;
	this->wrap_s=wrap_s;
	this->wrap_t=wrap_t;
	this->type = type;
	bufferNumber = -1;
	glCubeTextureId = 0;
}
Texture::~Texture() {
	this->Unload();
}

SceneObject::SceneObject() {
	position = Vector3(0, 0, 0);
	rotation = Vector3(0, 0, 0);
	scale = Vector3(1, 1, 1);
	grid_dimension = 4;
	grid_width = grid_height = 50;
	debug = false;
}

SceneObject::~SceneObject() {
	textures.clear();
	textureIds.clear();
}

SceneObject::SceneObject(string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int ShaderId, vector<int> &textureIds,SceneObject::objType type, bool debug) {
	this->debug = debug;
	this->name = name;
	this->type = type;
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
	this->depthTest = depthTest;
	this->modelId = modelId;
	this->shaderId = shaderId;
	this->model = ResourceManager::getInstance()->getModel(modelId);
	this->shader = ResourceManager::getInstance()->getShader(shaderId);
	for (auto& x : textureIds) {
		this->textureIds.push_back(x);
		textures.insert(make_pair(x,ResourceManager::getInstance()->getTexture(x)));
	}
}

SceneManager::SceneManager() {
	smInstance = nullptr;
	activeCamera = nullptr;
	activeCameraId = -1;
	screenSize.width = Globals::screenWidth;
	screenSize.height = Globals::screenHeight;
	fullscreen = false;
	bgcolor = Vector4(0, 0, 0, 1);
	objAxesColor.Ox = Vector3(1, 1, 1);
	objAxesColor.Oy = Vector3(1, 1, 1);
	objAxesColor.Oz = Vector3(1, 1, 1);
	camAxesColor.Ox = Vector3(1, 1, 1);
	camAxesColor.Oy = Vector3(1, 1, 1);
	camAxesColor.Oz = Vector3(1, 1, 1);
	fog.bigRadius = 900;
	fog.smallRadius = 800;
	fog.color = Vector3(60.0/255, 68.0/255, 85.0/255);
	fog.blendToSkybox = false;
	textureCount = 0;
}

SceneManager::~SceneManager() {
	keysToControls.clear();
	controlsToKeys.clear();
	cameras.clear();
}

SceneManager* SceneManager::getInstance() {
	if (smInstance == nullptr) 
		smInstance = new SceneManager;
	return smInstance;
}

int SceneManager::ParseXML(string xml_path) {
	return ParseXML(xml_path.c_str());
}

int SceneManager::ParseXML(char* xml_path) {
	file<> xmlFile(xml_path);
	xml_document<> doc;
	doc.parse<0>(xmlFile.data());
	xml_node<>* pRoot = doc.first_node();
	if (strcmp(_strlwr(pRoot->name()), "scenemanager") == 0)
		ParseNode(pRoot, string(""));
	else
		return -1; /// eroare

	return 0; /// succes
}

unsigned char ParseKey(char *str) {
	if (strcmp(_strupr(str), "CTRL") == 0 || strcmp(_strupr(str), "CONTROL") == 0)
		return VK_CONTROL;
	if (strcmp(_strupr(str), "SHIFT") == 0)
		return VK_SHIFT;
	if (strcmp(_strupr(str), "BACKSPACE") == 0)
		return VK_BACK;
	if (strcmp(_strupr(str), "ENTER") == 0 || strcmp(_strupr(str), "RETURN") == 0)
		return VK_RETURN;
	if (strcmp(_strupr(str), "TAB") == 0)
		return VK_TAB;
	if (strcmp(_strupr(str), "CAPS") == 0 || strcmp(_strupr(str), "CAPSLOCK") == 0 || strcmp(_strupr(str), "CAPITAL") == 0)
		return VK_CAPITAL;
	if (strcmp(_strupr(str), "NUMLOCK") == 0)
		return VK_NUMLOCK;
	if (strcmp(_strupr(str), "INS") == 0 || strcmp(_strupr(str), "INSERT") == 0)
		return VK_INSERT;
	if (strcmp(_strupr(str), "DELETE") == 0)
		return VK_DELETE;
	if (strcmp(_strupr(str), "HOME") == 0)
		return VK_HOME;
	if (strcmp(_strupr(str), "END") == 0)
		return VK_END;
	if (strcmp(_strupr(str), "ARRUP") == 0 || strcmp(_strupr(str), "ARROWUP") == 0 || strcmp(_strupr(str), "UP") == 0)
		return VK_UP;
	if (strcmp(_strupr(str), "ARRDOWN") == 0 || strcmp(_strupr(str), "ARROWDOWN") == 0 || strcmp(_strupr(str), "DOWN") == 0)
		return VK_DOWN;
	if (strcmp(_strupr(str), "ARRLEFT") == 0 || strcmp(_strupr(str), "ARROWLEFT") == 0 || strcmp(_strupr(str), "LEFT") == 0)
		return VK_LEFT;
	if (strcmp(_strupr(str), "ARRRIGHT") == 0 || strcmp(_strupr(str), "ARROWRIGHT") == 0 || strcmp(_strupr(str), "RIGHT") == 0)
		return VK_RIGHT;
	/// F1 ... F12
	char fkey[4] = "F";
	fkey[3] = NULL;
	for (int i = 1; i <= 12; ++i) {
		fkey[1] = i<10 ? i + '0' : '1';
		fkey[2] = i < 10 ? NULL : i % 10 + '0';
		
		if (strcmp(_strupr(str), fkey) == 0)
			return VK_F1 + (i - 1);
	}

	char numpad[5] = "NUM ";

	for (int i = 0; i < 9; ++i) {
		numpad[3] = '0' + i;
		if (strcmp(_strupr(str), numpad) == 0)
			return VK_NUMPAD0 + i;
	}
	return -1;
}

void SceneManager::ParseNode(xml_node<>* pNode, string additive_relative_path) {
	for (xml_node<>* p = pNode->first_node(); p; p = p->next_sibling())
		if (strcmp(_strlwr(p->name()), "gamename") == 0)
			this->gameName = p->value();
		else
			if (strcmp(_strlwr(p->name()), "defaultscreensize") == 0)
				for (xml_node<>* q = p->first_node(); q; q = q->next_sibling())
					if (strcmp(_strlwr(q->name()), "width") == 0)
						this->screenSize.width = atoi(q->value());
					else
						if (strcmp(_strlwr(q->name()), "height") == 0)
							this->screenSize.height = atoi(q->value());
						else
							if (strcmp(_strlwr(q->name()), "fullscreen") == 0)
								this->fullscreen = true;
							else
								;
			else
				if (strcmp(_strlwr(p->name()), "backgroundcolor") == 0)
					for (xml_node<>* q = p->first_node(); q; q = q->next_sibling())
						if (strcmp(_strlwr(q->name()), "r") == 0)
							this->bgcolor.x = atof(q->value());
						else
							if (strcmp(_strlwr(q->name()), "g") == 0)
								this->bgcolor.y = atof(q->value());
							else
								if (strcmp(_strlwr(q->name()), "b") == 0)
									this->bgcolor.z = atof(q->value());
								else
									if (strcmp(_strlwr(q->name()), "a") == 0)
										this->bgcolor.w = atof(q->value());
									else
										;
				else
					if (strcmp(_strlwr(p->name()), "controls") == 0)
						for (xml_node<>* q = p->first_node(); q; q = q->next_sibling())
							if (strcmp(_strlwr(q->name()), "control") == 0) {
								unsigned char key = -1;
								vector<SceneManager::action> actions;
								string action_string = string("");
								for (xml_node<>* r = q->first_node(); r; r = r->next_sibling())
									if (strcmp(_strlwr(r->name()), "key") == 0) {
										char buff[100];
										strcpy(buff, r->value());
										char* cptr = strtok(buff, " ");
										if (cptr)
											if (cptr[1] == NULL)
												key = toupper(cptr[0]);
											else
												key = ParseKey(cptr);
										else
											continue; /// eroare, nu este luata in considerare
									}
									else
										if (strcmp(_strlwr(r->name()), "action") == 0) {
											char buff[100];
											strcpy(buff, r->value());
											char* cptr = strtok(buff, " ");
											if (cptr)
												action_string.assign(cptr);
											else
												continue; /// eroare
											
											/// STRING -> action enum
											if (action_string.compare("MOVE_CAMERA_POSITIVE_X") == 0) {
												actions.push_back(MOVE_CAMERA_POSITIVE_X);
												continue;
											}
											if (action_string.compare("MOVE_CAMERA_POSITIVE_Y") == 0) {
												actions.push_back(MOVE_CAMERA_POSITIVE_Y);
												continue;
											}
											if (action_string.compare("MOVE_CAMERA_POSITIVE_Z") == 0) {
												actions.push_back(MOVE_CAMERA_POSITIVE_Z);
												continue;
											}
											if (action_string.compare("MOVE_CAMERA_NEGATIVE_X") == 0) {
												actions.push_back(MOVE_CAMERA_NEGATIVE_X);
												continue;
											}
											if (action_string.compare("MOVE_CAMERA_NEGATIVE_Y") == 0) {
												actions.push_back(MOVE_CAMERA_NEGATIVE_Y);
												continue;
											}
											if (action_string.compare("MOVE_CAMERA_NEGATIVE_Z") == 0) {
												actions.push_back(MOVE_CAMERA_NEGATIVE_Z);
												continue;
											}

											if (action_string.compare("ROTATE_CAMERA_POSITIVE_X") == 0) {
												actions.push_back(ROTATE_CAMERA_POSITIVE_X);
												continue;
											}
											if (action_string.compare("ROTATE_CAMERA_POSITIVE_Y") == 0) {
												actions.push_back(ROTATE_CAMERA_POSITIVE_Y);
												continue;
											}
											if (action_string.compare("ROTATE_CAMERA_POSITIVE_Z") == 0) {
												actions.push_back(ROTATE_CAMERA_POSITIVE_Z);
												continue;
											}
											if (action_string.compare("ROTATE_CAMERA_NEGATIVE_X") == 0) {
												actions.push_back(ROTATE_CAMERA_NEGATIVE_X);
												continue;
											}
											if (action_string.compare("ROTATE_CAMERA_NEGATIVE_Y") == 0) {
												actions.push_back(ROTATE_CAMERA_NEGATIVE_Y);
												continue;
											}
											if (action_string.compare("ROTATE_CAMERA_NEGATIVE_Z") == 0) {
												actions.push_back(ROTATE_CAMERA_NEGATIVE_Z);
												continue;
											}

											
										}
								if (key != -1 && actions.size() > 0 ) {
									for (action act : actions) {
										this->controlsToKeys.insert(make_pair(act, key));
										this->keysToControls.insert(make_pair(key, act));
									}
								}
							}
							else
								;
					else
						if (strcmp(_strlwr(p->name()), "cameras") == 0)
							for (xml_node<>* q = p->first_node(); q; q = q->next_sibling())
								if (strcmp(_strlwr(q->name()), "camera") == 0) {
									int camId = -1;
									Camera* cam = new Camera;
									for (xml_attribute<>* attr = q->first_attribute(); attr; attr = attr->next_attribute())
										if (strcmp(_strlwr(attr->name()), "id") == 0) {
											camId = atoi(attr->value());
											break;
										}
									for (xml_node<>* r = q->first_node(); r; r = r->next_sibling())
										if (strcmp(_strlwr(r->name()), "type") == 0)
											if (strcmp(_strlwr(r->value()), "firstperson") == 0 || strcmp(_strlwr(r->value()), "first person") == 0)
												cam->type = Camera::FIRST_PERSON;
											else
												if (strcmp(_strlwr(r->value()), "thirdperson") == 0 || strcmp(_strlwr(r->value()), "third person") == 0)
													cam->type = Camera::THIRD_PERSON;
												else
													;
										else
											if (strcmp(_strlwr(r->name()), "position") == 0)
												for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
													if (strcmp(_strlwr(nd->name()), "x") == 0)
														cam->position.x = atof(nd->value());
													else
														if (strcmp(_strlwr(nd->name()), "y") == 0)
															cam->position.y = atof(nd->value());
														else
															if (strcmp(_strlwr(nd->name()), "z") == 0)
																cam->position.z = atof(nd->value());
															else
																;
											else
												if (strcmp(_strlwr(r->name()), "target") == 0)
													for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
														if (strcmp(_strlwr(nd->name()), "x") == 0)
															cam->target.x = atof(nd->value());
														else
															if (strcmp(_strlwr(nd->name()), "y") == 0)
																cam->target.y = atof(nd->value());
															else
																if (strcmp(_strlwr(nd->name()), "z") == 0)
																	cam->target.z = atof(nd->value());
																else
																	;
												else
													if (strcmp(_strlwr(r->name()), "up") == 0)
														for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
															if (strcmp(_strlwr(nd->name()), "x") == 0)
																cam->up.x = atof(nd->value());
															else
																if (strcmp(_strlwr(nd->name()), "y") == 0)
																	cam->up.y = atof(nd->value());
																else
																	if (strcmp(_strlwr(nd->name()), "z") == 0)
																		cam->up.z = atof(nd->value());
																	else
																		;
													else
														if (strcmp(_strlwr(r->name()), "translationspeed") == 0)
															cam->moveSpeed = atof(r->value());
														else
															if (strcmp(_strlwr(r->name()), "rotationspeed") == 0)
																cam->rotateSpeed = atof(r->value());
															else
																if (strcmp(_strlwr(r->name()), "fov") == 0)
																	cam->fov = math::pi * atof(r->value()) / 180.0;
																else
																	if (strcmp(_strlwr(r->name()), "near") == 0)
																		cam->_near = atof(r->value());
																	else
																		if (strcmp(_strlwr(r->name()), "far") == 0)
																			cam->_far = atof(r->value());
																		else
																			;
								
									if (camId != -1) {
										cam->updateWorldView();
										this->cameras.insert(make_pair(camId, cam));
									}
									else
										delete cam;
									if (activeCameraId != -1 && camId == activeCameraId)
										activeCamera = cam;
								}
								else
									;
						else
							if (strcmp(_strlwr(p->name()), "activecamera") == 0) {
								this->activeCameraId = atoi(p->value());
								auto it = cameras.find(activeCameraId);
								if (it != cameras.end())
									this->activeCamera = it->second;
							}
							else 
								if (strcmp(_strlwr(p->name()), "objects") == 0) {
									for(xml_node<> *q = p->first_node(); q ; q = q->next_sibling())
										if (strcmp(_strlwr(q->name()), "object") == 0) {
											SceneObject *objptr = new SceneObject;
											objptr->depthTest = false;
											objptr->wired = false;

											float verticalOffset = 0;

											for(xml_attribute<> *atr = q->first_attribute(); atr; atr = atr->next_attribute())
												if (strcmp(_strlwr(atr->name()), "id") == 0) {
													objptr->id = atoi(atr->value());
													break;
												}
											if (objptr->id == -1) { delete objptr; break; } /// eroare

											for (xml_node<>* r = q->first_node(); r; r = r->next_sibling())
												if (strcmp(_strlwr(r->name()), "model") == 0) {
													if (strcmp(_strlwr(r->value()), "generated") == 0) {
														objptr->modelId = -1;
														objptr->model = new Model;
														objptr->generatedModel = true;
														objptr->model->generated = true;
													}
													else {
														objptr->generatedModel = false;
														objptr->modelId = atoi(r->value());
														objptr->model = ResourceManager::getInstance()->getModel(objptr->modelId);
													}
												}
												else
													if (strcmp(_strlwr(r->name()), "shader") == 0) {
														objptr->shaderId = atoi(r->value());
														objptr->shader = ResourceManager::getInstance()->getShader(objptr->shaderId);
													}
													else
														if (strcmp(_strlwr(r->name()), "name") == 0)
															objptr->name.assign(r->value());
														else
															if (strcmp(_strlwr(r->name()), "textures") == 0) {
																for (xml_node <>* txt = r->first_node(); txt; txt = txt->next_sibling())
																	if (strcmp(_strlwr(txt->name()), "texture") == 0)
																		for (xml_attribute <>* attr = txt->first_attribute(); attr; attr = attr->next_attribute())
																			if (strcmp(_strlwr(attr->name()), "id") == 0) {
																				objptr->textureIds.push_back(atoi(attr->value()));
																				objptr->textures.insert(make_pair(atoi(attr->value()), ResourceManager::getInstance()->getTexture(atoi(attr->value()))));
																				break;
																			}
																			
															}
															else
																if (strcmp(_strlwr(r->name()), "position") == 0)
																	for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
																		if (strcmp(_strlwr(nd->name()), "x") == 0)
																			objptr->position.x = atof(nd->value());
																		else
																			if (strcmp(_strlwr(nd->name()), "y") == 0)
																				objptr->position.y = atof(nd->value());
																			else
																				if (strcmp(_strlwr(nd->name()), "z") == 0)
																					objptr->position.z = atof(nd->value());
																				else
																					;
																else
																	if (strcmp(_strlwr(r->name()), "rotation") == 0)
																		for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
																			if (strcmp(_strlwr(nd->name()), "x") == 0)
																				objptr->rotation.x = math::toRadians(atof(nd->value()));
																			else
																				if (strcmp(_strlwr(nd->name()), "y") == 0)
																					objptr->rotation.y = math::toRadians(atof(nd->value()));
																				else
																					if (strcmp(_strlwr(nd->name()), "z") == 0)
																						objptr->rotation.z = math::toRadians(atof(nd->value()));
																					else
																						;
																	else
																		if (strcmp(_strlwr(r->name()), "scale") == 0)
																			for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
																				if (strcmp(_strlwr(nd->name()), "x") == 0)
																					objptr->scale.x = atof(nd->value());
																				else
																					if (strcmp(_strlwr(nd->name()), "y") == 0)
																						objptr->scale.y = atof(nd->value());
																					else
																						if (strcmp(_strlwr(nd->name()), "z") == 0)
																							objptr->scale.z = atof(nd->value());
																						else
																							;
																		else
																			if (strcmp(_strlwr(r->name()), "wired") == 0)
																				objptr->wired = true;
																			else
																				if (strcmp(_strlwr(r->name()), "color") == 0)
																					for (xml_node<>* nd = r->first_node(); nd; nd = nd->next_sibling())
																						if (strcmp(_strlwr(nd->name()), "r") == 0)
																							objptr->color.x = atof(nd->value());
																						else
																							if (strcmp(_strlwr(nd->name()), "g") == 0)
																								objptr->color.y = atof(nd->value());
																							else
																								if (strcmp(_strlwr(nd->name()), "b") == 0)
																									objptr->color.z = atof(nd->value());
																								else
																									;
																				else
																					if (strcmp(_strlwr(r->name()), "type") == 0)
																						if (strcmp(_strlwr(r->value()), "normal") == 0)
																							objptr->type = SceneObject::NORMAL;
																						else
																							if (strcmp(_strlwr(r->value()), "terrain") == 0)
																								objptr->type = SceneObject::TERRAIN;
																							else
																								if (strcmp(_strlwr(r->value()), "skybox") == 0)
																									objptr->type = SceneObject::SKYBOX;
																								else
																									;
																					else
																						if(strcmp(_strlwr(r->name()), "offsety") == 0)
																							verticalOffset = atof(r->value());
																						else
																							if (strcmp(_strlwr(r->name()), "followingcamera") == 0) {
																								for (xml_node<>* z = r->first_node(); z; z = z->next_sibling())
																									if (strcmp(_strlwr(z->name()), "ox") == 0)
																										objptr->followingCamera.x = 1;
																									else
																										if (strcmp(_strlwr(z->name()), "oy") == 0)
																											objptr->followingCamera.y = 1;
																										else
																											if (strcmp(_strlwr(z->name()), "oz") == 0)
																												objptr->followingCamera.z = 1;
																											else
																												;
																							}
																							else
																								if(strcmp(_strlwr(r->name()), "griddimension") == 0)
																									objptr->grid_dimension = atof(r->value());
																								else
																									if(strcmp(_strlwr(r->name()), "gridwidth") == 0 )
																										objptr->grid_width = atof(r->value());
																									else
																										if(strcmp(_strlwr(r->name()), "gridheight") == 0)
																											objptr->grid_height = atof(r->value());
																										else
																											if(strcmp(_strlwr(r->name()), "debug") == 0)
																												objptr->debug = true;
																											else
																												;
											if(objptr->type == SceneObject::SKYBOX) {
												SkyBox *sb = new SkyBox(objptr, verticalOffset);
												sb->depthTest = true;
												this->sceneObjects.insert(make_pair(sb->id, (SceneObject*)sb));
												delete objptr;
											}
											else {
												if (objptr->type == SceneObject::TERRAIN)
													objptr->generateModel();
												this->sceneObjects.insert(make_pair(objptr->id, objptr));
											}
										}
									
								}
								else
									if (strcmp(_strlwr(p->name()), "debugsettings") == 0) {
										for (xml_node<>* q = p->first_node(); q; q = q->next_sibling())
											if (strcmp(_strlwr(q->name()), "objectaxes") == 0)
												for (xml_node<>* axes = q->first_node(); axes; axes = axes->next_sibling())
													if (strcmp(_strlwr(axes->name()), "oxcolor") == 0)
														for (xml_node <>* clr = axes->first_node(); clr; clr = clr->next_sibling())
															if (strcmp(_strlwr(clr->name()), "r") == 0)
																objAxesColor.Ox.x = atof(clr->value());
															else
																if (strcmp(_strlwr(clr->name()), "g") == 0)
																	objAxesColor.Ox.y = atof(clr->value());
																else
																	if (strcmp(_strlwr(clr->name()), "b") == 0)
																		objAxesColor.Ox.z = atof(clr->value());
																	else
																		;
													else
														if (strcmp(_strlwr(axes->name()), "oycolor") == 0)
															for (xml_node<>* clr = axes->first_node(); clr; clr = clr->next_sibling())
																if (strcmp(_strlwr(clr->name()), "r") == 0)
																	objAxesColor.Oy.x = atof(clr->value());
																else
																	if (strcmp(_strlwr(clr->name()), "g") == 0)
																		objAxesColor.Oy.y = atof(clr->value());
																	else
																		if (strcmp(_strlwr(clr->name()), "b") == 0)
																			objAxesColor.Oy.z = atof(clr->value());
																		else
																			;
														else
															if (strcmp(_strlwr(axes->name()), "ozcolor") == 0)
																for (xml_node<>* clr = axes->first_node(); clr; clr = clr->next_sibling())
																	if (strcmp(_strlwr(clr->name()), "r") == 0)
																		objAxesColor.Oz.x = atof(clr->value());
																	else
																		if (strcmp(_strlwr(clr->name()), "g") == 0)
																			objAxesColor.Oz.y = atof(clr->value());

																		else
																			if (strcmp(_strlwr(clr->name()), "b") == 0)
																				objAxesColor.Oz.z = atof(clr->value());
																			else
																				;
															else
																;
											else
												if (strcmp(_strlwr(q->name()), "camaxes") == 0)
													for (xml_node<>* axes = q->first_node(); axes; axes = axes->next_sibling())
														if (strcmp(_strlwr(axes->name()), "oxcolor") == 0)
															for (xml_node<>* clr = axes->first_node(); clr; clr = clr->next_sibling())
																if (strcmp(_strlwr(clr->name()), "r") == 0)
																	camAxesColor.Ox.x = atof(clr->value());
																else
																	if (strcmp(_strlwr(clr->name()), "g") == 0)
																		camAxesColor.Ox.y = atof(clr->value());
																	else
																		if (strcmp(_strlwr(clr->name()), "b") == 0)
																			camAxesColor.Ox.z = atof(clr->value());
																		else
																			;
														else
															if (strcmp(_strlwr(axes->name()), "oycolor") == 0)
																for (xml_node<>* clr = axes->first_node(); clr; clr = clr->next_sibling())
																	if (strcmp(_strlwr(clr->name()), "r") == 0)
																		camAxesColor.Oy.x = atof(clr->value());
																	else
																		if (strcmp(_strlwr(clr->name()), "g") == 0)
																			camAxesColor.Oy.y = atof(clr->value());
																		else
																			if (strcmp(_strlwr(clr->name()), "b") == 0)
																				camAxesColor.Oy.z = atof(clr->value());
																			else
																				;
															else
																if (strcmp(_strlwr(axes->name()), "ozcolor") == 0)
																	for (xml_node<>* clr = axes->first_node(); clr; clr = clr->next_sibling())
																		if (strcmp(_strlwr(clr->name()), "r") == 0)
																			camAxesColor.Oz.x = atof(clr->value());
																		else
																			if (strcmp(_strlwr(clr->name()), "g") == 0)
																				camAxesColor.Oz.y = atof(clr->value());
																			else
																				if (strcmp(_strlwr(clr->name()), "b") == 0)
																					camAxesColor.Oz.z = atof(clr->value());
																				else
																					;
																else
																	;
												else
													;
																	

									}
									else
										if (strcmp(_strlwr(p->name()), "fog") == 0) {
											for (xml_node <>* q = p->first_node(); q; q = q->next_sibling())
												if (strcmp(_strlwr(q->name()), "color") == 0) {
													for (xml_attribute<>* attr = q->first_attribute(); attr; attr = attr->next_attribute())
														if (strcmp(_strlwr(attr->name()), "blend") == 0)
															if (strcmp(_strlwr(attr->value()), "true") == 0)
																this->fog.blendToSkybox = true;
															else
																;
														else
															;
													/// fog.blendToSkybox false elseway from destructor
													if (this->fog.blendToSkybox == false) {
														for (xml_node<>* color = q->first_node(); color; color = color->next_sibling())
															if (strcmp(_strlwr(color->name()), "r") == 0)
																this->fog.color.x = atof(color->value());
															else
																if (strcmp(_strlwr(color->name()), "g") == 0)
																	this->fog.color.y = atof(color->value());
																else
																	if (strcmp(_strlwr(color->name()), "b") == 0)
																		this->fog.color.z = atof(color->value());
													}
												}
												else
													if (strcmp(_strlwr(q->name()), "radiuses") == 0) {
														for (xml_node<>* r = q->first_node(); r; r = r->next_sibling())
															if (strcmp(_strlwr(r->name()), "small") == 0)
																this->fog.smallRadius = atof(r->value());
															else
																if (strcmp(_strlwr(r->name()), "big") == 0)
																	this->fog.bigRadius = atof(r->value());
													}
												
										}
							

			
}

string SceneManager::getGameName() { return this->gameName; }

int SceneManager::getScreenHeight() {
	return this->screenSize.height;
}

int SceneManager::getScreenWidth() {
	return this->screenSize.width;
}

Vector4 SceneManager::getBgColor() {
	return bgcolor;
}

vector<SceneManager::action> SceneManager::getActionsFromKey(unsigned char key) {
	vector<action> ret;
	auto p = keysToControls.equal_range(key);
	for (auto& q = p.first; q != p.second; ++q)
		ret.push_back(q->second);
	return ret;
}

unordered_multimap<unsigned char, SceneManager::action> SceneManager::getKeysToControls() {
	return keysToControls;
}

Camera* SceneManager::getCameraWithId(int id) {
	Camera* ret = NULL;
	auto p = cameras.find(id);
	if (p != cameras.end())
		ret = p->second;
	return ret;
}

int SceneManager::setActiveCamera(Camera* cam) {
	if (cam == nullptr)
		return -1;
	this-> activeCamera = cam;
	return 0;
}

Camera* SceneManager::getActiveCamera() {
	return this->activeCamera;
}

SceneObject* SceneManager::getSceneObject(int id) {
	SceneObject* ret = NULL;
	auto p = sceneObjects.find(id);
	if (p != sceneObjects.end())
		ret = p->second;
	return ret;
}

Vector3 SceneManager::getFogColor() {
	return this->fog.color;
}

int SceneManager::addCamera(int id, Camera* cam) {
	if (this->getCameraWithId(id) != nullptr) {
		this->cameras.insert(make_pair(id, cam));
		return 0;
	}
	return -1;
}

float SceneManager::getFogBigRadius() {
	return this->fog.bigRadius;
}

float SceneManager::getFogSmallRadius() {
	return this->fog.smallRadius;
}

unordered_map<int, SceneObject*>& SceneManager::getAllSceneObjects() {
	return this->sceneObjects;
}

Model* SceneObject::getModel() {
	return this->model;
}

Shader* SceneObject::getShader() {
	return this->shader;
}

std::unordered_map<int,Texture*>& SceneObject::getTextures() {
	return this->textures;
}

int SceneObject::Init() {
/// model
	int n;
	glEnable(GL_DEPTH_TEST);
	this->model->Load();
	/// indices
	if (this->model->iboId == 0) {
		glGenBuffers(1, &this->model->iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);

		/// convert to memory block
		n = this->model->mr->indices.size();

		Vector3_uhint* indices_data = new Vector3_uhint[n];
		for (int i = 0; i < n; ++i)
			indices_data[i] = this->model->mr->indices[i];
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, n * sizeof(Vector3_uhint), indices_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		delete[] indices_data;
	}
	/// vertices
	if(this->model->vboId == 0)
		glGenBuffers(1, &this->model->vboId);

	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
		
		/// convert to memory block
	n = this->model->mr->vertices.size();
	ModelVertex* vertices_data = new ModelVertex[n];
	for (int i = 0; i < n; ++i) {
		vertices_data[i] = this->model->mr->vertices[i];
		/*vertices_data[i].pos.x = (vertices_data[i].pos.x + this->position.x) * this->scale.x;
		vertices_data[i].pos.y = (vertices_data[i].pos.y + this->position.y) * this->scale.y;
		vertices_data[i].pos.z = (vertices_data[i].pos.z + this->position.z) * this->scale.z;*/
	}

	glBufferData(GL_ARRAY_BUFFER, n * sizeof(ModelVertex), vertices_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices_data;
/// textures
	if(this->type != SceneObject::TERRAIN)
	for (auto& txt : this->textures) 
		txt.second->Load();
/// shader
	if(this->shader->vertexShader != 0)
		if (this->shader->Init() != 0)
			return -1;
	return 0;
}

int SceneObject::Draw(Matrix &vp) {
	Matrix modelMatrix, translate, scale, rotateX, rotateY, rotateZ, MVP;
	translate.SetTranslation(this->position);
	scale.SetScale(this->scale);
	rotateX.SetRotationX(this->rotation.x);
	rotateY.SetRotationY(this->rotation.y);
	rotateZ.SetRotationZ(this->rotation.z);
	
	modelMatrix = rotateZ * rotateY * rotateX * scale * translate;

	MVP = modelMatrix * vp;
	glUseProgram(this->shader->program);
	glBindBuffer(GL_ARRAY_BUFFER, this->model->vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->model->iboId);

	int i = 0;
	for (auto& x : this->textures)
		if (x.second->type == GL_TEXTURE_2D) {
			glBindTexture(GL_TEXTURE_2D, x.second->glTextureId);
			glActiveTexture(GL_TEXTURE0 + i);
			if (this->shader->textureUniform != -1)
				glUniform1i(this->shader->textureUniform, i);
		}
		else
			if (x.second->type == GL_TEXTURE_CUBE_MAP) {
				glBindTexture(GL_TEXTURE_CUBE_MAP, x.second->glTextureId);
				glActiveTexture(GL_TEXTURE_CUBE_MAP);
			}

	if (this->shader->positionAttribute != -1) {
		glEnableVertexAttribArray(this->shader->positionAttribute);
		glVertexAttribPointer(this->shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), 0);
	}
	if (this->shader->colorAttribute != -1) {
		glEnableVertexAttribArray(this->shader->colorAttribute);
		glVertexAttribPointer(this->shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*)sizeof(Vector3));
	}
	if (this->shader->cam_matrixUniform != -1) {
		glUniformMatrix4fv(this->shader->cam_matrixUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}
	if (this->shader->normalAttribute != -1) {
		glEnableVertexAttribArray(this->shader->normalAttribute);
		glVertexAttribPointer(this->shader->normalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*)(2 * sizeof(Vector3)));
	}
	if (this->shader->binormalAttribute != -1) {
		glEnableVertexAttribArray(this->shader->binormalAttribute);
		glVertexAttribPointer(this->shader->binormalAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*)(3 * sizeof(Vector3)));
	}
	if (this->shader->tangentAttribute != -1) {
		glEnableVertexAttribArray(this->shader->tangentAttribute);
		glVertexAttribPointer(this->shader->tangentAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*)(4 * sizeof(Vector3)));
	}
	if (this->shader->uvAttribute != -1) {
		glEnableVertexAttribArray(this->shader->uvAttribute);
		glVertexAttribPointer(this->shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (GLvoid*)(5 * sizeof(Vector3)));
	}

	if (this->shader->cubeTextureUniform != -1)
		glUniform1i(this->shader->cubeTextureUniform, 0); 

	if (this->shader->fogColorUniform != -1) {
		Vector3 fogColor = SceneManager::getInstance()->getFogColor();
		glUniform3f(this->shader->fogColorUniform, (GLfloat)fogColor.x, (GLfloat)fogColor.y, (GLfloat)fogColor.z);
	}

	if (this->shader->camPosition != -1) {
		Vector3 campos = SceneManager::getInstance()->getActiveCamera()->getPosition();
		glUniform3f(this->shader->camPosition, (GLfloat)campos.x, (GLfloat)campos.y, (GLfloat)campos.z);
	}

	if (this->shader->smallRadius != -1)
		glUniform1f(this->shader->smallRadius, SceneManager::getInstance()->getFogSmallRadius());

	if (this->shader->bigRadius != -1)
		glUniform1f(this->shader->bigRadius, SceneManager::getInstance()->getFogBigRadius());

	glDrawElements(GL_TRIANGLES, 3 * this->model->mr->indices.size(), GL_UNSIGNED_SHORT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return 0;
}

SkyBox::SkyBox() : SceneObject() {
}

SkyBox::SkyBox(SceneObject* sobj, float offsetY) {
	this->id = sobj->id;
	this->modelId = sobj->modelId;
	this->shaderId = sobj->shaderId;
	this->textureIds = sobj->textureIds;
	this->model = sobj->model;
	this->shader = sobj->shader;
	this->textures = sobj->textures;
	this->depthTest = sobj->depthTest;
	this->wired = sobj->wired;
	this->generatedModel = sobj->generatedModel;
	this->type = sobj->type;
	this->name = sobj->name;
	this->position = sobj->position;
	this->rotation = sobj->rotation;
	this->scale = sobj->scale;
	this->color = sobj->color;
	this->followingCamera = sobj->followingCamera;

	this->position.x = 0;
	this->position.z = 0;
	this->position.y = offsetY;
}

void SceneObject::generateModel() {
	if (this->type = SceneObject::TERRAIN) {
		this->generatedModel = true;
		if (!this->model) {
			this->model = new Model;
			this->model->generated = true;
		}
		if(!this->model->mr)
			this->model->mr = new ModelResource;
		ModelResource* mdlres = this->model->mr;
		mdlres->vertices.clear();
		mdlres->indices.clear();
		/// generate indices & vertices
		ModelVertex vertex;
		int square_width = grid_width / grid_dimension;
		int square_height = grid_height / grid_dimension;
		vertex.tgt = Vector3(0, 0, 0);
		vertex.binorm = Vector3(0, 1, 0);
		vertex.norm = Vector3(0, 1, 0);
		for (int i = 0; i <= grid_dimension; ++i)
			for (int j = 0; j <= grid_dimension; ++j) {
				vertex.uv.x = j * square_width;
				vertex.uv.y = i * square_height;
				float scale = vertex.uv.Length(); 
				vertex.uv = vertex.uv.Normalize();
				vertex.pos.x = vertex.uv.x * scale - (float)grid_width / 2;
				vertex.pos.z = vertex.uv.y * scale  - (float)grid_height / 2;
				mdlres->vertices.push_back(vertex);
			}
		for (unsigned short int i = 0; i < grid_dimension; ++i)
			for (unsigned short int j = 0; j < grid_dimension; ++j) {
				Vector3_uhint vec;
				vec.x = i * (grid_dimension + 1) + j;
				vec.y = i * (grid_dimension + 1) + j + 1;
				vec.z = (i+1) * (grid_dimension + 1) + j;
				mdlres->indices.push_back(vec);

				vec.x = (i + 1) * (grid_dimension + 1) + j;
				vec.y = (i + 1) * (grid_dimension + 1) + j + 1;
				vec.z = i * (grid_dimension + 1) + j + 1;
				mdlres->indices.push_back(vec);
			}
	}
}

SceneObject* SceneManager::getSceneObjectByType(SceneObject::objType type) {
	for (auto& x : sceneObjects)
		if (x.second->type == type)
			return x.second;
	return nullptr;
}

ModelResource* Model::getResource() {
	return this->mr;
}

unsigned short int SceneObject::getGridDimension() {
	return this->grid_dimension;
}

float SceneObject::getGridWidth() {
	return this->grid_width;
}

float SceneObject::getGridHeight() {
	return this->grid_height;
}

bool SceneObject::isOnDebug() {
	return this->debug;
}