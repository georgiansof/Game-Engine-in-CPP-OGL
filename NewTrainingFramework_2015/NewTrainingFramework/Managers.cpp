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
														 		 "MIRRORED_REPEAT",
																 "CLAMP_TO_BORDER",
																 "MIRROR_CLAMP_TO_EDGE" });
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
													case 3:
														wrap_s = Texture::CLAMP_TO_BORDER;
														break;
													case 4:
														wrap_s = Texture::MIRROR_CLAMP_TO_EDGE;
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
														case 3:
															wrap_t = Texture::CLAMP_TO_BORDER;
															break;
														case 4:
															wrap_t = Texture::MIRROR_CLAMP_TO_EDGE;
															break;
														}
														break;
													}
													++i;
												}
											}
						int id = atoi(attr->value());
;						this->textures.insert(make_pair(id, new Texture(id,filepath, min_filter, mag_filter, wrap_s, wrap_t)));
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
	if (path.length() == 0) {
		cout << "Model path length is zero!\n";
		return -1;
	}
	if (mr) {
		mr->indices.clear();
		mr->vertices.clear();
	}
	else 
		mr = new ModelResource;
	NFG_Parser::Parse(path, mr->vertices, mr->indices);
	nrIndices = mr->indices.size();
	///nrIndicesWired = ??? 
	if(iboId == -1) glGenBuffers(1, &iboId);
	if(vboId == -1) glGenBuffers(1, &vboId);
	return 0;
}

int Model::Unload() {
	if (mr) {
		mr->indices.clear();
		mr->vertices.clear();
		delete mr;

		return 0;
	}
	if(iboId != -1) glDeleteBuffers(1, &iboId);
	if(vboId != -1) glDeleteBuffers(1, &vboId);
	iboId = -1;
	wiredIboId = -1;
	vboId = -1;
	return -1;
}

Model::Model() {
	mr = NULL;
	iboId = -1;
	wiredIboId = -1;
	vboId = -1;
	nrIndices = -1;
	nrIndicesWired = -1;
}

Model::Model(string path) {
	mr = NULL;
	iboId = -1;
	wiredIboId = -1;
	vboId = -1;
	nrIndices = -1;
	nrIndicesWired = -1;
	this->path.assign(path.c_str());
}

Model::~Model() {
	this->Unload();
}

int Texture::getTextureId() { return textureId; }

int Texture::Load() {
	if (tr)
		return -1;
	tr = new TextureResource;
	tr->pixel_array = (unsigned char*) LoadTGA(path.c_str(), &tr->texture_height, &tr->texture_width, &tr->bpp);
	if (tr->pixel_array == NULL) {
		delete tr;
		return -1;
	}
	if(textureId == -1) 
		glGenTextures(1, &textureId);

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
	if (textureId != -1)
		glDeleteTextures(1, &textureId);
	textureId = -1;
	return -1;
}
Texture::Texture() {
	tr = NULL;

	textureId = -1;
	this->min_filter = Texture::UNSETF;
	this->mag_filter = Texture::UNSETF;
	this->wrap_s = Texture::UNSETW;
	this->wrap_t = Texture::UNSETW;
}
Texture::Texture(int textureId, std::string path, Texture::filtering_type min_filter, Texture::filtering_type mag_filter, Texture::wrapping_type wrap_s, Texture::wrapping_type wrap_t) {
	tr = NULL;
	this->textureId = textureId;
	this->path.assign(path.c_str());
	this->min_filter=min_filter;
	this->mag_filter=mag_filter;
	this->wrap_s=wrap_s;
	this->wrap_t=wrap_t;
}
Texture::~Texture() {
	this->Unload();
}

Shader::Shader() {
	sr = NULL;
	progId = -1;
	vsId = -1;
	fsId = -1;
}

Shader::Shader(string FSpath, string VSpath) {
	progId = -1;
	vsId = -1;
	fsId = -1;
	sr = new ShaderResource;
	sr->FragmentShader = FSpath;
	sr->VertexShader = VSpath;
}

int Shader::Load(string FSpath, string VSpath) {
	if (!sr)
		sr = new ShaderResource;
	sr->FragmentShader = FSpath;
	sr->VertexShader = VSpath;
	return Load();
}

int Shader::Load() {
	if (!sr) {
		cout << "Error due trial to load a shader with no VS & FS path specified";
		return -1;
	}

	vsId = esLoadShader(GL_VERTEX_SHADER,(char*) sr->VertexShader.c_str());
	if (vsId == 0) {
		vsId = -1;
		cout << "Failed to load Vertex Shader\n";
		return -1;
	}
	fsId = esLoadShader(GL_FRAGMENT_SHADER,(char*) sr->FragmentShader.c_str());
	if (fsId == 0) {
		fsId = -1;
		glDeleteShader(vsId);
		vsId = -1;
		cout << "Failed to load Fragment Shader\n";
		return -1;
	}
	progId = esLoadProgram(vsId, fsId);
	return 0;
}

int Shader::Unload() {
	if (sr != NULL)
		delete sr;
	int x = progId;
	progId = -1;

	if (vsId != -1)
		glDeleteShader(vsId);
	if (fsId != -1)
		glDeleteShader(fsId);
	if (progId != -1)
		glDeleteProgram(progId);
	return x;
}

Shader::~Shader() {
	Unload();
}

SceneObject::SceneObject() {}

SceneObject::~SceneObject() {
	textures.clear();
	textureIds.clear();
}

SceneObject::SceneObject(string name, Vector3 position, Vector3 rotation, Vector3 scale, bool depthTest, int modelId, int ShaderId, vector<int> &textureIds) {
	this->name = name;
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
		textures.push_back(ResourceManager::getInstance()->getTexture(x));
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
									if (camId != -1)
										this->cameras.insert(make_pair(camId, cam));
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
														objptr->model = nullptr;
														objptr->generatedModel = true;
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
																				objptr->textures.push_back(ResourceManager::getInstance()->getTexture(atoi(attr->value())));
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
																				objptr->rotation.x = atof(nd->value());
																			else
																				if (strcmp(_strlwr(nd->name()), "y") == 0)
																					objptr->rotation.y = atof(nd->value());
																				else
																					if (strcmp(_strlwr(nd->name()), "z") == 0)
																						objptr->rotation.z = atof(nd->value());
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
																								;
																					else
																						;
											this->sceneObjects.insert(make_pair(objptr->id, objptr));
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