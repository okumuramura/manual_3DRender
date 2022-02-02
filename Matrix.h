#pragma once
#include <array>
#include <iostream>
#include <SFML/Graphics.hpp>

class Matrix
{
public:
	float get(int, int) const;
	void set(int, int, float);
	void rotate(float, float, float);
	void scale(float, float, float);
	void translate(float, float, float);

	Matrix operator*(const Matrix&);

	static Matrix rotate_matrix(float, float, float, sf::Vector3f origin = {0.f, 0.f, 0.f});
	static Matrix scale_matrix(float, float, float, sf::Vector3f origin = {0.f, 0.f, 0.f});
	static Matrix translate_matrix(float, float, float);

private:
	std::array<std::array<float, 4>, 4> mat = { {
					{1.f, 0.f, 0.f, 0.f},
					{0.f, 1.f, 0.f, 0.f},
					{0.f, 0.f, 1.f, 0.f},
					{0.f, 0.f, 0.f, 1.f}
				} };
};

