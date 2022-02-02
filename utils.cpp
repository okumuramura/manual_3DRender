#include "utils.h"

void utils::draw_line(sf::Image& image, Point p1, Point p2, sf::Color color) {
	const bool steep = (abs(p2.y - p1.y) > abs(p2.x - p1.x));
	if (steep) {
		std::swap(p1.x, p1.y);
		std::swap(p2.x, p2.y);
	}
	if (p1.x > p2.x) {
		std::swap<Point>(p1, p2);
	}

	int dx = abs(p2.x - p1.x);
	int dy = abs(p2.y - p1.y);
	float error = dx / 2.0f;
	const int ystep = (p1.y < p2.y) ? 1 : -1;
	int y = p1.y;

	const int maxX = p2.x;

	for (int x = p1.x; x <= maxX; x++) {
		if (steep) {
			image.setPixel(y, x, color);
		}
		else {
			image.setPixel(x, y, color);
		}

		error -= dy;
		if (error < 0) {
			y += ystep;
			error += dx;
		}
	}
}

int utils::line_cut(Point& p1, Point& p2, const Point& lt, const Point& rb) {
	int code_a, code_b, code;
	code_a = checkpos(p1, lt, rb);
	code_b = checkpos(p2, lt, rb);
	Point* c;

	while (code_a | code_b) {
		if (code_a & code_b) { // not in rectangle
			return -1;
		}

		if (code_a) {
			code = code_a;
			c = &(p1);
		}
		else {
			code = code_b;
			c = &(p2);
		}

		if (code & LEFT) {
			c->y += (p2.y - p1.y) * (lt.x - c->x) / (p2.x - p1.x);
			c->x = lt.x;
		}
		else if (code & RIGHT) {
			c->y += (p2.y - p1.y) * (rb.x - c->x) / (p2.x - p1.x);
			c->x = rb.x;
		}
		else if (code & BOT) {
			c->x += (p2.x - p1.x) * (rb.y - c->y) / (p2.y - p1.y);
			c->y = rb.y;
		}
		else if (code & TOP) {
			c->x += (p2.x - p1.x) * (lt.y - c->y) / (p2.y - p1.y);
			c->y = lt.y;
		}

		if (code == code_a) {
			p1 = *c;
			code_a = checkpos(p1, lt, rb);
		}
		else if (code == code_b) {
			p2 = *c;
			code_b = checkpos(p2, lt, rb);
		}
	}
	return 0;
}

int utils::checkpos(const Point& p, const Point& lt, const Point& rb) {
	return (((p.x < lt.x) ? LEFT : 0) +
		((p.x > rb.x) ? RIGHT : 0) +
		((p.y > lt.y) ? TOP : 0) +
		((p.y < rb.y) ? BOT : 0));
}

sf::Color& utils::gradient(const sf::Color& from, const sf::Color& to, float value) {
	sf::Color result;



	return result;
}