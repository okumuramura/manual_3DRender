#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct Point {
	int x, y;
};

namespace utils {
	const int INSIDE = 0b0000;
	const int RIGHT = 0b0010;
	const int LEFT = 0b0001;
	const int BOT = 0b0100;
	const int TOP = 0b1000;

	void draw_line(sf::Image&, Point, Point, sf::Color = sf::Color::White);
	int line_cut(Point&, Point&, const Point&, const Point&);
	int checkpos(const Point&, const Point&, const Point&);
	sf::Color& gradient(const sf::Color&, const sf::Color&, float);
}