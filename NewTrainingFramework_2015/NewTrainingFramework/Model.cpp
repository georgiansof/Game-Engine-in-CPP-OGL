#include "stdafx.h"
#include "Model.h"
#include "Vertex.h"
#include <fstream>
#include <cstring>

#define THROW_INCORRECT(variable) { \
fprintf(stderr, "Incorrect parsed file format. var:[%s] index:[%d]\n",variable,i); \
exit(EXIT_FAILURE); \
		}

void Parser::Parse(std::string file_full_path, std::vector<ModelVertex> &mdl, std::vector<Vector3_uhint> &indices) {
	std::ifstream fin(file_full_path);
	char sep[] = " :[,;";
	char buff[256];
	int n;
	fin.getline(buff, 255);
	char *p = strtok(buff, sep);
	while (isalpha(p[0])) p = strtok(0, sep);
	if (p[0] >= '0' && p[0] <= '9') n = atoi(p); /// citire numar vertecsi
	else {
		fprintf(stderr, "Number of vertices numeral does not exist.");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < n; ++i) {
		Vector3 pos, norm, binorm, tgt;
		Vector2 uv;
		p = 0;
		fin.getline(buff, 255);
		p = strtok(buff, sep);

		while (!p) {
			fin.getline(buff, 255);
			p = strtok(buff, sep);  /// sare linii goale
		}
		if (!p) { /// nicio linie nu contine informatie relevanta
			fprintf(stderr, "The declared number of vertices is greater than the amount of information provided in the parsed file.\n");
			exit(EXIT_FAILURE);
		}
		int ind;
		ind = atoi(p);
		if (ind != i) {
			fprintf(stderr, "Wrong format in parsed file. Indexes are not contiguous.\nIndexing starts from 0.\n");
			exit(EXIT_FAILURE);
		}
		//pos
		//verifica daca punctul de dupa index e legat de cuvantul ,,pos''
		int j = 0;
		while (isalnum(p[j])) ++j;
		if (p[j] == '.') ++j;
		if (p[j] != '\0') p = p + j;
		else p = strtok(0, sep);

		if (!p || p[0] == '\0') {
			fprintf(stderr,"Not enough information to extract from parsed file.\n");
			exit(EXIT_FAILURE);
		}
		/// POS
		if (strcmp("pos", p) != 0) 
			THROW_INCORRECT("pos")
		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("pos")
		pos.x = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("pos")
		pos.y = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1])) 
			THROW_INCORRECT("pos")
		pos.z = atof(p);
		/// NORM
		p = strtok(0, sep);
		if (!p || strcmp(p,"norm")!=0)
			THROW_INCORRECT("norm")

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("norm")
		norm.x = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("norm")
		norm.y = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("norm")
		norm.z = atof(p);

		/// BINORM

		p = strtok(0, sep);
		if (!p || strcmp(p, "binorm") != 0)
			THROW_INCORRECT("binorm")

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("binorm")
		binorm.x = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("binorm")
		binorm.y = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("binorm")
		binorm.z = atof(p);

		/// TGT

		p = strtok(0, sep);
		if (!p || strcmp(p, "tgt") != 0)
			THROW_INCORRECT("tgt")

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("tgt")
		tgt.x = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("tgt")
		tgt.y = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("tgt")
		tgt.z = atof(p);

		/// UV

		p = strtok(0, sep);
		if (!p || strcmp(p, "uv") != 0)
			THROW_INCORRECT("uv")

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("uv")
		uv.x = atof(p);

		p = strtok(0, sep);
		if (!p || !isalnum(p[0]) && !isalnum(p[1]))
			THROW_INCORRECT("uv")
		uv.y = atof(p);
		ModelVertex aux;
		aux.pos = pos;
		aux.norm = norm;
		aux.binorm = binorm;
		aux.tgt = tgt;
		aux.uv = uv;
		mdl.push_back(aux);
	}

	fin.getline(buff, 255);
	p = strtok(buff, sep);
	p = strtok(0, sep);
	n = atoi(p)/3; /// TOFIX
	
	for (int i = 0; i < n; ++i) {
		fin.getline(buff, 255);
		p = strtok(buff, sep);
		Vector3_uhint vect;

		p = strtok(0, sep);
		vect.x = atoi(p);


		p = strtok(0, sep);
		vect.y = atoi(p);

		p = strtok(0, sep);
		vect.z = atoi(p);

		indices.push_back(vect);
	}
}





ModelVertex::ModelVertex() {
	
}

ModelVertex::ModelVertex(ModelVertex const &mv) {
	this->binorm = mv.binorm;
	this->norm = mv.norm;
	this->pos = mv.pos;
	this->tgt = mv.tgt;
	this->uv = mv.uv;
}