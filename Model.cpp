#include "Model.h"
#include <string>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>

Model::Model() {
	
}

Model::Model(const std::string path) {
	this->load(path);
}

bool Model::load(const std::string path) {
	std::ifstream obj_file;
	std::string line;
	std::string type;
	std::stringstream stream;

	std::shared_ptr<Vertex> vertex;
	Polygon* polygon;
	float x, y, z, w;
	obj_file.open(path);

	if (!obj_file.is_open())
		return false;

	while (!obj_file.eof()) {
		std::getline(obj_file, line);
		stream = std::stringstream(line);
		w = 1.f;
		stream >> type;

		if (type == "v") {
			stream >> x >> y >> z >> w;
			this->vertices.push_back(std::make_shared<Vertex>(x, y, z, w));
		}
		else if (type == "f") {
			polygon = new Polygon();
			while (stream >> x) {
				stream >> type;
				polygon->add_vertex(this->vertices.at(x-1));
			}
			this->polygons.push_back(*polygon);
		}
	}
	return true;
}

void Model::fit(const sf::RenderWindow& window) {
	this->fit(window.getSize().x, window.getSize().y);
}

void Model::fit(int width, int height) {
	// TODO adaptive fit

	Vertex minv(FLT_MAX, FLT_MAX, FLT_MAX);
	Vertex maxv(FLT_MIN, FLT_MIN, FLT_MIN);

	for (const std::shared_ptr<Vertex>& v : this->vertices) {
		if (v->x > maxv.x) maxv.x = v->x;
		else if (v->x < minv.x) minv.x = v->x;
		if (v->y > maxv.y) maxv.y = v->y;
		else if (v->y < minv.y) minv.y = v->y;
		if (v->z > maxv.z) maxv.z = v->z;
		else if (v->z < minv.z) minv.z = v->z;
	}

	Matrix center = Matrix::translate_matrix(static_cast<float>(width / 2), static_cast<float>(height / 2), 0.f);
	center = center * Matrix::scale_matrix(3.f, 3.f, 3.f);

	//this->transform = this->transform * translate * scale;
	this->transform = this->transform * center;
}

void Model::draw_vertices(sf::Image& image, sf::Color color){
	int x, y;
	sf::Vector2u size = image.getSize();
	for (const auto& v : this->vertices) {
		if (v->x > 0 && v->x < size.x && v->y > 0 && v->y < size.y)
			image.setPixel(v->x, size.y - v->y, color);
	}
}

void Model::draw_wireframe(sf::Image& image, sf::Color color){
	int x, y;
	std::shared_ptr<Vertex> v1, v2;
	Point lt = { 1, image.getSize().y - 2};
	Point rb = { image.getSize().x - 2, 1};
	Point p1, p2;
	Vertex vertex;
	bool fi;
	for (const Polygon& p : this->polygons) {
		fi = true;
		for (const auto& v : p.vertices) {
			vertex = *v;
			vertex.apply_transform(this->transform);
			p2 = { (int)vertex.x, (int)(image.getSize().y - vertex.y) };
			if (!fi) {
				if (utils::line_cut(p1, p2, lt, rb) != -1)
					utils::draw_line(image, p1, p2, color);
			}
			fi = false;
			p1 = p2;
		}
	}

	if (this->origin.x > 0 && this->origin.x < image.getSize().x && this->origin.y > 0 && this->origin.y < image.getSize().y)
		image.setPixel((int)(this->origin.x), (int)(image.getSize().y - this->origin.y), sf::Color::Red);
}

void Model::draw_poly(sf::RenderWindow& window, bool smooth){

	const Matrix& t = this->transform;
	std::sort(
		this->polygons.begin(),
		this->polygons.end(),
		[&t](const Polygon& a, const Polygon& b) {
			return a.get_z(t) < b.get_z(t);
		}
	);

	float max_z, min_z;
	if (!smooth) {
		min_z = (*(this->polygons.begin())).get_z(t);
		max_z = (*(this->polygons.end() - 1)).get_z(t);
	}
	else {
		auto minmax = std::minmax_element(
			this->vertices.begin(),
			this->vertices.end(),
			[&t](const std::shared_ptr<Vertex>& a, const std::shared_ptr<Vertex>& b) {
				return a->with_transform(t).z < b->with_transform(t).z;
			}
		);

		min_z = (*(minmax.first))->with_transform(t).z;
		max_z = (*(minmax.second))->with_transform(t).z;
	}

	float z = 0.f;
	std::unique_ptr<sf::VertexArray> va;

	for (const auto& polygon : this->polygons) {
		if (!smooth) {
			z = (polygon.get_z(t) - min_z) / ((max_z - min_z) / (255 - 10)) + 10;
			va = polygon.vertices_sfml(this->transform, z);
		}
		else {
			va = polygon.vertices_sfml(this->transform, min_z, max_z);
		}
		for (int i = 0; i < va->getVertexCount(); i++) {
			va->operator[](i).position.y = window.getSize().y - va->operator[](i).position.y;
		}
		window.draw(*va);
	}
}

void Model::scale(float scale) {
	Matrix scale_matrix = Matrix::scale_matrix(scale, scale, scale);
	this->transform = this->transform * scale_matrix;
	/*for (auto& v : this->vertices) {
		v->apply_transform(scale_matrix);
	}*/
}

void Model::move(float x, float y, float z) {
	Matrix translate_matrix = Matrix::translate_matrix(x, y, z);
	this->transform = this->transform * translate_matrix;
	/*this->origin += sf::Vector3f(x, y, z);
	for (auto& v : this->vertices) {
		v->apply_transform(translate_matrix);
	}*/
}

void Model::rotate(float x, float y, float z) {
	Matrix rotate_matrix;
	if (x) this->transform = this->transform * Matrix::rotate_matrix(x, 0.f, 0.f);
	if (y) this->transform = this->transform * Matrix::rotate_matrix(0.f, y, 0.f);
	if (z) this->transform = this->transform * Matrix::rotate_matrix(0.f, 0.f, z);
	//this->transform = this->transform * rotate_matrix;
	/*for (auto& v : this->vertices) {
		v->apply_transform(rotate_matrix);
	}*/
}

void Model::reset() {
	this->transform = Matrix();
}