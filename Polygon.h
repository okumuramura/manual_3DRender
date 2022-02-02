#pragma once
#include "Vertex.h"
#include "Matrix.h"
#include "utils.h"
#include <vector>

#include <SFML/Graphics.hpp>

class Polygon
{
public:
	int size() const;
	float get_z(const Matrix&) const;

	std::vector<std::shared_ptr<Vertex>> vertices;
	Polygon();
	Polygon(std::vector<Vertex>&);
	void add_vertex(const Vertex&);
	void add_vertex(std::shared_ptr<Vertex>);
	void draw(sf::Image&, sf::Color = sf::Color::White);
	
	std::unique_ptr<sf::VertexArray> vertices_sfml(const Matrix&, float, float) const;
	std::unique_ptr<sf::VertexArray> vertices_sfml(const Matrix&, int) const;
	
private:
	int _size;
	float mean_z;
};

