#pragma once
#include <iostream>
#include "Polygon.h"
#include "Matrix.h"
#include <SFML/Graphics.hpp>

class Model
{
public:
	std::vector<Polygon> polygons;
	std::vector<std::shared_ptr<Vertex>> vertices;

	Model();
	Model(const std::string);
	bool load(const std::string);
	void draw_vertices(sf::Image&, sf::Color = sf::Color::White);
	void draw_wireframe(sf::Image&, sf::Color = sf::Color::White);
	void draw_poly(sf::RenderWindow&, bool = true);
	void fit(int, int);
	void fit(const sf::RenderWindow&);
	void rotate(float, float, float);
	void move(float, float, float);
	void scale(float);
	void reset();

private:
	sf::Vector3f origin = {0.f, 0.f, 0.f};
	Matrix transform;
};

