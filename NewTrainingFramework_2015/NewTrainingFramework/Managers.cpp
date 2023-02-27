#include "stdafx.h"
#include "Managers.h"
#include <iostream>


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
	///TODO
	return 0;
}

int Model::Unload() {
	if (mr) {
		mr->indices.clear();
		mr->vertices.clear();
		delete mr;

		return 0;
	}
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
	return 0;
}

int Texture::GetTextureResource(TextureResource *res) {
	if (tr) {
		*res = *tr;
		return 0;
	}
	else
		return -1;
}

string Texture::getTGA() {return this->path;}

int Texture::Unload() {
	if (tr != NULL) {
		if (tr->pixel_array != NULL)
			delete tr->pixel_array;
		delete tr;
		return 0;
	}
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
}

Shader::Shader(string FSpath, string VSpath) {
	progId = -1;
	sr = new ShaderResource;
	sr->FragmentShader = FSpath;
	sr->VertexShader = VSpath;
}

int Shader::Load(string FSpath, string VSpath) {
	/// TODO progId
	return 0;
}

int Shader::Unload() {
	if (sr == NULL)
		return -1;
	delete sr;
	int x = progId;
	progId = -1;
	return x;
}

Shader::~Shader() {
	Unload();
}