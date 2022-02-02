#pragma once
#include "Matrix.h"

class Vertex
{
public:
	float x, y, z, w;

	Vertex();
	Vertex(float, float, float, float = 1.f);
	void apply_transform(const Matrix&);
	Vertex& with_transform(const Matrix&);
	~Vertex() {};
};

