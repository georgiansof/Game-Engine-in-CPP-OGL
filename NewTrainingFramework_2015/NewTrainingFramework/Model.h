#pragma once

#include <vector>
#include <string>
#include "../Utilities/Math.h"

#include "../Utilities/utilities.h"

struct ModelVertex {
	Vector3 pos,color, norm, binorm, tgt;
	Vector2 uv, uv2;
	ModelVertex();
	ModelVertex(ModelVertex const &mv);
};

class NFG_Parser {
public:
	static void Parse(std::string file_full_path, std::vector<ModelVertex> &mdl, std::vector<Vector3_uhint> &indices);
};