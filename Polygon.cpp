#include "Polygon.h"
#include <iostream>

Polygon::Polygon() {
	this->_size = 0;
	this->mean_z = 1;
}

Polygon::Polygon(std::vector<Vertex>& vertices) {
	this->_size = vertices.size();
	this->mean_z = 0;

	for (Vertex& v : vertices) {
		this->vertices.push_back(std::make_shared<Vertex>(v));
		this->mean_z += v.z;
	}

	this->mean_z /= this->vertices.size();
}


int Polygon::size() const {
	return this->_size;
}

float Polygon::get_z(const Matrix& transform) const {
	float mean_z = 0.f;
	std::array<float, 4> t;

	for (int i = 0; i < 4; i++) {
		t[i] = transform.get(2, i);
	}


	for (const auto& v : this->vertices) {
		mean_z += v->x * t[0] + v->y * t[1] + v->z * t[2] + v->w * t[3];
	}

	return mean_z / this->vertices.size();
}

void Polygon::draw(sf::Image& image, sf::Color color) { // scan-line
	auto topdown = std::minmax_element(
		this->vertices.begin(), 
		this->vertices.end(), 
		[](const auto& a, const auto& b) -> bool { 
			return a->y > b->y; 
		}
	);
	std::shared_ptr<Vertex> top, down;
	top = *topdown.first;
	down = *topdown.second;

	std::cout << top->y << " " << down->y << std::endl;
}

void Polygon::add_vertex(const Vertex& vertex) {
	this->vertices.push_back(std::make_shared<Vertex>(vertex));
}

void Polygon::add_vertex(std::shared_ptr<Vertex> vp) {
	this->vertices.push_back(std::move(vp));
}

std::unique_ptr<sf::VertexArray> Polygon::vertices_sfml(const Matrix& transform, float min_z, float max_z) const {
	std::unique_ptr<sf::VertexArray> va = std::make_unique<sf::VertexArray>();
	int bright;
	
	//va->setPrimitiveType((this->vertices.size() == 4) ? sf::Quads : sf::TriangleStrip);

	va->setPrimitiveType(sf::TriangleFan);

	Vertex vertex_copy;
	
	for (const auto& v : this->vertices) {
		vertex_copy = *v;
		vertex_copy.apply_transform(transform);
		bright = (vertex_copy.z - min_z) / ((max_z - min_z) / (255 - 10)) + 10;
		va->append(
			sf::Vertex(
				sf::Vector2f(vertex_copy.x, vertex_copy.y),
				//sf::Color(bright, bright, bright)
				sf::Color(bright, 120, 80) // 160 20
			)
		);
	}

	return va;
}

std::unique_ptr<sf::VertexArray> Polygon::vertices_sfml(const Matrix& transform, int bright) const {
	std::unique_ptr<sf::VertexArray> va = std::make_unique<sf::VertexArray>();
	va->setPrimitiveType(sf::TriangleFan);

	Vertex vertex_copy;

	for (const auto& v : this->vertices) {
		vertex_copy = v->with_transform(transform);

		va->append(
			sf::Vertex(
				sf::Vector2f(vertex_copy.x, vertex_copy.y),
				//sf::Color(bright, bright, bright)
				sf::Color(bright, 120, 80)
			)
		);
	}

	return va;
}