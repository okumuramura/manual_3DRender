#include "Vertex.h"
#include <array>

Vertex::Vertex() {
	this->x = 0.f;
	this->y = 0.f;
	this->z = 0.f;
	this->w = 1.f;
}

Vertex::Vertex(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void Vertex::apply_transform(const Matrix& mat) {
	std::array<float, 4> vec = {this->x, this->y, this->z, this->w};
	std::array<float, 4> res = {0.f, 0.f, 0.f, 0.f};

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i] += mat.get(i, j) * vec[j];
		}
	}

	this->x = res[0];
	this->y = res[1];
	this->z = res[2];
	this->w = res[3];
}

Vertex& Vertex::with_transform(const Matrix& transform) {
	std::array<float, 4> vec = { this->x, this->y, this->z, this->w };
	std::array<float, 4> res = { 0.f, 0.f, 0.f, 0.f };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i] += transform.get(i, j) * vec[j];
		}
	}

	Vertex new_vertex(res[0], res[1], res[2], res[3]);

	return new_vertex;
}